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
        QMessageBox::warning(nullptr, tr("Warning!"), tr("Couldn't open file:")+address.url(), QMessageBox::Ok);
    }
}

TextFile::TextFile(const TextFile & afile)
{
    isModified = afile.isModified;
    url = afile.url;
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
    file.open(path.url().toLocal8Bit().data());
    if (file) {
        SaveVisitor save(file);
        bool success = text->traverse(save);
        file.close();
        if (success)
            url = path;
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
    ++nextCommand;
}

void TextFile::search(QString format, Qt::CaseSensitivity cs)
{
    SearchVisitor searchVisitor(format,cs);
    text->traverse(searchVisitor);
    std::vector<std::pair<int,int>> result = searchVisitor.getResult();
    for(auto e: result) {
        emit highlight(e.first, e.second, format.size());
    }
}

void TextFile::replace(QString format, QString newString, Qt::CaseSensitivity cs)
{
    std::shared_ptr<SearchVisitor> searchVisitor(new SearchVisitor(format,cs));
    text->traverse(*searchVisitor);
    std::vector<std::pair<int,int>> result = searchVisitor->getResult();
    for(auto e: result) {
        highlight(e.first, e.second, format.size());
    }
    std::shared_ptr<ReplaceCommand> replaceCommand(
                new ReplaceCommand(searchVisitor, newString, text, this));
    (*replaceCommand)();
   addCommand(replaceCommand);
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
