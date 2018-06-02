#include "replacecommand.h"
#include "Visitor/searchvisitor.h"
#include "textfile.h"
#include "textstructure.h"

ReplaceCommand::ReplaceCommand(std::unique_ptr<SearchVisitor> &&RepVtr, QString str,
                               std::shared_ptr<TextStructure> rec, TextFile *inv)
    :visitor(RepVtr.release()), newString(str), invoker(inv), receiver(rec)
{

}

ReplaceCommand::~ReplaceCommand() = default;

void ReplaceCommand::operator ()()
{
    auto result = visitor->getResult();
    auto format = visitor->getFormat();
    for(auto i = result.crbegin(); i != result.crend(); i++) {
        receiver->erase(*i, {i->first, i->second + format.size()});
        emit invoker->eraseStr(i->first, i->second, i->first, i->second + format.size());
        receiver->insert(*i, newString);
        emit invoker->insertStr(i->first, i->second, newString);
    }
}

void ReplaceCommand::undo()
{
    auto result = visitor->getResult();
    auto format = visitor->getFormat();
    for(auto i = result.cbegin(); i != result.cend(); i++) {
        receiver->erase(*i, {i->first, i->second + newString.size()});
        emit invoker->eraseStr(i->first, i->second, i->first, i->second + newString.size());
        receiver->insert(*i, format);
        emit invoker->insertStr(i->first, i->second, format);
    }
}

void ReplaceCommand::redo()
{
    operator ()();
}

