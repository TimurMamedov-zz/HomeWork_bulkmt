#include <fstream>
#include "solvers.h"
#include "commands_storage.h"

SaveSolver::SaveSolver(ThreadSave_Queue<std::pair<std::vector<std::string>, std::chrono::_V2::system_clock::time_point> > &queue_,
                       std::atomic_bool& finish_)
    :Solver(finish_), queue(queue_)
{
}

PrintSolver::PrintSolver(ThreadSave_Queue<std::vector<std::string> > &queue_,
                         std::atomic_bool& finish_)
    :Solver(finish_), queue(queue_)
{
}

void SaveSolver::operator()()
{
    while(queue.empty() && !finish)
    {
        std::pair<std::vector<std::string>, std::chrono::system_clock::time_point> commandPair;
        queue.wait_and_pop(commandPair);
        std::ofstream file;
        std::stringstream ss;
        ss << std::chrono::system_clock::to_time_t(commandPair.second);
        ss << "_" << std::this_thread::get_id();
        file.open(std::string("bulk") + ss.str() + ".log");
        file << bulkCommandString(commandPair.first);
        file.close();

        increaseCounts(commandPair.first.size());
    }
}

void PrintSolver::operator()()
{
    while(queue.empty() && !finish)
    {
        std::vector<std::string> commandVector;
        queue.wait_and_pop(commandVector);
        std::cout << bulkCommandString(commandVector) << "\n";

        increaseCounts(commandVector.size());
    }
}
