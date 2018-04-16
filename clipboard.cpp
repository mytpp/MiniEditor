#include "clipboard.h"

ClipBoard::ClipBoard()
{

}


ClipBoard& ClipBoard::instance()
{
    static ClipBoard _instance;
    return _instance;
}

//void ClipBoard::setContent(QString str)
//{
//    content = str;
//}
