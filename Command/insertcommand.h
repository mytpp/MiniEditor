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

    //consists of insert(); sendInsertSignal();
    virtual void operator ()() override;
    //consists of erase(); sendEraseSignal();
    virtual void undo() override;
    //consists of insert(); sendInsertSignal();
    virtual void redo() override;

private:
    void insert();
    void erase();

    //by EditCommand::invoker()
    void sendInsertSignal();
    void sendEraseSignal();

private:
    enum {CHA, STR} token;
    //necessary to store two positions?
    //Maybe. Because chunk may contain '\n', which can be a trouble
    std::pair<int,int> begin;
    std::pair<int,int> end;
    //for redo()
    QChar charater;
    QString chunk;
    //to emit modification signal through _invoker(a TextFile)
    TextFile *invoker;
    std::shared_ptr<TextStructure> receiver;
};

#endif // INSERTCOMMAND_H
