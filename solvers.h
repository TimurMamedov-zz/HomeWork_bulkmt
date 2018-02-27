/*!
\file
\brief
*/
#pragma once

#include "solver.h"

class SaveSolver : public Solver
{
public:
    explicit SaveSolver(CommandsStorage& commStor);
    void operator()() override;
};

class ForcingSaveSolver : public Solver
{
public:
    explicit ForcingSaveSolver(CommandsStorage& commStor);
    void operator()() override;
};
