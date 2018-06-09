#include "savevisitor.h"
#include <QString>
#include <QDebug>

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
        if(to << QString(element).toLocal8Bit().data())
            return true;
        else
            return false;
        lineBreaking = false;
    } else {
        lineBreaking = true;
    }
    return true;
}
