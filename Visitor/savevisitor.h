#ifndef SAVEVISITOR_H
#define SAVEVISITOR_H

#include <fstream>
#include "Visitor/abstractvisitor.h"

//this class is created to save a existing TextStructure into a file
class SaveVisitor: public AbstractVisitor
{
public:
    SaveVisitor(std::fstream &dest);

    //line-breaking character needs treating particularly
    virtual bool visit(QChar& element) override;

private:
    std::fstream &to;
};

#endif // SAVEVISITOR_H
