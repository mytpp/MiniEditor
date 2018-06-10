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
}

TextFile::TextFile(QUrl address)
    : isModified(false),
      url(address),
      name(url.fileName()),
      text(new TextStructure),
      file(address.url().toLocal8Bit().data()+8),
      searchVisitor(),
      currentSearchResult(),
      historyList(),
      nextCommand(historyList.end())
{
    if(file) {
        //read the file line by line
        std::string line;
        QString line_16bit;
        for (int i=0; std::getline(file, line); i++) {
            line_16bit = QString::fromLocal8Bit(line.c_str());
            if(!file.eof())
                line_16bit.append('\n');
            text->insert({i,0}, line_16bit);
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
}

TextFile::~TextFile() {
    emit fileDestroyed();
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
    file.open(path.url().toLocal8Bit().data()+8, std::ios::out);
    if (file) {
        SaveVisitor save(file);
        bool success = text->traverse(save);
        file.close();
        if (success) {
            url = path;
            name = url.fileName();
        }
        isModified = false;
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
                                     tr("File \"") + name + tr("\" has been modified. Save changes?"),
                                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (pressed == QMessageBox::Save)
            return save();
        else if (pressed == QMessageBox::Cancel)
            return false;  //cancel means regret closing, so "can't close"
        return true;       //refuse to save (Discard)
    } else {
        return true;
    }
}

const QString TextFile::fileName() const
{
    return name;
}

void TextFile::addCommand(std::unique_ptr<EditCommand> &&command)
{
    historyList.erase(nextCommand, historyList.end());
    historyList.push_back(std::move(command));
    nextCommand = historyList.end();
    isModified = true;

    /*cut the history list size to 50*/
    if(historyList.size() > 100)
        for(auto i=0; i < 50; i++)
            historyList.pop_front();
}

void TextFile::highlightAll(int length)
{
    auto& result = searchVisitor->getResult();
    for(const auto &e: result)
        emit highlight(e.first, e.second, length);
}

bool TextFile::search(QString format, Qt::CaseSensitivity cs)
{
    searchVisitor = std::make_unique<SearchVisitor>(format,cs);
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
    emit highlightCurrent(
                currentSearchResult->first,
                currentSearchResult->second,
                searchVisitor->getFormat().size()
    );
}

void TextFile::showNext()
{
    if(searchVisitor->noResult())
        return;
    if(currentSearchResult == --searchVisitor->getResult().cend())
        currentSearchResult = searchVisitor->getResult().begin();
    else
        ++currentSearchResult;
    emit highlightCurrent(
                currentSearchResult->first,
                currentSearchResult->second,
                searchVisitor->getFormat().size()
    );
}

void TextFile::replaceAll(QString newString)
{
    if(searchVisitor->noResult() || searchVisitor->getFormat() == newString)
        return;
    auto replaceCommand = std::make_unique<ReplaceCommand>(
                    std::make_unique<SearchVisitor>(*searchVisitor), //synthesized copy constructor will work
                    newString, text, this);
    (*replaceCommand)();
    addCommand(std::move(replaceCommand));

    (searchVisitor->getResult()).clear();
}

void TextFile::replaceCurrent(QString newString)
{
    if(searchVisitor->noResult() || searchVisitor->getFormat() == newString)
        return;
    int row = currentSearchResult->first;
    int column = currentSearchResult->second;
    int length = (searchVisitor->getFormat()).size();
    auto &result = searchVisitor->getResult();

    erase(row, column, row, column+length);
    insert(row, column, newString);

    //erase the replaced item from search result
    auto iter = result.erase(currentSearchResult);
    if(!result.empty())
        if(iter == result.end())
            currentSearchResult = result.begin();
        else
            currentSearchResult = iter;

    //adjust the positions in search result which is in the same line as 'currentSearchResult'
    while(iter != result.end() && iter->first == row){
        iter->second += newString.size() - length;
        iter++;
    }
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
    auto insertCommand = std::make_unique<InsertCommand>(
               std::make_pair(row, column), character, text, this);
    (*insertCommand)();
    addCommand(std::move(insertCommand));
}

void TextFile::insert(int row, int column, QString newString)
{
    auto insertCommand = std::make_unique<InsertCommand>(
               std::make_pair(row, column), newString, text, this);
    (*insertCommand)();
    addCommand(std::move(insertCommand));
}

void TextFile::erase(int row, int column)
{
    auto eraseCommand = std::make_unique<EraseCommand>(
                std::make_pair(row, column), text, this);
    (*eraseCommand)();
    addCommand(std::move(eraseCommand));
}

void TextFile::erase(int rowBegin, int colBegin, int rowEnd, int colEnd)
{
    auto eraseCommand = std::make_unique<EraseCommand>(
                std::make_pair(rowBegin, colBegin),
                std::make_pair(rowEnd, colEnd),
                text, this);
    (*eraseCommand)();
    addCommand(std::move(eraseCommand));
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
