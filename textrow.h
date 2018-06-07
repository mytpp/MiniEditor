#ifndef TEXTROW_H
#define TEXTROW_H

#include <vector>
#include <QString>
#include<QDebug>

class AbstractVisitor;

class TextRow
{
public:
    TextRow();
    //this constructor may be called when a file is being loaded
    TextRow(QString text);

    QChar operator[](int position)const;

    int size() const;
    int capacity() const;  //only for debug

    //make sure the capacity of row is a multiple of 50
    //consider its member function vector::reserve()
    bool insert(int position, QChar cha);
    bool insert(int position, QString str);

    bool erase(int position);
    bool erase(int begin, int end);

    bool traverse(AbstractVisitor &visitor);

private:
    std::vector<QChar> row;
};

#endif // TEXTROW_H
