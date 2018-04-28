#include "savevisitor.h"
#include <QString>

SaveVisitor::SaveVisitor(std::fstream &dest)
    : to(dest)
{

}

bool SaveVisitor::visit(QChar& element)
{
    if(to << QString(element).toLocal8Bit().data())
        return true;
    else
        return false;
}
