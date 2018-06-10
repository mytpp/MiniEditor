#ifndef SAVEVISITOR_H
#define SAVEVISITOR_H

#include <fstream>
#include "Visitor/abstractvisitor.h"

//this class is created to save a existing TextStructure into a file
class SaveVisitor: public AbstractVisitor
{
public:
    SaveVisitor(std::fstream &dest);

    virtual bool visit(QChar& element) override;

private:
    std::fstream &to;
    bool lineBreaking;
};

#endif // SAVEVISITOR_H
