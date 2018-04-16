#include "cutcommand.h"

CutCommand::CutCommand(std::pair<int,int> begin, std::pair<int,int> end, TextFile *inv)
    :EraseCommand(begin, end)
{

}

void CutCommand::doCut()
{

}

