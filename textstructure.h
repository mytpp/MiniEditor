#ifndef TEXTSTRUCTURE_H
#define TEXTSTRUCTURE_H

#include <list>
#include <utility>
#include "textrow.h"

class AbstractVisitor;

class TextStructure
{
public:
    TextStructure();

    QChar data(std::pair<int,int> position) const;
    //copy or cut may use this
    //return a string from begin to end (including '\n')
    const QString data(std::pair<int, int> begin, std::pair<int, int> end) const;

    //line-breaking character can be handled by these functions.
    //insert some content before the position point.
    bool insert(std::pair<int,int> position, QChar character);
    //copy a paragraph may use this
    bool insert(std::pair<int,int> position, QString newString);


    bool erase(std::pair<int,int> position);
    //erase content between [begin, end)
    bool erase(std::pair<int,int> begin, std::pair<int,int> end);

    //inner iterator
    bool traverse(AbstractVisitor &visitor);

private:
    //each line end with a '\n'
    //a empty line contain only a '\n'
    std::list<TextRow> text;
};

#endif // TEXTSTRUCTURE_H
