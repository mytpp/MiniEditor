#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFontDatabase>
#include "application.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQmlContext *root=engine.rootContext();
    Application myapp;
    root->setContextProperty("app", &myapp);

    /*Load Fonts*/
    int FontDatabase = QFontDatabase::addApplicationFont(":/assets/fonts/fontawesome-webfont.ttf");
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(FontDatabase);
    if(!loadedFontFamilies.isEmpty()) qDebug() << loadedFontFamilies.at(0) << "\n";

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
