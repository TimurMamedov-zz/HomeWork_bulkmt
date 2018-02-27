#include "commands_storage.h"
#include "solvers.h"
#include <thread>

CommandsStorage::CommandsStorage(std::size_t bulkSize) : bulkSize_(bulkSize)
{
    commandsVector.reserve(bulkSize_);
    autoSavingSolver = std::make_unique<SaveSolver>(*this);
    forcingAutoSavingSolver = std::make_unique<ForcingSaveSolver>(*this);

    std::thread file1(std::ref(*autoSavingSolver));
    file1.detach();
}

CommandsStorage::~CommandsStorage()
{
//    forcingAutoSavingSolver->solve();
}

void CommandsStorage::addString(const std::string& str)
{
    if(str == "{" || str == "}")
        addBracket(str);
    else
        addCommand(str);
//    autoSavingSolver->solve();
}

std::size_t CommandsStorage::bracketSize() const { return bracketStack.size(); }
std::size_t CommandsStorage::commandsSize() const { return commandsVector.size(); }
std::size_t CommandsStorage::bulkSize() const { return bulkSize_; }

void CommandsStorage::addCommand(const std::string& command)
{
    if(commandsVector.empty())
        firstBulkTime = std::chrono::system_clock::now();

    commandsVector.push_back(command);
}

void CommandsStorage::addBracket(const std::string& bracket)
{
    if(bracket == "{")
    {
//        forcingAutoSavingSolver->solve();
        bracketStack.push(bracket);
    }
    else if(bracket == "}")
    {
        if(!bracketStack.empty())
            if(bracketStack.top() == "{")
                bracketStack.pop();
//        forcingAutoSavingSolver->solve();
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

const std::chrono::system_clock::time_point &CommandsStorage::getFirstBulkTime() const
{
    return firstBulkTime;
}

void CommandsStorage::clearCommandsVector()
{
    commandsVector.clear();
}
