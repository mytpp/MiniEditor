#ifndef INSERTCOMMAND_H
#define INSERTCOMMAND_H

#include "editcommand.h"


class InsertCommand : public EditCommand
{
public:
    InsertCommand(std::pair<int,int> pos, QChar cha,
                  std::shared_ptr<TextStructure> rec, TextFile *inv  = nullptr);
    InsertCommand(std::pair<int,int> pos, QString newString,
                  std::shared_ptr<TextStructure> rec, TextFile *inv  = nullptr);

    virtual void operator ()() override;
    virtual void undo() override;
    virtual void redo() override;

private:
    enum {CHA, STR} token;

    std::pair<int,int> begin;
    std::pair<int,int> end;

    //for redo()
    QChar character;
    QString chunk;

    //to emit modification signal through _invoker(a TextFile)
    TextFile *invoker;
    std::shared_ptr<TextStructure> receiver;
};

#endif // INSERTCOMMAND_H
