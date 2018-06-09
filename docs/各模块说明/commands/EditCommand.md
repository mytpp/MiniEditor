# EditCommand class

| 头文件 |#include "editcommand.h" |
|-|-|
| 继承 | None |
| 子类 | EraseCommand，InsertCommand，ReplaceCommand |

## Public Functions
| 类型 |声明|
|-|-|
| |EditCommand() |
||~EditCommand()
| virtual void | operator () = 0|
| virtual void | undo() = 0|
| virtual void | redo() = 0|

## Description
用作会修改底层数据结构的操作的接口类，提供通用的函数调用运算符，撤销和重做操作。
