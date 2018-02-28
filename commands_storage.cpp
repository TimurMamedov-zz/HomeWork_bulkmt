#include "commands_storage.h"
#include "solvers.h"
#include <thread>

CommandsStorage::CommandsStorage(std::size_t bulkSize) : bulkSize_(bulkSize)
{
    commandsVector.reserve(bulkSize_);
    finish.store(false);
    auto autoSavingSolver1 = std::make_unique<SaveSolver>(file_queue, finish);
    auto autoSavingSolver2 = std::make_unique<SaveSolver>(file_queue, finish);
    auto autoPrintSolver = std::make_unique<PrintSolver>(log_queue, finish);
    solvers.emplace_back(std::move(autoSavingSolver1));
    solvers.emplace_back(std::move(autoSavingSolver2));
    solvers.emplace_back(std::move(autoPrintSolver));

    std::thread file1(std::ref(*solvers[0])),
            file2(std::ref(*solvers[1])),
            log(std::ref(*solvers[2]));

    threads.emplace_back(std::move(file1));
    threads.emplace_back(std::move(file2));
    threads.emplace_back(std::move(log));
}

CommandsStorage::~CommandsStorage()
{
    forcing_push();
    finish.store(true);

    for(auto& thread : threads)
        thread.join();
}

void CommandsStorage::addString(const std::string& str)
{
    if(str == "{" || str == "}")
        addBracket(str);
    else
        addCommand(str);
}

void CommandsStorage::addCommand(const std::string& command)
{
    if(commandsVector.empty())
        firstBulkTime = std::chrono::system_clock::now();

    commandsVector.push_back(command);
    if((commandsVector.size() >= bulkSize_) && bracketStack.empty())
    {
        queues_push();
    }
}

void CommandsStorage::queues_push()
{
    auto&& commandString = bulkCommandString();
    file_queue.push(std::pair<std::string, std::chrono::system_clock::time_point>(commandString, firstBulkTime));
    log_queue.push(std::move(commandString));
    commandsVector.clear();
}

void CommandsStorage::forcing_push()
{
    if(bracketStack.empty() && commandsVector.size())
    {
        queues_push();
    }
}

void CommandsStorage::addBracket(const std::string& bracket)
{
    if(bracket == "{")
    {
        forcing_push();
        bracketStack.push(bracket);
    }
    else if(bracket == "}")
    {
        if(!bracketStack.empty())
            if(bracketStack.top() == "{")
                bracketStack.pop();

        forcing_push();
    }
}

std::string CommandsStorage::bulkCommandString() const
{
    std::stringstream ss;
    if(!commandsVector.empty())
    {
        ss << "bulk: ";
        auto last = commandsVector.end();
        last--;
        for(auto it = commandsVector.begin(); it != last; it++)
        {
            ss << *it << ", ";
        }
        ss << *last;
    }
    return ss.str();
}
