#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <Qt>
#include <QUrl>
#include <QObject>
#include <fstream>
#include <memory>
#include <utility>
#include <QDebug>

class TextStructure;
class EditCommand;
class SearchVisitor;

class TextFile: public QObject
{
    Q_OBJECT
public:
    TextFile();                //unstored file

    TextFile(const TextFile &);//requisite for std::list

    //construct the TextStructure according to the file been read
    //and display the file via QML
    TextFile(QUrl address);                                                  //use DisplayVisitor inside

    void display();

    bool save();
    bool saveAs();
    bool canClose();

    const QString fileName() const;

    Q_INVOKABLE void cut(int rowBegin, int colBegin, int rowEnd, int colEnd);//use EraseCommand inside
    Q_INVOKABLE void copy(int rowBegin, int colBegin, int rowEnd, int colEnd);
    Q_INVOKABLE void paste(int row, int column);                             //use InsertCommand inside

    //emit highlight() signal in the two functions
    //search() return false if no matched item is found
    Q_INVOKABLE bool search(QString format, Qt::CaseSensitivity = Qt::CaseSensitive);//use SearchVisitor inside
    Q_INVOKABLE void showPrevious();
    Q_INVOKABLE void showNext();
    //use a SearchVisitor to traverse 'text' and pass the visitor's reference to ReplaceCommand
    //the erase & insert signal are emitted inside EditCommand, while highlight() signal is emited ouside the visitor
    //because the former has a influence on undo(), the latter is just for UI
    Q_INVOKABLE void replaceAll(QString newString);          //use ReplaceCommand & SearchVisitor inside
    Q_INVOKABLE void replaceCurrent(QString newString);

    Q_INVOKABLE void insert(int row, int column, QChar character);           //use InsertCommand inside
    Q_INVOKABLE void insert(int row, int column, QString newString);         //use InsertCommand inside
    Q_INVOKABLE void erase(int row, int column);                             //use EraseCommand inside
    Q_INVOKABLE void erase(int rowBegin, int colBegin, int rowEnd, int colEnd);//use EraseCommand inside

    Q_INVOKABLE void undo();
    Q_INVOKABLE void redo();

    Q_INVOKABLE void test() {
        testSlot(5);
        qDebug()<<"send Signal testSlot successfully";
    }

signals:
    void testSlot(int event);

    /*signal destroyed() is provided by QObject*/

    //cha and str may contain '\n', which need special handling in QML
    void insertCha(int row, int column, QChar cha);
    void insertStr(int row, int column, QString str);
    void append(QChar cha); //for loading file (DisplayVisitor)

    void eraseCha(int row, int column);
    //erase content between {rowBegin, colBegin}(included) and {rowEnd, colEnd}(excluded)
    //which may contain '\n'
    void eraseStr(int rowBegin, int columnBegin, int rowEnd, int columnEnd);
    void eraseLine(int row);//'row' should be greater than 0, the first row is never to be erased
                            //merge line 'row' with the previous line or erase an empty line

    void highlight(int row, int column, int length = 1);//for search result, maybe in yellow?
    //highlighting selected string (maybe in blue?) should be done just in QML, i.e. UI level.
    void highlightCurrent(int row, int column, int length = 1);//highlight in different color

private:
    bool saveFile(QUrl path);
    void addCommand(std::shared_ptr<EditCommand> command);
    void highlightAll(int length = 1);

private:
    bool isModified; //check if the file needs saving when closing it
    QUrl url;        //url==QUrl() if the flie has been stored in hard drive
    QString name;
    std::shared_ptr<TextStructure> text;
    std::fstream file;
    std::shared_ptr<SearchVisitor> searchVisitor;
    std::vector<std::pair<int,int>>::iterator currentSearchResult;
    std::list<std::shared_ptr<EditCommand>> historyList;
    std::list<std::shared_ptr<EditCommand>>::iterator nextCommand;
};

#endif // TEXTFILE_H
