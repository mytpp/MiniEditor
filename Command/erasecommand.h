#ifndef ERASECOMMAND_H
#define ERASECOMMAND_H

#include "editcommand.h"

class EraseCommand: public EditCommand
{
public:
    EraseCommand(std::pair<int,int> pos,
                 std::shared_ptr<TextStructure> rec, TextFile *inv = nullptr);
    EraseCommand(std::pair<int,int> begin, std::pair<int,int> end,
                 std::shared_ptr<TextStructure> rec, TextFile *inv = nullptr);

    virtual void operator ()() override;
    virtual void undo() override;
    virtual void redo() override;

private:
    enum {CHA, STR} token;
    std::pair<int,int> begin;
    std::pair<int,int> end;

    //before delete sth from the structure, store it for undo()
    //may contain '\n'
    QChar character;
    QString chunk;

    //to emit modification signal through _invoker(a TextFile)
    TextFile *invoker;
    std::shared_ptr<TextStructure> receiver;
};

#endif // ERASECOMMAND_H
