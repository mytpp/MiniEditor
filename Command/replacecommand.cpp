#include "replacecommand.h"

ReplaceCommand::ReplaceCommand(std::shared_ptr<SearchVisitor> RepVtr, QString fmt, QString str, TextFile *inv)
{

}

void ReplaceCommand::operator ()()
{

}


void ReplaceCommand::undo()
{

}

void ReplaceCommand::redo()
{

}

TextFile *ReplaceCommand::invoker()
{
    return _invoker;
}
