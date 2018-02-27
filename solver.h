/*!
\file
\brief
*/
#pragma once

class CommandsStorage;

class Solver
{
public:
    explicit Solver(CommandsStorage& commStor) :commandStorage(commStor){}

    virtual void operator()() = 0;
    virtual ~Solver(){}

protected:
    CommandsStorage& commandStorage;
    void print();
};

