#include "insertcommand.h"

InsertCommand::InsertCommand(std::pair<int,int> pos, QChar cha, TextFile *inv)
{
    token=CHA;
}

void InsertCommand::operator ()()
{

}

void InsertCommand::undo()
{

}

void InsertCommand::redo()
{

}


TextFile *InsertCommand::invoker()
{
    return _invoker;
}
