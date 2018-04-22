#include "displayvisitor.h"

DisplayVisitor::DisplayVisitor(TextFile *src)
    : background(src)
{

}

bool DisplayVisitor::visit(QChar element)
{
    return false;
}

