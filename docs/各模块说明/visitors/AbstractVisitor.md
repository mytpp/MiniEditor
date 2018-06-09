# AbstractVisitor class

| 头文件 |#include "abstractvisitor.h" |
|-|-|
| 继承 | None |
| 子类 | DisplayVisitor，SaveVisitor，SearchVisitor |

## Public Functions
| 类型 |声明|
|-|-|
||AbstractVisitor()
||~AbstractVisitor()
virtual bool|visit(QChar& element) = 0

## Description
用于访问底层数据结构的visitor们的接口类，提供通用的`visit(QChar& element)`函数。
