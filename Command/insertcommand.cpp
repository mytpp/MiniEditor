#include "insertcommand.h"
#include "textfile.h"

InsertCommand::InsertCommand(std::pair<int,int> pos, QChar cha,
                             std::shared_ptr<TextStructure> rec, TextFile *inv)
{
    token=CHA;
}

InsertCommand::InsertCommand(std::pair<int, int> pos, QString newString,
                             std::shared_ptr<TextStructure> rec, TextFile *inv)
{
    token=STR;
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

