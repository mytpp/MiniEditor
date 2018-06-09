# DisplayVisitor Class

| 头文件 | #include "displayvisitor.h" |
|-|-|
| 继承 | AbstractVisitor |
| 子类 | none |

## Private Properties
| 类型 | 标识符 |
|-|-|
TextFile *|background
QString|line

## Public Functions
| 类型 |声明|
|-|-|
|| DisplayVisitor()
virtual bool|visit(QChar& element) = 0

## Property Documentation
**`background`**  
借此发出将底层数据结构中内容展示在UI上的信号。  

**`line`**  
每累积一行，放到`line`里，在用一个信号发出去通知界面插入了新字符。  


## Member Function Documentation
**`virtual bool visit(QChar& element) override`**  
将访问的字符放到`line`里，每次遇到换行符，就一次性发信号在UI上添加一行。
