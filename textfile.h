#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <QObject>
#include <fstream>
#include <memory>
#include <QUrl>
#include <QDebug>

class TextStructure;
class EditCommand;

class TextFile: public QObject
{
    Q_OBJECT
public:
    TextFile();                //unstored file

    TextFile(const TextFile &);//requisite for QML loading

    //construct the TextStructure according to the file been read
    //and display the file via QML
    TextFile(QUrl address);                                                  //use DisplayVisitor inside

    Q_INVOKABLE void saveFile(QUrl address);
    Q_INVOKABLE void close();

    Q_INVOKABLE void cut(int rowBegin, int colBegin, int rowEnd, int colEnd);//use EraseCommand inside
    Q_INVOKABLE void copy(int rowBegin, int colBegin, int rowEnd, int colEnd);
    Q_INVOKABLE void paste(int row, int column, QString newString);          //use InsertCommand inside

    //emit highlight() signal in the two functions
    Q_INVOKABLE void search(QString format);                                 //use SearchVisitor inside
    //use a SearchVisitor to traverse 'text' and pass the visitor's reference to ReplaceCommand
    //the erase & insert signal are emitted inside EditCommand, while highlight() signal is emited ouside the visitor
    //because the former has a influence on undo(), the latter is just for UI
    Q_INVOKABLE void replace(QString format, QString newString);             //use ReplaceCommand & SearchVisitor inside

    Q_INVOKABLE void insert(int row, int column, QChar character);           //use InsertCommand inside
    Q_INVOKABLE void insert(int row, int column, QString newString);         //use InsertCommand inside
    Q_INVOKABLE void erase(int row, int column);                             //use EraseCommand inside
    Q_INVOKABLE void erase(int rowBegin, int colBegin, int rowEnd, int colEnd);//use EraseCommand inside

    Q_INVOKABLE void undo();
    Q_INVOKABLE void redo();//a littke hard to implement

    Q_INVOKABLE void test() {
        testSlot(5);
        qDebug()<<"send Signal testSlot successfully";
    }

signals:
    void testSlot(int event);

    void insertCha(int row, int column, QChar cha);
    //void insertStr(int row, int column, QString str);  //seems useless
    void newLine(int row, int column);  //break line at the specified position
    void eraseCha(int row, int column);
    void eraseStr(int rowBegin, int colBegin, int rowEnd, int colEnd);
    void eraseLine(int row);//'row' should be greater than 0, the first row is never to be erased
                            //merge line 'row' with the previous line or erase an empty line
    void highlight(int rowBegin, int colBegin, int rowEnd, int colEnd);//for search result, maybe in yellow?
    //highlighting selected string (maybe in blue?) should be done just in QML, i.e. UI level.

private:
    bool isModified;
    QUrl url;
    std::shared_ptr<TextStructure> text;
    std::fstream file;
    std::list<EditCommand*> historyList;
};

#endif // TEXTFILE_H