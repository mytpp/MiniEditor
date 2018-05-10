# TextFile class

| 头文件 | #include "textfile.h" |
|-|-|
| 继承 | QObject |
| 子类 | none |

## Properties
| 属性 | 类型 | 标识符 |
|-|-|-|
private|bool | isModified;
private|QUrl | url;
private|std::shared_ptr\<TextStructure> | text;
private|std::fstream | file;
private|std::list\<EditCommand*> | historyList;

## Public Functions
| 类型 |声明|
|-|-|
| |TextFile()
| |TextFile(const TextFile &)
| |TextFile(QUrl address)
Q_INVOKABLE void | saveFile(QUrl address);
Q_INVOKABLE void | close();
Q_INVOKABLE void | cut(int rowBegin, int colBegin, int rowEnd, int colEnd)
Q_INVOKABLE void | copy(int rowBegin, int colBegin, int rowEnd, int colEnd)
Q_INVOKABLE void | paste(int row, int column, QString newString)
Q_INVOKABLE void | search(QString format)
Q_INVOKABLE void | replace(QString format, QString newString)
Q_INVOKABLE void | insert(int row, int column, QChar character)
Q_INVOKABLE void | erase(int rowBegin, int colBegin, int rowEnd, int colEnd)
Q_INVOKABLE void | undo()
Q_INVOKABLE void | redo()
Q_INVOKABLE void | test()

## Property Documentation

## Member Function Documentation