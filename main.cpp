#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "fucklistmodel.h"
#include "application.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    fuckListModel shit;
    QQmlContext *root=engine.rootContext();
    root->setContextProperty("shit",&shit);

    Application myapp;
    root->setContextProperty("app", &myapp);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
