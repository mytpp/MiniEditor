# SaveVisitor Class

| 头文件 | #include "savevisitor.h" |
|-|-|
| 继承 | AbstractVisitor |
| 子类 | none |

## Private Properties
| 类型 | 标识符 |
|-|-|
std::fstream &|to
bool |lineBreaking

## Public Functions
| 类型 |声明|
|-|-|
|| SaveVisitor()
virtual bool|visit(QChar& element) = 0

## Property Documentation
**`to`** 
借它来定位保存文件到硬盘上的位置。将访问的字符送到该文件流中。

**`lineBreaking`** 
用于辅助判别是否要向文件流中输入换行符。


## Member Function Documentation
**`virtual bool visit(QChar& element) override`**  
如果上一次读到的字符是换行符，则向文件流中输出一个换行符。  
若读到普通字符，则直接输出到文件中去。

