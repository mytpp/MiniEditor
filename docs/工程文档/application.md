# Application class

| 头文件 |#include "application.h"|
|-|-|
| 继承 | QObject |
| 子类 | none |

## Properties
| 属性 | 类型 | 标识符 |
|-|-|-|
|private| std::list\<TextFile\> |openFiles|

## Public Functions
| 类型 |声明|
|-|-|
| |Application()
Q_INVOKABLE bool | addFile(QUrl address)
Q_INVOKABLE void | save();
Q_INVOKABLE void | saveAs()
Q_INVOKABLE void | close()
Q_INVOKABLE void | closeAll();
Q_INVOKABLE void | saveAll()
Q_INVOKABLE QObject* | currentFile()
Q_INVOKABLE QChar | addChar();

## Property Documentation

## Member Function Documentation