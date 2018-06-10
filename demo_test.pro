QT += quick widgets
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    textstructure.cpp \
    textrow.cpp \
    textfile.cpp \
    application.cpp \
    Visitor/abstractvisitor.cpp \
    Visitor/displayvisitor.cpp \
    Visitor/searchvisitor.cpp \
    Command/insertcommand.cpp \
    Command/erasecommand.cpp \
    Command/replacecommand.cpp \
    Command/editcommand.cpp \
    Visitor/savevisitor.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    textstructure.h \
    textrow.h \
    textfile.h \
    application.h \
    Visitor/abstractvisitor.h \
    Visitor/displayvisitor.h \
    Visitor/searchvisitor.h \
    Command/insertcommand.h \
    Command/erasecommand.h \
    Command/replacecommand.h \
    Command/editcommand.h \
    Visitor/savevisitor.h

INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.15063.0/ucrt"
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.15063.0/ucrt/x64"
