# EraseCommand class
文档删除操作

| 头文件 | #include "erasecommand.h" |
|-|-|
| 继承 | EditCommand |
| 子类 | none |

## Properties
| 属性 | 类型 | 标识符 |
|-|-|-|
|private| enum |token|
||std::pair<int,int> | begin
||std::pair<int,int> | end
||QChar | character
||QString | chunk
|| TextFile | *invoker |
|| std::shared_ptr<TextStructure> | receiver |

## Public Functions
| 类型 |声明|
|-|-|
| | InsertCommand() |
| virtual void | operator () override|
| virtual void | undo() override|
| virtual void | redo() override|


## Property Documentation
**`token`** 标识删除的是字符串还是单个字符

**`begin`** 标识删除范围起点位置

**`end`** 标识删除范围终点位置

**`character`** 若删除单个字符，保存该字符，以备撤回用

**`chunk`** 若删除字符串，保存该字符串，以备撤回用

**`invoker`** 用于发送信号的`textfile`指针

**`receiver`** 底层文档结构`TextStructure`指针

## Member Function Documentation
**`InsertCommand(std::pair<int,int> pos, QChar cha,std::shared_ptr<TextStructure> rec, TextFile *inv  = nullptr)`** 构造函数，删除单个字符时使用

**`InsertCommand(std::pair<int,int> pos, QString newString,std::shared_ptr<TextStructure> rec, TextFile *inv  = nullptr)`** 构造函数，删除字符串时使用

**`operator ()()`** 重载函数调用运算符，使对象能直接以函数方式调用，执行删除操作

**`undo ()`** 撤回函数，撤销本条指令对`textfile`所做的更改

**`redo()`** 重做指令，重新执行本条指令的更改

