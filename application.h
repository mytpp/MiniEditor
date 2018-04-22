#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <list>
#include <QUrl>
#include "textfile.h"


class Application: public QObject
{
    Q_OBJECT
public:
    Application();

    //triggered by 'open' button
    Q_INVOKABLE bool addFile(QUrl address = QUrl());

    //the three function is for currentFile
    //when save(As)?() is called, remeber to ask for URL when necessary
    Q_INVOKABLE void save();
    Q_INVOKABLE void saveAs();
    //close current file
    //remeber to check if the file need saving
    Q_INVOKABLE bool close();

    //close all files, triggered by 'quit' button
    Q_INVOKABLE void closeAll();
    Q_INVOKABLE void saveAll();


    //maintain a reference in QML for currentFile, in case of calling this function too frenquently
    Q_INVOKABLE QObject* currentFile();


    //debug
    Q_INVOKABLE QChar addChar();

private:
    //support multi-file edition
    std::list<TextFile> openFiles;
    std::list<TextFile>::iterator current;
};

#endif // APPLICATION_H
