#include "textstructure.h"
#include "Visitor/abstractvisitor.h"


TextStructure::TextStructure()
{
}

const QString TextStructure::data(std::pair<int, int> begin, std::pair<int, int> end) const
{
    QString result;
    return result;
}

bool TextStructure::insert(std::pair<int, int> position, QChar character)
{
    return true;
}

bool TextStructure::insert(std::pair<int, int> position, QString newString)
{
    return true;
}

bool TextStructure::erase(std::pair<int, int> position)
{
    return true;
}

bool TextStructure::erase(std::pair<int, int> begin, std::pair<int, int> end)
{
    return true;
}


bool TextStructure::traverse(AbstractVisitor &visitor)
{
    return true;
}
