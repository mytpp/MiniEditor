#include "displayvisitor.h"
#include "textfile.h"

DisplayVisitor::DisplayVisitor(TextFile *src)
    : background(src)
{

}

bool DisplayVisitor::visit(QChar& element)
{
    background->append(element); //emit signal
    return true;
}

