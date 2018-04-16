#ifndef TEXTSTRUCTURE_H
#define TEXTSTRUCTURE_H

#include "textrow.h"
#include <list>
#include <utility>


class TextStructure
{
public:
    TextStructure();

    QChar data(int row, int column);

    //bool push_back(QChar character);
    //bool push_back(QString newString);

    //line-breaking character can be handled by these functions.
    //insert some content before the position point.
    //better check if the row is the last row (position.first==text.size())
    //if so, use list::back() to locate position may be faster
    bool insert(std::pair<int,int> position, QChar character);
    //copy a paragraph may use this
    bool insert(std::pair<int,int> position, QString newString);
    //used when loading a file
    bool push_back(QChar character);

    bool pop_back();
    bool erase(std::pair<int,int> position);
    bool erase(std::pair<int,int> begin, std::pair<int,int> end);

    //inner iterator
    bool traverse(AbstractVisitor &visitor, std::pair<int,int> begin={0,0});

private:
    //each line begin with a '\n' indicating this line exists
    std::list<TextRow> text;
};

#endif // TEXTSTRUCTURE_H
