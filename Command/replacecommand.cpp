#include "replacecommand.h"
#include "Visitor/searchvisitor.h"
#include "textfile.h"

ReplaceCommand::ReplaceCommand(std::shared_ptr<SearchVisitor> RepVtr, QString fmt, QString str,
                               std::shared_ptr<TextStructure> rec, TextFile *inv)
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

