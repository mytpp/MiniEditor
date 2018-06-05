#ifndef REPLACECOMMAND_H
#define REPLACECOMMAND_H

#include "editcommand.h"


class SearchVisitor;

class ReplaceCommand: public EditCommand
{
public:
    //use shared_ptr to avoid deep copy
    ReplaceCommand(std::unique_ptr<SearchVisitor> &&RepVtr, QString str,
                   std::shared_ptr<TextStructure> rec, TextFile *inv = nullptr);
    ~ReplaceCommand();

    //call TextStructure::traverse() through EditCommand::invoker()
    //scan the text and substitute the searched string from end to beginning,
    //in case that the subsequent coodinates become invalid after erasing the previous string
    virtual void operator ()() override;
    //undo() scans the text from beginning to end
    virtual void undo() override;
    virtual void redo() override;

private:
    //just call getResult() from this visitor
    std::unique_ptr<SearchVisitor> visitor;
    //for undo() and redo()
    QString newString;
    //to emit modification signal through _invoker(a TextFile)
    TextFile *invoker;
    std::shared_ptr<TextStructure> receiver;
};

#endif // REPLACECOMMAND_H
