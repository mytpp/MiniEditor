# TextFile class

| 头文件 | #include "clipboard.h" |
|-|-|
| 继承 | none |
| 子类 | none |

## Properties
| 属性 | 类型 | 标识符 |
|-|-|-|
|private| static QString | content|

## Public Functions
| 类型 |声明|
|-|-|
| |ClipBoard(const ClipBoard&) = delete
| |ClipBoard& operator = (const ClipBoard&) = delete
static | ClipBoard& instance()
static void | setContent(QString str)

## Protected Functions
| 类型 |声明|
|-|-|
| | ClipBoard() |

## Property Documentation

## Member Function Documentation