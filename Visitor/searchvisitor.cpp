#include "searchvisitor.h"

SearchVisitor::SearchVisitor(QString format, Qt::CaseSensitivity cs)
    :target(format), sensitivity(cs), next(new int [format.size()])
{

}
SearchVisitor::~SearchVisitor()
{
    delete [] next;
}

bool SearchVisitor::visit(QChar element)
{
    return false;
}

 std::vector<std::pair<int,int>> SearchVisitor::getResult()
{
    return result;
}

