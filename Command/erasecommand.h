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

    //consists of doCut(); erase(); sendEraseSignal();
    virtual void operator ()() override;
    //consists of insert(); sendInsertSignal();
    virtual void undo() override;
    //consists of doCut(); erase(); sendEraseSignal();
    virtual void redo() override;

private:
    void insert();
    void erase();

    //by EditCommand::invoker()
    void sendInsertSignal();
    void sendEraseSignal();

private:
    enum {CHA, STR} token;
    std::pair<int,int> begin;
    std::pair<int,int> end;
    //before delete sth from the structure, store it for undo()
    //may contain '\n'. caution
    QChar character;
    QString chunk;
    //to emit modification signal through _invoker(a TextFile)
    TextFile *invoker;
    std::shared_ptr<TextStructure> receiver;
};

#endif // ERASECOMMAND_H
