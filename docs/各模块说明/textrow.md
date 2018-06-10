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
int|size()
int|capacity()
bool|insert(int position, QChar cha)
bool | insert(int position, QString str)
bool | erase(int position)
bool | erase(int begin, int end)
bool | traverse(AbstractVisitor &visitor)

## Property Documentation
**`std::vector<QChar> row`** 存入了文档的一行字符的容器
## Member Function Documentation
**`TextRow::TextRow()`** 构造函数，向空行内压入换行符，动态申请50个字符长度的空间

**`TextRow::TextRow(QString text) `** 将字符串text添加到一行里

**`int TextRow::size() const`**  返回该行的字符个数

**`int TextRow::capacity() `** 返回该行的容量

**`QChar TextRow::operator[](int position)const`** 下标运算符重载

**`bool TextRow::insert(int position, QChar cha)`** 向该行的position位置插入字符cha，并返回是否插入成功

**`bool TextRow::insert(int position, QString str)`** 向该行的position位置插入字符串str，并返回是否插入成功

**`bool TextRow::erase(int position) `** 删除该行position位置上的字符，并返回是否删除成功

**`bool TextRow::erase(int begin, int end)`** begin和end直接表示下标 ，从begin开始删除字符串到end的前一位，并返回是否删除成功

**`bool TextRow::traverse(AbstractVisitor &visitor)`** 对该行每个元素用visitor进行遍历
