#include "textfile.h"
#include "clipboard.h"
#include "textstructure.h"
#include "Command/editcommand.h"
#include "Visitor/abstractvisitor.h"


TextFile::TextFile()
{
    qDebug()<<"Empty TextFile Construted";
}

TextFile::TextFile(QUrl address)
{
    qDebug()<<address;
}

TextFile::TextFile(const TextFile & afile)
{
    isModified = afile.isModified;
    url = afile.url;
    qDebug()<<"TextFile Copy Constructed";
}

void TextFile::saveFile(QUrl address)
{

}

void TextFile::close()
{

}


void TextFile::search(QString format)
{

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
