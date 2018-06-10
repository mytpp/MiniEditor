#ifndef DISPLAYVISITOR_H
#define DISPLAYVISITOR_H

#include "abstractvisitor.h"
#include <QString>

class TextFile;

//this Class is to display a existing file when it's first loaded
class DisplayVisitor: public AbstractVisitor
{
public:
    DisplayVisitor(TextFile *src);

    virtual bool visit(QChar& element) override;

private:
    //use this to emit insert signal
    TextFile *background;

    QString line;
};

#endif // DISPLAYVISITOR_H
