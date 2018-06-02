#include "erasecommand.h"
#include "textfile.h"
#include "textstructure.h"

EraseCommand::EraseCommand(std::pair<int, int> pos,
    std::shared_ptr<TextStructure> rec, TextFile* inv)
    : begin(pos)
    , receiver(rec)
    , invoker(inv)
{
    token = CHA;
    character = receiver->data(begin);
}

EraseCommand::EraseCommand(std::pair<int, int> begin, std::pair<int, int> end,
    std::shared_ptr<TextStructure> rec, TextFile* inv)
    : begin(begin)
    , end(end)
    , receiver(rec)
    , invoker(inv)
{
    token = STR;
    chunk = receiver->data(begin, end);
}

void EraseCommand::operator()()
{
    if (token == CHA) {
        receiver->erase(begin);
        invoker->eraseCha(begin.first, begin.second);
    } else {
        receiver->erase(begin, end);
        invoker->eraseStr(begin.first, end.first, begin.second, end.second);
    }
}

void EraseCommand::undo()
{
    if (token == CHA) {
        receiver->insert(begin, character);
        invoker->insertCha(begin.first, begin.second, character);
    } else {
        receiver->insert(begin, chunk);
        invoker->insertStr(begin.first, begin.second, chunk);
    }
}

void EraseCommand::redo()
{
    operator()();
}
