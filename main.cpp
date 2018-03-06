#include <iostream>
#include "commands_storage.h"


int main(int argc, char *argv[])
{
    try
    {
        std::size_t size = 4;
        if(argc > 1)
        {
            std::istringstream ss(argv[1]);
            ss >> size;
        }
        CommandsStorage cmdStorage(size);
        for(std::string line; std::getline(std::cin, line);)
        {
            cmdStorage.addString(line);
        }
        cmdStorage.forcing_push();
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
