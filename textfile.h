#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <Qt>
#include <QUrl>
#include <QObject>
#include <fstream>
#include <memory>
#include "Command/editcommand.h"
#include <QDebug>

class TextStructure;

class TextFile: public QObject
{
    Q_OBJECT
public:
    TextFile();                //unstored file

    TextFile(const TextFile &);//requisite for QML loading

    //construct the TextStructure according to the file been read
    //and display the file via QML
    TextFile(QUrl address);                                                  //use DisplayVisitor inside

    void display();

    bool save();
    bool saveAs();
    bool canClose();

    Q_INVOKABLE void cut(int rowBegin, int colBegin, int rowEnd, int colEnd);//use EraseCommand inside
    Q_INVOKABLE void copy(int rowBegin, int colBegin, int rowEnd, int colEnd);
    Q_INVOKABLE void paste(int row, int column);                             //use InsertCommand inside

    //emit highlight() signal in the two functions
    Q_INVOKABLE void search(QString format, Qt::CaseSensitivity = Qt::CaseSensitive);//use SearchVisitor inside
    //use a SearchVisitor to traverse 'text' and pass the visitor's reference to ReplaceCommand
    //the erase & insert signal are emitted inside EditCommand, while highlight() signal is emited ouside the visitor
    //because the former has a influence on undo(), the latter is just for UI
    Q_INVOKABLE void replace(QString format, QString newString, Qt::CaseSensitivity = Qt::CaseSensitive);//use ReplaceCommand & SearchVisitor inside

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

    void loaded();  //emitted in constructors
    /*signal destroyed is provided by QObject*/

    void insertCha(int row, int column, QChar cha);
    //void insertStr(int row, int column, QString str);  //seems useless
    void newLine(int row, int column);  //break line at the specified position

    void eraseStr(int row, int column, int length = 1);
    void eraseLine(int row);//'row' should be greater than 0, the first row is never to be erased
                            //merge line 'row' with the previous line or erase an empty line

    void highlight(int row, int column, int length = 1);//for search result, maybe in yellow?
    //highlighting selected string (maybe in blue?) should be done just in QML, i.e. UI level.

private:
    bool saveFile(QUrl path);
    void addCommand(std::shared_ptr<EditCommand> command);

private:
    bool isModified; //check if the file needs saving when closing it
    QUrl url;        //url==QUrl() if the flie has been stored in hard drive
    std::shared_ptr<TextStructure> text;
    std::fstream file;
    std::list<std::shared_ptr<EditCommand>> historyList;
    std::list<std::shared_ptr<EditCommand>>::iterator nextCommand;
};

#endif // TEXTFILE_H
