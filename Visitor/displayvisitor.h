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

    //line-breaking character needs treating particularly
    virtual bool visit(QChar& element) override;

private:
    //use this to emit insertCha() or newLine() signal
    TextFile *background;
    //since we can't avoid breaking a line into QChars and display them on by one,
    //we'd better do this in C++， not in QML
    QString line;
};

#endif // DISPLAYVISITOR_H
