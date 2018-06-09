#include "displayvisitor.h"
#include "textfile.h"

DisplayVisitor::DisplayVisitor(TextFile *src)
    : background(src),
      line()
{

}

bool DisplayVisitor::visit(QChar& element)
{
    line.append(element);
    if(element == QChar('\n')) {
        emit background->append(line);
        line.clear();
    }
    return true;
}

