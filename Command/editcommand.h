#ifndef EDITCOMMAND_H
#define EDITCOMMAND_H

#include <utility>
#include <QString>
#include <memory>

class TextFile;
class TextStructure;

class EditCommand
{
public:
    EditCommand() {}
    virtual ~EditCommand() {}

    virtual void operator()() = 0;
    virtual void undo()       = 0;
    virtual void redo()       = 0;

};

#endif // EDITCOMMAND_H
