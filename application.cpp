#include "application.h"
#include <QDebug>

Application::Application()
{
    //debug
    openFiles.push_back(TextFile());
}

bool Application::addFile(QUrl address)
{
    openFiles.push_back(TextFile(address));
    return true;
}

QObject* Application::currentFile()
{
    return &openFiles.front();
}

void Application::save()
{

}

void Application::saveAs()
{

}

bool Application::close()
{
    return true;
}

void Application::closeAll()
{
}

void Application::saveAll()
{

}

QChar Application::addChar()
{
    QString a=tr("hh檀");
    qDebug()<<a.size();
    return a[2];
}


