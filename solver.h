/*!
\file
\brief
*/
#pragma once

class Solver
{
public:
    Solver() = default;

    virtual void operator()() = 0;
    virtual ~Solver(){}
};

