# Application class

| 头文件 |#include "application.h"|
|-|-|
| 继承 | QObject |
| 子类 | none |


## Properties
| 属性 | 类型 | 标识符 |
|-|-|-|
|private| std::list\<TextFile\> |openFiles|
|private|std::list\<TextFile\>::iterator|current|


## Public Functions
| 索引 | 类型 |声明|
|-|-|-|
1 | |Application()
2 | Q_INVOKABLE bool | addFile(QUrl address)
3 | Q_INVOKABLE void | save();
4 | Q_INVOKABLE void | saveAs()
5 | Q_INVOKABLE void | close()
6 | Q_INVOKABLE void | closeAll();
7 | Q_INVOKABLE void | saveAll()
8 | Q_INVOKABLE QObject* | currentFile()
9 | Q_INVOKABLE void | setCurrentFile(int index)


## Private Functions
| 类型 |声明|
|-|-|
void | setCurrentFile(std::list<TextFile>::iterator index)


## Signal
void fileLoaded(QString name)


## Detailed Description
`Application`是衔接QML与C++的重要接口类之一(另一个是TextFile)，该类在全局只有一个实例，并在主函数中被注册到QML中去。它是整个软件的核心部分，由它来负责协调软件中各C++子模块间的工作，并为QML部分提供底层接口，是衔接C++与QML之间的桥梁，扮演着Mediator的角色。菜单栏中文件级别的操作都是通过该类提供的接口执行的。   

该类维护一个`TextFile`的`list`，用来表示目前打开的文件，用以支持多文档编辑。通过`currentFile()`和`setCurrentFile(int index)`两个函数，QML中可以获得表示当前tag对应文件的对象，然后通过该对象进行对单个文件的编辑操作。   

#### C++与QML数据交互
1. QML => C++  
`Application`提供一系列Q_INVOKABLE函数，通过Qt元对象系统的辅助（`Application`必须继承自QObject），QML中的代码可以调用这些函数来实现QML到C++的数据传递。当用户在UI交互界面进行编辑操作时，产生的事件就是通过Q_INVOKABLE函数传递给C++处理的（在此之前QML会对事件进行一些预处理）。   
2. C++ => QML  
在C++处理事件的过程中，会有一系列信号通过`Application`和`TextFile`传递出来（`Application`和`TextFile`声明了这些信号，由Qt元对象系统提供具体实现）。在QML中会通过`Connections`对象监听从这两个类的对象中传来的信号，并对收到的信号进行处理，这里的“处理”可以理解为把C++代码的执行结果在UI界面上展示出来。  

因为在`main()`函数中会直接将一个`Application`对象注册到QML上下文中（在QML中名字为`app`），所以在QML中可以直接建立对该对象的链接，若要监听从`TextFile`传出的信号，需要在QML中通过`app`调用`currentFile()`返回当前文件的指针，从而建立起对`TextFile`对象的链接。因为QML中默认可以识别`QObject`对象（而不能直接识别自定义类），所以让`currentFile()`返回`QObject *`。


## Property Documentation
`std::list<TextFile> openFiles`   
一组在该软件中打开的文件列表。  
在打开文件时，会向该列表中加入新打开的文件，关闭某个文件时，会将该文件从`openFiles`中删除。在没有打开的文件时该列表为空。  

`std::list<TextFile>::iterator current`  
指向`openFiles`中元素的迭代器，用于标志当前正在显示的文件。  
该标志与UI上的tag同步。可以通过`setCurrentFile()`来重置该标志的位置，通过`currentFile()`返回该迭代器指向的`TextFile`对象。针对单个文件的操作几乎都是在`current`指向的文本文件对象上进行的。  


## Member Function Documentation
1. [public] `Application()`  
默认构造函数。  
初始时打开文件的列表为空，标识当前文件的迭代器`current`指向`openFiles`的尾后位置。

2. [public] `Q_INVOKABLE void addFile(QUrl address = QUrl())`  
向打开文件列表`openFiles`中添加一个元素（文件）。  
`address`表示当前文件在硬盘上的地址，默认值为空。  
如果`address`为空，意味着这是新建文件的操作，向`openFiles`中加入一个空的文件（`TextFile`），否则是打开文件操作，通过`address`构造一个`TextFile`并将其放入文件列表。再设置当前文件标志`current`指向新加的文件。  

3. [public] `Q_INVOKABLE void save()`  
对当前文件执行“保存”操作。  
如果当前文件标志`current`有效，则直接调用当前文件的`save()`操作。

4. [public] `Q_INVOKABLE void saveAs()`  
对当前文件执行“另存为”操作。  
如果当前文件标志`current`有效，则直接调用当前文件的`saveAs()`操作。

5. [public] `Q_INVOKABLE bool close()`  
尝试关闭当前文件。  
如果当前文件标志`current`有效且可以被关闭（`current->canClose()`返回`true`），则从打开文件列表`openFiles`中删除当前文件，并重置`current`位置。  
该函数返回值表示是否成功关闭了当前文件。

6. [public] `Q_INVOKABLE bool closeAll()`  
尝试关闭打开文件列表中的所有文件。  
对打开文件列表`openFiles`中每个文件判断是否可以关闭（`canClose()`返回`true`），如果可以关闭则将其从列表中删除，否则跳过该文件（不删除）。如果最终文件列表为空，说明打开的文件全部关闭，如果是“退出”操作调用的该函数，那么此时就可以将该软件关闭了。否则软件不会关闭。

7. [public] `Q_INVOKABLE void saveAll()`  
对打开文件列表`openFiles`中所有的文件进行一次保存（`save()`）操作。

8. [public] `Q_INVOKABLE QObject* currentFile()`  
返回当前文件标志`current`指向对象的指针。  
QML中需要通过它返回的指针建立起对当前文件的信号监听，以及对当前文件进行编辑操作。

9. [public] `Q_INVOKABLE void setCurrentFile(int index)`  
设置当前文件标志`current`为`openFiles`中第`index`个元素（文件）。  
此函数是留给QML的接口，QML中可以向C++中传 int 型参数，实际完成工作的是该函数接受迭代器参数的重载版本。  

[private] `void setCurrentFile(std::list<TextFile>::iterator index)`  
设置当前文件标志`current`为`openFiles`中第`index`个文件。  
在设置完成之后会调用当前文件的`display()`函数，将当前文件的内容展示在图形界面上。

[signal] `void fileLoaded(QString name)`  
表示文件载入完毕的信号。QML接受到该信号后会将当前文件名`name`展示在tag上（空文件文件名为"Untitled"）。并且清除原来展示在界面上的文件内容，为展示新文件的内容做准备。

