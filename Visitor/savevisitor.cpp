#include "savevisitor.h"
#include <QString>

SaveVisitor::SaveVisitor(std::fstream &dest)
    : to(dest),
      lineBreaking(false)
{

}

bool SaveVisitor::visit(QChar& element)
{
    /*delay inserting '\n'*/
    if(lineBreaking)
        to<<"\n";
    if(element != '\n'){
        lineBreaking = false;
        if(to << QString(element).toLocal8Bit().data())
            return true;
        else
            return false;
    } else {
        lineBreaking = true;
    }
    return true;
}
