/*!
\file
\brief
*/
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSave_Queue
{
public:
    explicit ThreadSave_Queue() = default;
    ThreadSave_Queue(const ThreadSave_Queue& ) = delete;
    ThreadSave_Queue& operator =(const ThreadSave_Queue& ) = delete;
    ~ThreadSave_Queue() = default;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mt);
        queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mt);
        data_cond.wait(lk, [this]{ return !queue.empty();});
        value = queue.front();
        queue.pop();
    }

private:
    std::queue<T> queue;
    mutable std::mutex mt;
    std::condition_variable data_cond;
};
