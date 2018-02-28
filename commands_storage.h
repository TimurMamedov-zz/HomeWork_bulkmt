/*!
\file
\brief
*/
#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <memory>
#include <thread>
#include <atomic>

#include "solver.h"
#include "threadsafe_queue.h"

class CommandsStorage
{
public:
    explicit CommandsStorage(std::size_t bulkSize);
    ~CommandsStorage();

    void addString(const std::string& str);

    std::string bulkCommandString() const;

private:
    std::stack<std::string> bracketStack;
    std::vector<std::string> commandsVector;
    std::chrono::system_clock::time_point firstBulkTime;
    const std::size_t bulkSize_;
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<Solver> > solvers;
    std::atomic_bool finish;

    ThreadSave_Queue<std::pair<std::string, std::chrono::system_clock::time_point> > file_queue;
    ThreadSave_Queue<std::string> log_queue;

    void addCommand(const std::string& command);
    void addBracket(const std::string& bracket);
    void queues_push();
    void forcing_push();
};
