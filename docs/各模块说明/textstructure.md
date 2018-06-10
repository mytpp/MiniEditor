# TextStructure class
文档基础数据结构

| 头文件 | #include "textstructure.h" |
|-|-|
| 继承 | none |
| 子类 | none |

## Properties
| 属性 | 类型 | 标识符 |
|-|-|-|
|private| std::list\<TextRow\> |text|

## Public Functions
| 类型 |声明|
|-|-|
| | TextStructure() |
| QChar | data(std::pair<int,int> position)|
| QString  |data(std::pair<int, int> begin, std::pair<int, int> end)|
|bool |insert(std::pair<int,int> position,QChar character)|
|bool |insert(std::pair<int,int> position, QString newString)|
|bool |erase(std::pair<int,int> position)|
|bool |erase(std::pair<int,int> begin, std::pair<int,int> end)|
|bool |traverse(AbstractVisitor &visitor)|

## Property Documentation
**`text`** 存放了TextRow类的list容器，用于存放文档的内容，每个元素表示文档的一行 


## Member Function Documentation
**`QChar data(std::pair<int,int> position) const`** 返回文档在position位置的字符

**`const QString data(std::pair<int, int> begin, std::pair<int, int> end) const`** 返回文档在begin和end之间的字符串


**`bool insert(std::pair<int,int> position, QChar character)`** 向文档position位置插入字符character，并返回是否插入成功

**`bool insert(std::pair<int,int> position, QString newString);`** 向文档position位置插入字符串character，并返回是否插入成功

**`bool erase(std::pair<int,int> position);`** 删除文档position位置的字符，并返回是否删除成功

**`bool erase(std::pair<int,int> begin, std::pair<int,int> end)`** 删除文档在begin和end之间的字符串，并返回是否删除成功 
