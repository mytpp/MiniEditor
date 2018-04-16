#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QString>

class ClipBoard
{
public:
    ClipBoard(const ClipBoard&) = delete;
    ClipBoard& operator = (const ClipBoard&) = delete;

    static ClipBoard& instance();
    static void setContent(QString str) {
        //may use squeeze() to avoid the capacity is far more than size
        content = str;
    }

protected:
    ClipBoard();

private:
    static QString content;
};

#endif // CLIPBOARD_H
