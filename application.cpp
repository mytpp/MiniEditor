#include <iterator>
#include "application.h"
#include <QDebug>

Application::Application()
    :openFiles(),
      current(openFiles.end())
{
    //debug
    //openFiles.push_back(TextFile());
}

void Application::addFile(QUrl address)
{
    if(address.isEmpty())
        openFiles.push_back(TextFile());
    else
        openFiles.push_back(TextFile(address));
    setCurrentFile(--openFiles.end()); //call display()
}

QObject* Application::currentFile()
{
    return &(*current);
}

void Application::setCurrentFile(int index)
{
    auto i = openFiles.begin();
    advance(i, index);//complexity O(n)
    setCurrentFile(i);
}

void Application::setCurrentFile(std::list<TextFile>::iterator index)
{
    current = index;
    emit fileLoaded(current->fileName());
    current->display();
}

void Application::save()
{
    if(current == openFiles.end())
        return;
    current->save();
}

void Application::saveAs()
{
    if(current == openFiles.end())
        return;
    current->saveAs();
}

bool Application::close()
{
    if(current == openFiles.end() || !current->canClose())
        return false;  //useless, whether a file closed successfully is determined by destroy() signal
    auto i = openFiles.erase(current);//QObject emit destroyed signal, which is to be handled by QML
    if(!openFiles.empty()) {
        if(i == openFiles.end())
            setCurrentFile(--i);
        else
            setCurrentFile(i);
    }
    return true;       //useless
}

bool Application::closeAll()
{
    auto iter = openFiles.begin();
    while (iter != openFiles.end()) {
        if(iter->canClose())
            iter = openFiles.erase(iter);
        else
            iter++;
    }
    if(openFiles.empty())
        return true;
    setCurrentFile(openFiles.begin());
    return false;
}

void Application::saveAll()
{
    for(auto &file: openFiles)
        file.save();
}

QChar Application::addChar()
{
    QString a=tr("hh檀");
    qDebug()<<a.size();
    return a[2];
}


