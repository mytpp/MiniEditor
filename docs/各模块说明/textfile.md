# TextFile class

| 头文件 | #include "textfile.h" |
|-|-|
| 继承 | QObject |
| 子类 | none |

## Properties
| 属性 | 类型 | 标识符 |
|-|-|-|
private | bool | isModified;
private | QUrl | url;
private | QString | name
private | std::shared_ptr\<TextStructure> | text;
private | std::fstream | file;
private | std::list\<EditCommand*> | historyList;
private | std::list\<std::shared_ptr\<EditCommand>>::iterator | nextCommand;

## Public Functions
| 类型 |声明|
|-|-|
| |TextFile()
| |TextFile(const TextFile &)
| |TextFile(QUrl address)
void | display()
bool | save()
bool | saveAs()
bool | canClose()
const QString | fileName() const;
Q_INVOKABLE void | cut(int rowBegin, int colBegin, int rowEnd, int colEnd)
Q_INVOKABLE void | copy(int rowBegin, int colBegin, int rowEnd, int colEnd)
Q_INVOKABLE void | paste(int row, int column)
Q_INVOKABLE void | search(QString format, Qt::CaseSensitivity = Qt::CaseSensitive);
Q_INVOKABLE void | replace(QString format, QString newString, Qt::CaseSensitivity = Qt::CaseSensitive)
Q_INVOKABLE void | insert(int row, int column, QChar character)
Q_INVOKABLE void | insert(int row, int column, QString newString)
Q_INVOKABLE void | erase(int row, int column)
Q_INVOKABLE void | erase(int rowBegin, int colBegin, int rowEnd, int colEnd)
Q_INVOKABLE void | undo()
Q_INVOKABLE void | redo()

## Private Functions
| 类型 |声明|
|-|-|
bool | saveFile(QUrl path)
void | addCommand(std::shared_ptr<EditCommand> command)

## Signals
* insertCha(int row, int column, QChar cha)
* insertStr(int row, int column, QString str)
* append(QChar cha)
* eraseCha(int row, int column)
* eraseStr(int rowBegin, int colBegin, int rowEnd, int colEnd)
* eraseLine(int row)
* highlight(int row, int column, int length = 1)

## Detailed Description
TextFile是衔接QML与C++的重要接口类之一(另一个是Application)，允许底层数据结构、硬盘中的文本文件、展示到UI界面上的编辑界面相互关联起来，扮演着Mediator的角色。  
所有的编辑操作都是在单个文件（TextFile）上进行的，操作在不同文件（TextFile）上的操作是不相干的
#### 初始化一个TextFile  
默认构造函数初始化一个没有与硬盘中的文本文件相关联的TextFile对象，对应“新建文件”的操作。接受一个QUrl的构造函数将硬盘中该QUrl对应的地址上的文本文件读入内存中，并以TextStructure类来维护该结构，这个构造函数对应“打开文件”的操作。  
#### C++与QML数据交互
TextFile提供一系列Q_INVOKABLE函数，通过Qt元对象系统的辅助（TextFile必须继承自QObject），QML中的代码可以调用这些函数来实现QML到C++的数据传递。当用户在UI交互界面进行编辑操作时，产生的事件就是通过Q_INVOKABLE函数传递给C++处理的（在此之前QML会对事件进行一些预处理）。  
在C++处理事件的过程中，会有一系列信号通过TextFile传递出来（TextFile声明了这些信号，由Qt元对象系统提供具体实现）。在QML中会通过Connections对象监听从TextFile传来的信号，并对收到的信号进行处理，这里的“处理”可以理解为把C++代码的执行结果在UI界面上展示出来。
#### 撤销/重做



## Property Documentation

## Member Function Documentation

