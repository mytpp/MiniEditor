#include "insertcommand.h"
#include "textfile.h"
#include "textstructure.h"

InsertCommand::InsertCommand(std::pair<int, int> pos, QChar cha,
    std::shared_ptr<TextStructure> rec, TextFile* inv)
    : character(cha)
    , begin(pos)
    , invoker(inv)
    , receiver(rec)
{
    token = CHA; //insert character
    end = std::pair<int, int>(begin.first, begin.second + 1);
}

InsertCommand::InsertCommand(std::pair<int, int> pos, QString newString,
    std::shared_ptr<TextStructure> rec, TextFile* inv)
    : chunk(newString)
    , begin(pos)
    , receiver(rec)
    , invoker(inv)
{
    token = STR;
    int row = pos.first;
    int column = pos.second;
    for(int i = 0; i < newString.length(); i++){
        if(newString[i] != '\n'){
            column++;
        } else {
            row++;
            column = 0;
        }
    }
    end = std::pair<int, int>(row, column);
}

void InsertCommand::operator()()
{
    if (token == CHA) {
        receiver->insert(begin, character);
        invoker->insertCha(begin.first, begin.second, character);
    } else {
        receiver->insert(begin, chunk);
        invoker->insertStr(begin.first, begin.second, chunk);
    }
}

void InsertCommand::undo()
{
    if (token == CHA) {
        receiver->erase(begin);
        invoker->eraseCha(begin.first, begin.second);
    } else {
        receiver->erase(begin, end);
        invoker->eraseStr(begin.first, begin.second, end.first, end.second);
    }
}

void InsertCommand::redo()
{
    operator()();
}
