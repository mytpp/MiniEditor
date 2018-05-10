# TextRow class

| 头文件 | #include "textrow.h" |
|-|-|
| 继承 | none |
| 子类 | none |

## Properties
| 属性 | 类型 | 标识符 |
|-|-|-|
|private| std::vector\<QChar\> |row|

## Public Functions
| 类型 |声明|
|-|-|
| |TextRow()
| |TextRow(QString text)
QChar& | operator[](int position)
bool | insert(int position, QChar cha)
bool | insert(int position, QString str)
bool | erase(int position)
bool | erase(int begin, int end)
bool | traverse(AbstractVisitor &visitor)

## Property Documentation

## Member Function Documentation