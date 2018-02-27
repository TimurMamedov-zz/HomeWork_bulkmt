#include "solvers.h"
#include "commands_storage.h"
#include <fstream>

void Solver::print()
{
    auto&& commandString = commandStorage.bulkCommandString();;
    std::ofstream file;
    std::stringstream ss;
    ss << std::chrono::system_clock::to_time_t(commandStorage.getFirstBulkTime());
    file.open(std::string("bulk") + ss.str() + ".log");
    file << commandString;
    file.close();
    std::cout << commandString << std::endl;

    commandStorage.clearCommandsVector();
}
