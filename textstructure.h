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

    //for every function that need position parameter(s),
    //check if 'row' is a legal position, and let TextRow check if 'column' is legal,
    //and send some message if the position is out of range

    QChar& data(std::pair<int,int> position);  //seems useless
    //copy or cut may use this
    //return a string from begin to end (including '\n')
    const QString data(std::pair<int, int> begin, std::pair<int, int> end) const;

    //line-breaking character can be handled by these functions.
    //insert some content before the position point.
    //better check if the row is the last row (position.first==text.size())
    //if so, use list::back() to locate position may be faster
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
