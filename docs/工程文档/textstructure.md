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
| QChar | data(int row, int column)|
|bool |insert(std::pair<int,int> position, QChar character)|
|bool |insert(std::pair<int,int> position, QString newString)|
|bool | push_back(QChar character)|
|bool | pop_back()|
|bool |erase(std::pair<int,int> position)|
|bool |erase(std::pair<int,int> begin, std::pair<int,int> end)|

## Property Documentation
**TextStructure::TextStructure()**
构造函数

**QChar TextStructure::data(int row, int column)**
返回 row column 对应的字符

## Member Function Documentation

