#include "searchvisitor.h"

SearchVisitor::SearchVisitor(QString format, Qt::CaseSensitivity cs)
    :target(format), sensitivity(cs), next(new int [format.size()])
{

}
SearchVisitor::~SearchVisitor()
{
    delete [] next;
}

bool SearchVisitor::visit(QChar& element)
{
    return false;
}

const QString& SearchVisitor::getFormat() const
{
    return target;
}

std::vector<std::pair<int,int>>& SearchVisitor::getResult()
{
    return result;
}

bool SearchVisitor::noResult()
{
    return result.empty();
}

