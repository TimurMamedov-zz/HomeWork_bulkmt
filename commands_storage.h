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

#include "solver.h"

class CommandsStorage
{
public:
    explicit CommandsStorage(std::size_t bulkSize);
    ~CommandsStorage();

    void addString(const std::string& str);

    void clearCommandsVector();

    std::size_t bracketSize() const;
    std::size_t commandsSize() const;
    std::size_t bulkSize() const;

    std::string bulkCommandString() const;

    const std::chrono::system_clock::time_point& getFirstBulkTime() const;

private:
    std::stack<std::string> bracketStack;
    std::vector<std::string> commandsVector;
    std::chrono::system_clock::time_point firstBulkTime;
    const std::size_t bulkSize_;
    std::shared_ptr<Solver> autoSavingSolver;
    std::shared_ptr<Solver> forcingAutoSavingSolver;

    void addCommand(const std::string& command);
    void addBracket(const std::string& bracket);
};
