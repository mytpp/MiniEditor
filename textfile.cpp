#include <string>
#include <utility>
#include <QFileDialog>
#include <QMessageBox>
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
        DisplayVisitor display(this);
        text->traverse(display);
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
    file.open(url.url().toLocal8Bit().data());
    if (file) {
        SaveVisitor save(file);
        bool success = text->traverse(save);
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


void TextFile::search(QString format, Qt::CaseSensitivity cs)
{
    SearchVisitor searchVisitor(format,cs);
    text->traverse(searchVisitor);
    std::vector<std::pair<int,int>> result = searchVisitor.getResult();
    for(auto e: result) {
        highlight(e.first, e.second, format.size());
    }
}

void TextFile::replace(QString format, QString newString)
{

}

void TextFile::cut(int rowBegin, int colBegin, int rowEnd, int colEnd)
{

}

void TextFile::copy(int rowBegin, int colBegin, int rowEnd, int colEnd)
{

}

void TextFile::paste(int row, int column, QString newString)
{

}

void TextFile::insert(int row, int column, QChar character)
{

}

void TextFile::insert(int row, int column, QString newString)
{

}

void TextFile::erase(int row, int column)
{

}

void TextFile::erase(int rowBegin, int colBegin, int rowEnd, int colEnd)
{

}

void TextFile::undo()
{

}

void TextFile::redo()
{

}
