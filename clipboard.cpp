#include "clipboard.h"

ClipBoard::ClipBoard()
{

}

ClipBoard& ClipBoard::instance()
{
    static ClipBoard _instance;
    return _instance;
}

//const QString& ClipBoard::getContent()
//{
//    return content;
//}

//void ClipBoard::setContent(QString str)
//{
//    content = str;
//}
