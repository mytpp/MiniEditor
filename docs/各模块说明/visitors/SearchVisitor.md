# SearchVisistor class
查找字符串

| 头文件 | #include "abstractvisitor.h" |
|-|-|
| 继承 | none |
| 子类 | none |

## Properties
| 属性 | 类型 | 标识符 |
|-|-|-|
|private| Qstring |target|
||Qt::CaseSensitivity  | sensitivity
||int* | next
||int | index
||int | row_count
||int | row_count 
|| std::vector<std::pair<int,int>> | result |

## Public Functions
| 类型 |声明|
|-|-|
| |  SearchVisitor(QString format, Qt::CaseSensitivity cs = Qt::CaseSensitive) |
| virtual bool  |  visit(QChar& element) override|
|  const QString& | getFormat() const|
|std::vector<std::pair<int,int>>& | getResult()|
|bool |noResult()

## Property Documentation
**`target`** 代表待查找的模式串

**`sensitity`** 标识查找时是否区分大小写

**`next[]`** kmp算法中模式串的next数组

**`indexr`** 标识当前指向的模式串的位置

**`row_count`** 标识当前的行数

**`column_count`** 标识当前的列数

**`result`** 存放所有匹配位置的容器
## Member Function Documentation
**`SearchVisitor(QString format, Qt::CaseSensitivity cs = Qt::CaseSensitive);`** 构造函数，cs标识是否区分大小写，构造format的next数组用于kmp算法，初始化index,row_count,column_count

**`virtual ~SearchVisitor();
`** 析构函数

**`virtual bool visit(QChar& element) override`** 利用kmp算法匹配模式串target和element，并且将匹配成功的位置压入容器result

**`const QString& getFormat() const`** 返回模式串target

**`bool noResult()`** 返回是否在文档中找到与模式串匹配的字符串

**`bool equal(QChar a, QChar b)`** 按照cs的标准，返回a和b是否相等
