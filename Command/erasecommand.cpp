#include "erasecommand.h"
#include "textfile.h"

EraseCommand::EraseCommand(std::pair<int,int> pos,
                           std::shared_ptr<TextStructure> rec, TextFile *inv)
{

}

EraseCommand::EraseCommand(std::pair<int, int> begin, std::pair<int, int> end,
                           std::shared_ptr<TextStructure> rec, TextFile *inv)
{

}

void EraseCommand::operator ()()
{

}

void EraseCommand::undo()
{

}

void EraseCommand::redo()
{

}

