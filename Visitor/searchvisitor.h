#ifndef SEARCHVISITOR_H
#define SEARCHVISITOR_H

#include "abstractvisitor.h"
#include <QString>
#include <Qt>
#include <vector>
#include <utility>

class SearchVisitor: public AbstractVisitor
{
public:
    SearchVisitor(QString format, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    virtual ~SearchVisitor();

    //let traverse() take the responsibility to get the position of the matched string
    virtual bool visit(QChar& element) override;

    //ReplaceCommand may use this
    const QString& getFormat() const;
    std::vector<std::pair<int,int>>& getResult();

    bool noResult();

private:
    //decide if a is equal to b accroding to 'cs'
    bool equal(QChar a, QChar b);

private:
    QString target;      //the formatted string to be searched
    Qt::CaseSensitivity sensitivity;
    int *next;           //the 'next' array, remeber to release it
    int index;
    int row_count;       //the total number of rows that heve been traversed
    int column_count;    //the total number of QChars that heve been traversed in a row
    std::vector<std::pair<int,int>> result; //position of the first character of each matched string
};

#endif // SEARCHVISITOR_H


