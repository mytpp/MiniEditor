#include <string>
#include <utility>
#include <QFileDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QGuiApplication>
#include "textfile.h"
#include "textstructure.h"
#include "Visitor/displayvisitor.h"
#include "Visitor/savevisitor.h"
#include "Visitor/searchvisitor.h"
#include "Command/erasecommand.h"
#include "Command/insertcommand.h"
#include "Command/replacecommand.h"


TextFile::TextFile()
    : isModified(false),
      url(),
      name("Untitled"),
      text(new TextStructure),
      file(),
      searchVisitor(),
      currentSearchResult(),
      historyList(),
      nextCommand(historyList.end())
{
    text->insert({0,0}, QChar('\n'));
    qDebug()<<"Empty TextFile Construted";
    //qDebug()<<QUrl("file:file.txt").url();
}

TextFile::TextFile(QUrl address)
    : isModified(false),
      url(address),
      name(url.fileName()),
      text(new TextStructure),
      file(address.url().toLocal8Bit().data()),
      searchVisitor(),
      currentSearchResult(),
      historyList(),
      nextCommand(historyList.end())
{
    qDebug()<<address;
    if(file) {
        std::string line;
        QString line_16bit;
        for (int i=0; std::getline(file, line); i++) {
            line_16bit = QString::fromLocal8Bit(line.c_str());
            text->insert({i,0}, QChar('\n'));
            text->insert({i,1}, line_16bit);
        }
        file.close();
    } else {
        QMessageBox::warning(nullptr, tr("Warning!"),
                             tr("Couldn't open file:")+address.url(), QMessageBox::Ok);
    }
}

TextFile::TextFile(const TextFile &afile)
    :isModified(afile.isModified),
      url(afile.url),
      name(afile.name),
      text(afile.text),
      file(),
      searchVisitor(),
      currentSearchResult(),
      historyList(),
      nextCommand(historyList.end())
{
    qDebug()<<"TextFile Copy Constructed";
}

void TextFile::display()
{
    DisplayVisitor display(this);
    text->traverse(display);
}

bool TextFile::save()
{
    if(url.isEmpty()) //if the file has not been stored, call saveAs()
        return saveAs();
    else
        return saveFile(url);
}

bool TextFile::saveAs()
{
    //can't assign the return value directly to 'url' in case the return value is an empty QUrl
    QUrl path = QFileDialog::getSaveFileUrl(nullptr, tr("Save As..."), url, tr("文本文档 (*.txt)"));
    if(path.isEmpty())
        return false;
    else
        return saveFile(path);
}

bool TextFile::saveFile(QUrl path)
{
    file.open(path.url().toLocal8Bit().data(), std::ios::out);
    if (file) {
        SaveVisitor save(file);
        bool success = text->traverse(save);
        file.close();
        if (success) {
            url = path;
            name = url.fileName();
        }
        return success;
    } else {
        QMessageBox::warning(nullptr, tr("Warning!"), tr("Couldn't find path:")+url.url(), QMessageBox::Ok);
        return false;
    }
}

bool TextFile::canClose()
{
    if(isModified) {
        QMessageBox::StandardButton pressed =
                QMessageBox::warning(nullptr, tr("MiniWord"),
                                     tr("The file has been modified. Save changes?"),
                                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (pressed = QMessageBox::Save)
            return save();
        else if (pressed = QMessageBox::Cancel)
            return false;
        return true;     //refuse to save (Discard)
    } else {
        return true;
    }
}

const QString TextFile::fileName() const
{
    return name;
}

void TextFile::addCommand(std::shared_ptr<EditCommand> command)
{
    historyList.erase(nextCommand, historyList.end());
    historyList.push_back(command);
    nextCommand = historyList.end();
    isModified = true;
}

void TextFile::highlightAll(int length)
{
    std::vector<std::pair<int,int>>& result = searchVisitor->getResult();
    for(const auto &e: result) {
        emit highlight(e.first, e.second, length);
    }
}

bool TextFile::search(QString format, Qt::CaseSensitivity cs)
{
    searchVisitor = std::make_shared<SearchVisitor>(format,cs);
    text->traverse(*searchVisitor);
    if(searchVisitor->noResult())
        return false;
    highlightAll(format.size());
    currentSearchResult = searchVisitor->getResult().begin();
    emit highlightCurrent(currentSearchResult->first, currentSearchResult->second, format.size());
    return true;
}

void TextFile::showPrevious()
{
    if(searchVisitor->noResult())
        return;
    if(currentSearchResult == searchVisitor->getResult().cbegin())
        currentSearchResult = searchVisitor->getResult().end();
    --currentSearchResult;
    emit highlightCurrent(currentSearchResult->first,
                       currentSearchResult->second,
                       searchVisitor->getFormat().size());
}

void TextFile::showNext()
{
    if(searchVisitor->noResult())
        return;
    if(currentSearchResult == --searchVisitor->getResult().cend())
        currentSearchResult = searchVisitor->getResult().begin();
    else
        ++currentSearchResult;
    emit highlightCurrent(currentSearchResult->first,
                       currentSearchResult->second,
                       searchVisitor->getFormat().size());
}

void TextFile::replaceAll(QString newString)
{
    //search(format, cs);   //hope user click "search" button manually
    std::shared_ptr<SearchVisitor> searchInfo =
            std::make_shared<SearchVisitor>(*searchVisitor);//synthesized copy constructor will work
    std::shared_ptr<ReplaceCommand> replaceCommand(
                new ReplaceCommand(searchInfo, newString, text, this));
    (*replaceCommand)();
    addCommand(replaceCommand);
    highlightAll(newString.size());
    (searchVisitor->getResult()).clear();
}

void TextFile::replaceCurrent(QString newString)
{
    if(searchVisitor->noResult() || searchVisitor->getFormat() == newString)
        return;
    int row = currentSearchResult->first;
    int column = currentSearchResult->second;
    int length = (searchVisitor->getFormat()).size();
    eraseStr(row, column, row, column+length);
    insert(row, column, newString);
    //erase the replaced item from search result
    (searchVisitor->getResult()).erase(currentSearchResult);
    emit highlightCurrent(row, column, newString.size());
}

void TextFile::cut(int rowBegin, int colBegin, int rowEnd, int colEnd)
{
    copy(rowBegin, colBegin, rowEnd, colEnd);
    erase(rowBegin, colBegin, rowEnd, colEnd);
}

void TextFile::copy(int rowBegin, int colBegin, int rowEnd, int colEnd)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString content = text->data({rowBegin, colBegin}, {rowEnd, colEnd});
    clipboard->setText(content);
}

void TextFile::paste(int row, int column)
{
    QString content = QGuiApplication::clipboard()->text();
    insert(row, column, content);
}

void TextFile::insert(int row, int column, QChar character)
{
    std::shared_ptr<InsertCommand> insertCommand(
                new InsertCommand({row, column}, character, text, this));
    (*insertCommand)();
    addCommand(insertCommand);
}

void TextFile::insert(int row, int column, QString newString)
{
    std::shared_ptr<InsertCommand> insertCommand(
                new InsertCommand({row, column}, newString, text, this));
    (*insertCommand)();
    addCommand(insertCommand);
}

void TextFile::erase(int row, int column)
{
    std::shared_ptr<EraseCommand> eraseCommand(
                new EraseCommand({row, column}, text, this));
    (*eraseCommand)();
    addCommand(eraseCommand);
}

void TextFile::erase(int rowBegin, int colBegin, int rowEnd, int colEnd)
{
    std::shared_ptr<EraseCommand> eraseCommand(
                new EraseCommand({rowBegin, colBegin}, {rowEnd, colEnd}, text, this));
    (*eraseCommand)();
    addCommand(eraseCommand);
}

void TextFile::undo()
{
    if(nextCommand != historyList.begin()) {
        --nextCommand;
        (*nextCommand)->undo();
    }
}

void TextFile::redo()
{
    if(nextCommand != historyList.end()) {
        (*nextCommand)->redo();
        ++nextCommand;
    }
}
