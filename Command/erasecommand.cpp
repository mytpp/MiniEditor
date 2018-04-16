#include "erasecommand.h"

EraseCommand::EraseCommand(std::pair<int,int> pos, TextFile *inv)
{

}

EraseCommand::EraseCommand(std::pair<int, int> begin, std::pair<int, int> end, TextFile *inv)
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

TextFile *EraseCommand::invoker()
{
    return _invoker;
}
