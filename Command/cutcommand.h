#ifndef CUTCOMMAND_H
#define CUTCOMMAND_H

#include "erasecommand.h"

class CutCommand: public EraseCommand
{
public:
    CutCommand(std::pair<int,int> begin, std::pair<int,int> end, TextFile *inv = nullptr);

private:
    //Template Method
    virtual void doCut() override;

    //all requisite data are stored in base class scope
};

#endif // CUTCOMMAND_H
