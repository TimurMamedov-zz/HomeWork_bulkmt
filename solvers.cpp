#include "solvers.h"
#include "commands_storage.h"

SaveSolver::SaveSolver(CommandsStorage &commStor)
    :Solver(commStor)
{
}

ForcingSaveSolver::ForcingSaveSolver(CommandsStorage &commStor)
    :Solver(commStor)
{
}

void SaveSolver::operator()()
{
    while(true)
        if((commandStorage.commandsSize() >= commandStorage.bulkSize()) && !commandStorage.bracketSize())
        {
            Solver::print();
        }
}

void ForcingSaveSolver::operator()()
{
    if(!commandStorage.bracketSize() && commandStorage.commandsSize())
    {
        Solver::print();
    }
}
