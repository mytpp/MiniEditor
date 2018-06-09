# ReplaceCommand class

| 头文件 | #include "replacecommand.h" |
|-|-|
| 继承 | EditCommand |
| 子类 | none |


## Private Properties
| 类型 | 标识符 |
|-|-|
std::unique_ptr<SearchVisitor>|visitor
QString|newString
TextFile *| invoker
std::shared_ptr<TextStructure>|receiver


## Public Functions
| 类型 |声明|
|-|-|
| | ReplaceCommand() |
||~ReplaceCommand()
| virtual void | operator () override|
| virtual void | undo() override|
| virtual void | redo() override|


## Property Documentation
**`visitor`**  
一个放有搜索结果和模式串的SearchVisitor的智能指针。

**`newString`**  
用于替换模式串的新字符串。

**`invoker`**  
一个TextFile的指针，借此发出更改UI内容的信号

**`receiver`**  
执行该底层数据结构上的插入删除操作。


## Member Function Documentation  
**`ReplaceCommand(std::unique_ptr<SearchVisitor> &&RepVtr, QString str,std::shared_ptr<TextStructure> rec, TextFile *inv = nullptr)`**  
构造函数。  
`RepVtr`是右值引用，因此可以直接取得其对资源(`SearchVisitor`)的所有权。

**`~ReplaceCommand()`**  
析构函数。  
因为类的定义中包含了不完全类型的`unique_ptr`所以必须在类外定义析构函数。

**`operator ()()`**   
执行替换命令。  
从`visitor`中获取搜索结果和模式串，然后从后往前遍历搜索结果，依次用新串替换匹配的字符串。  

**`undo ()`**   
撤销替换操作。  
从`visitor`中获取搜索结果和模式串，从前往后遍历搜索结果，依次用原模式串替换新串。

**`redo()`**  
执行普通替换命令。  
直接调用`operator ()()`。

