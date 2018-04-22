#include "savevisitor.h"

SaveVisitor::SaveVisitor(std::fstream &dest)
    : to(dest)
{

}

bool SaveVisitor::visit(QChar element)
{
    return true;
}
