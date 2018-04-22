#include <iterator>
#include "application.h"
#include <QDebug>

Application::Application()
    :openFiles(),
      current(openFiles.end())
{
    //debug
    openFiles.push_back(TextFile());
}

void Application::addFile(QUrl address)
{
    if(address.isEmpty())
        openFiles.push_back(TextFile());
    else
        openFiles.push_back(TextFile(address));
}

QObject* Application::currentFile()
{
    return &(*current);
}

void Application::setCurrentFile(int index)
{
    current = openFiles.begin();
    advance(current, index);//complexity O(n)
    current->display();
}

void Application::save()
{
    current->save();
}

void Application::saveAs()
{
    current->saveAs();
}

bool Application::close()
{
    if(!current->canClose())
        return false;
    openFiles.erase(current);//QObject emit destroyed signal, which is to be handled by QML
    return true;
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
    return false;
}

void Application::saveAll()
{
    for(auto file: openFiles)
        file.save();
}

QChar Application::addChar()
{
    QString a=tr("hh檀");
    qDebug()<<a.size();
    return a[2];
}


