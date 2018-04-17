#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "application.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQmlContext *root=engine.rootContext();
    Application myapp;
    root->setContextProperty("app", &myapp);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
