#ifndef ABSTRACTVISITOR_H
#define ABSTRACTVISITOR_H

#include <QChar>

class AbstractVisitor
{
public:
    AbstractVisitor();
    virtual ~AbstractVisitor();

    virtual bool visit(QChar element) = 0;

};

#endif // ABSTRACTVISITOR_H
