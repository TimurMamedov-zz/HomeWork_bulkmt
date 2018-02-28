#include <fstream>
#include "solvers.h"
#include "commands_storage.h"

SaveSolver::SaveSolver(ThreadSave_Queue<std::pair<std::string, std::chrono::system_clock::time_point> >& queue_,
                       std::atomic_bool& finish_)
    :queue(queue_), finish(finish_)
{
}

PrintSolver::PrintSolver(ThreadSave_Queue<std::string>& queue_,
                         std::atomic_bool& finish_)
    :queue(queue_), finish(finish_)
{
}

void SaveSolver::operator()()
{
    while(queue.empty() && !finish)
    {
        std::pair<std::string, std::chrono::system_clock::time_point> commandPair;
        queue.wait_and_pop(commandPair);
        std::ofstream file;
        std::stringstream ss;
        ss << std::chrono::system_clock::to_time_t(commandPair.second);
        ss << "_" << std::this_thread::get_id();
        file.open(std::string("bulk") + ss.str() + ".log");
        file << commandPair.first;
        file.close();
    }
}

void PrintSolver::operator()()
{
    while(queue.empty() && !finish)
    {
        std::string commandString;
        queue.wait_and_pop(commandString);
        std::cout << commandString << "\n";
    }
}
