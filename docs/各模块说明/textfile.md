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
private | std::shared_ptr<SearchVisitor> | searchVisitor 
private | std::vector<std::pair<int,int>>::iterator | currentSearchResult
private | std::list\<EditCommand*> | historyList;
private | std::list\<std::shared_ptr\<EditCommand>>::iterator | nextCommand;


## Public Functions
|索引| 类型 |声明|
|-|-|-|
1| |TextFile()
2| |TextFile(const TextFile &)
3| |TextFile(QUrl address)
4|void | display()
5|bool | save()
6|bool | saveAs()
7|bool | canClose()
8|const QString | fileName() const;
9|Q_INVOKABLE void | cut(int rowBegin, int colBegin, int rowEnd, int colEnd)
10|Q_INVOKABLE void | copy(int rowBegin, int colBegin, int rowEnd, int colEnd)
11|Q_INVOKABLE void | paste(int row, int column)
12|Q_INVOKABLE bool | search(QString format, Qt::CaseSensitivity = Qt::CaseSensitive);
13|Q_INVOKABLE void | showPrevious()
14|Q_INVOKABLE void | showNext()
15|Q_INVOKABLE void | replaceAll(QString newString);
16|Q_INVOKABLE void | replaceCurrent(QString newString);
17|Q_INVOKABLE void | insert(int row, int column, QChar character)
18|Q_INVOKABLE void | insert(int row, int column, QString newString)
19|Q_INVOKABLE void | erase(int row, int column)
20|Q_INVOKABLE void | erase(int rowBegin, int colBegin, int rowEnd, int colEnd)
21|Q_INVOKABLE void | undo()
22|Q_INVOKABLE void | redo()


## Private Functions
| 类型 |声明|
|-|-|
bool | saveFile(QUrl path)
void | addCommand(std::shared_ptr<EditCommand> command)
void | highlightAll(int length = 1);


## Signals
1. insertCha(int row, int column, QChar cha)
2. insertStr(int row, int column, QString str)
3. append(QChar cha)
4. eraseCha(int row, int column)
5. eraseStr(int rowBegin, int colBegin, int rowEnd, int colEnd)
6. eraseLine(int row)
7. highlight(int row, int column, int length = 1)
8. highlightCurrent(int row, int column, int length = 1)  
从QObject继承来一个信号：
9. void destroyed(QObject *obj = Q_NULLPTR)


## Detailed Description
TextFile是衔接QML与C++的重要接口类之一(另一个是Application)，允许底层数据结构、硬盘中的文本文件、展示到UI界面上的编辑界面相互关联起来，扮演着Mediator的角色。
TextFile辅助完成编辑操作，Application辅助完成文件操作。  
所有的编辑操作都是在单个文件（TextFile）上进行的，在不同文件（TextFile）上的操作是独立的。

#### 初始化一个TextFile  
默认构造函数初始化一个没有与硬盘中的文本文件相关联的TextFile对象，对应“新建文件”的操作。接受一个QUrl的构造函数将硬盘中该QUrl对应的地址上的文本文件读入内存中，并以TextStructure类来维护该结构，这个构造函数对应“打开文件”的操作。  
> 注意：若文件中带中文字符，只能处理以GBK编码的这种文件

#### 文件保存与关闭
有4个相关函数save()，saveAs()，saveFile(QUrl path)，canClose()，返回值均为bool型。  
save()将文件保存到url（该类的私有数据成员）表示的硬盘地址中，若该地址为空，则会调用SaveAs弹框询问要保存的地址。  
saveAs()首先弹框询问当前地址，若用户在弹框中选Cancel，则终止保存操作，什么事情都不会发生，若成功获取url，则对该位置调用saveFile()来保存文件。  
saveFile(QUrl path)在path指定的位置创建文件(若已有同名文件会被覆盖)，若创建成功，则将内存（TextStructure）中的文件写到硬盘中，写之前会将文件转为GBK编码。若创建失败，会弹窗发出警告信息。  
以上三个函数均是若成功保存则返回true，否则返回false，故可以根据函数返回值判断是否保存成功。  
canClose()检查文件是否可以关闭，若文件被修改过，则会弹窗提示是否要保存，若在此时选Cancel或不小心关闭了选择保存文件地址的弹窗，则会返回false，表示关闭失败。Application会调用这个函数判断是否可以关闭某个文件，若收到返回值false，则什么都不会做（不关闭文件）。

#### 撤销&重做（undo&redo）
所有的“写”的操作都被封装在EditCommand的子类里，编辑操作在传递到底层数据结构之间要经过一层EditCommand，如下图：

![level](level.png)

TextFile中维护一个command的列表（historyList，使用STL中的list封装的一串EditCommand），用于记录执行过的命令序列，辅助完成撤销/重做操作，nextCommand指向重做（redo）时要执行的下一条指令。  
在未执行过撤销（undo）操作的情况下，nextCommand指向historyList的尾后位置（尾后迭代器），这个位置上是不存在EditCommand的，如图：

![redo undo](undo%20redo0.png)

在当前位置，执行两次撤销操作后，nextCommand会指向黄色的EditCommand：

![redo undo](undo%20redo1.png)

在这个位置上，如果执行撤销操作，则先将nextCommand往回推一步，指向紫色的EditCommand，再调用紫色EditCommand的undo()成员函数。  
若执行重做操作，则先调用黄色EditCommand的redo()成员函数，再将nextCommand向后推一步，指向右边那个灰色的EditCommand。  
若在此处执行普通的编辑操作，则会将nextCommand（包括它当前指向的黄色EditCommand）全部清除，再将新的指令压入historyList末尾，并将nextCommand后推一步，令它依旧是尾后迭代器。执行后的结果如下图：

![undo redo](undo%20redo2.png)

>注意：  
>记录执行过的写操作的历史列表长度最大为50！  
>但是只有当执行过的写操作数超过100时才会将该列表的前50项砍掉。


## Property Documentation
`isModified : bool`   
标记文件是否被修改过。  
用于在关闭文件时，判断文件是否可以关闭。若文件没有被修改过，则一定可以关闭（`canClose()`返回`true`），若文件被修改过，则需要先保存后才能关闭文件。  
这个标记会在`addCommand()`中（执行“写”操作后）被标记为`true`。

`url : QUrl`  
文件在硬盘上的位置。  
若为空(`url==QUrl()`)，说明文件还没有被存到硬盘上。Textfile的默认构造函数会构造空的url，接受一个QUrl参数的构造函数会根据传入的参数初始化`url`。因为QML中url类型默认可以直接与C++中的QUrl类型交换数据，所以在C++中用QUrl类型来储存文件路径。   
`saveFile(QUrl path)`中若保存文件成功，会更新`url`。

`name : QString`   
文件名，会显示在UI界面的tag上。   
在接受一个QUrl参数的构造函数中会根据`url`的内容来更新`name`。`saveFile(QUrl path)`中若保存文件成功，也会根据url的内容来更新`name`。

`text : std::shared_ptr<TextStructure>`   
指向文件底层结构的智能指针。   
该成员会在构造函数中被初始化。在接受一个QUrl参数的构造函数中会根据对应地址的文本文件内容初始化自己。   
选用`std::shared_ptr`来保存TextStructure一是为了减少传参时的不必要拷贝，二是`std::shared_ptr`的模板参数可以是不完全类型，这样就可以在"textfile.h"中只声明TextStructure而不必`#include "textstructure.h"`，这就降低了编译依存性。而且还保证了TextStructure对象的声明周期和该智能指针一样长，不需要自己手动释放内存，很难出现dangling pointer的情况。（下面一些数据成员使用智能指针封装原因类似）

`file : std::fstream`   
url对应地址文件的文件流。  
在接受一个QUrl参数的构造函数中会根据传入的参数讲`file`关联到相应的文件，在读取完毕后会断开该文件流和硬盘上文件的链接。在保存文件（`saveFile(QUrl path)`）的操作中`file`会根据`path`重新建立与硬盘上文件的关联，并通过`SaveVisitor`将`text`中的内容保存到硬盘上，此操作结束后，会再次断开硬盘上文件的关联。

`searchVisitor : std::shared_ptr<SearchVisitor>`   
对当前文件调用`search()`后得到的搜索结果。  
`search()`操作会将搜索结果放进该成员中，然后`showPrevious()`或`showNext()`函数可以根据`searchVisitor`中保存的搜索结果将高亮区域定位到前一个或后一个与搜索格式串相匹配的字符处。

`currentSearchResult : std::vector<std::pair<int,int>>::iterator`  
指向当前搜索结果中被高亮条目的的迭代器。  
用于辅助完成`showPrevious()`和`showNext()`函数。若搜索结果不为空，则`showPrevious()`会高亮`currentSearchResult`前一个搜索结果条目，`showNext()`会高亮`currentSearchResult`后一个搜索结果条目。

`historyList : std::list<std::shared_ptr<EditCommand>>`  
在当前文件上执行过的“写操作”的历史列表。  
用于支持撤销/重做操作。在`addCommand()`中会将一条EditCommand压入历史列表（在此之前会清空`historyList`中`nextCommand`所指向的命令之后的所有命令，包括`nextCommand`指向的命令）。  
更详细说明见Detailed Description部分中“撤销&重做（undo&redo）”一节。

`nextCommand : std::list<std::shared_ptr<EditCommand>>::iterator`  
指向下一次调用`redo()`操作将要执行的命令。  
这是一个`historyList`上的迭代器。在`redo()`操作中会将其加一，在`undo()`操作中会将其减一。


## Member Function Documentation
1. [public] `TextFile::TextFile()`  
默认构造函数。  
初始化一个未命名的文件，这个文件还没有被写到硬盘上，对应“新建文件”功能。

2. [public] `TextFile::TextFile(QUrl address)`  
构造函数。  
根据`address`将该地址的文本文件中的内容读入底层数据结构`TextStructure`中（只能处理GBK编码的中文字符），文件名`name`也会依此初始化。如果无法打开文件，则会弹出警告信息对话框。  
初始化阶段会建立文件流`file`与`address`所指文件的关联，函数结束前会断开该关联。  

3. [public] `TextFile::TextFile(const TextFile & afile)`  
拷贝构造函数。  
只拷贝了`isModified`和`url`两个数据成员。因为Application类的数据成员包括一个TextFile类的`std::list`，自己定义拷贝构造函数是必须的。

4. [public] `void TextFile::display()`  
将TextFile中底层数据结构的内容展示到UI界面上。  
通过用一个DisplayVisitor遍历`text`来实现。

5. [public] `bool TextFile::save()`  
保存文件到硬盘上。  
其返回值表示是否保存成功。  
若文件之前没有在硬盘上储存过，会转而调用`saveAs()`，否则直接对当前TextFile的url调用`saveFile()`来在文件源路径对其保存。  
其他相关说明见Detailed Description部分中“文件保存与关闭”一节。

6. [public] `bool TextFile::saveAs()`  
在指定的路径保存文件。  
其返回值表示是否保存成功。  
弹窗询问要保存文件的路径，若成功获得一个保存路径，则调用`saveFile()`来将文件保存到获得的保存路径上。否则直接返回`false`。  
其他相关说明见Detailed Description部分中“文件保存与关闭”一节。

7. [public] `bool TextFile::canClose()`  
返回值表示文件是否可以关闭。  
如果文件没有被修改过（`isModified == false`），一定是可以关闭的。若文件被修改过，则会弹窗询问是否要保存：
    * 若选择“保存”且保存成功，则文件可以关闭；
    * 若选择“保存”却保存失败，则文件不可以被关闭；
    * 若选择“取消”，文件也可以关闭，但这种情况下关闭文件前不会向硬盘中保存；
    * 若选择取消，则文件不可以被关闭。   
其他相关说明见Detailed Description部分中“文件保存与关闭”一节。

8. [public] `const QString TextFile::fileName() const`  
返回当前文件名。

9. [public] `void TextFile::cut(int rowBegin, int colBegin, int rowEnd, int colEnd)`  
将指定范围中的内容剪切到剪切板。  
`rowBegin`和`colBegin`指定了选中区域的起始坐标，`rowEnd`和`colEnd`指定了选中区域的终止位置坐标。   
对选中范围内的文字进行复制操作，然后删除选中的文字。

10. [public] `void TextFile::copy(int rowBegin, int colBegin, int rowEnd, int colEnd)`
将选中区域的文字复制到剪切板。  
`rowBegin`和`colBegin`指定了选中区域的起始坐标，`rowEnd`和`colEnd`指定了选中区域的终止位置坐标。   
这里的剪切板是操作系统自带的剪切板。该函数提取出选中的文字，将剪切板的内容置为这些文字。

11. [public] `void TextFile::paste(int row, int column)`
将剪切板中的内容粘贴到指定位置。
从操作系统的剪切板中提取出文字内容，将它们插入由`row`和`column`指定位置。

12. [public] `bool TextFile::search(QString format, Qt::CaseSensitivity cs)`  
在文本中搜索格式串`format`出现的位置，`cs`表示是否区分大小写。  
返回值表示是否存在匹配的字符串。  
根据`format`和`cs`构造`searchVisitor`，用它去遍历底层结构以实现搜索操作。之后遍历搜索结果，将所有搜索到的匹配的字符串高亮显示。如果搜索结果不为空，则以另一种颜色高亮第一个搜索结果。

13. [public] `void TextFile::showPrevious()`  
以特殊颜色高亮搜索结果中前一个匹配串。  
如果搜索结果为空，什么都不会做。若当前特殊高亮的已经是第一个匹配串，那么调用此函数会特殊高亮最后一个匹配串，`currentSearchResult`也会被设为指向这个匹配串。  
其他相关信息见Property Documentation部分关于`searchVisitor`和`currentSearchResult`的说明。

14. [public] `void TextFile::showNext()`  
以特殊颜色高亮搜索结果中后一个匹配串。  
如果搜索结果为空，什么都不会做。若当前特殊高亮的已经是最后一个匹配串，那么调用此函数会特殊高亮第一个匹配串，`currentSearchResult`也会被设为指向这个匹配串。  
其他相关信息见Property Documentation部分关于`searchVisitor`和`currentSearchResult`的说明。

15. [public] `void TextFile::replaceAll(QString newString)`  
用新字符串`newString`替换文本中所有的搜索结果。  
如果搜索结果为空，什么都不会做。否则根据搜索结果和`newString`构造出一个`ReplaceCommand`，利用它执行替换操作


16. [public] `void TextFile::replaceCurrent(QString newString)`

[public] `void TextFile::insert(int row, int column, QChar character)`

[public] `void TextFile::insert(int row, int column, QString newString)`

[public] `void TextFile::erase(int row, int column)`

[public] `void TextFile::erase(int rowBegin, int colBegin, int rowEnd, int colEnd)`

[public] `void TextFile::undo()`

[public] `void TextFile::redo()`

[private]  `bool TextFile::saveFile(QUrl path)`  
向`path`指定的路径保存`text`(`std::shared_ptr<TextStructure>`)中存储的文本内容。  
将`file`关联到`path`指定的路径，如果关联成功，则用一个SaveVisitor来遍历`text`实现在指定路径上的文件保存。之后会断开`file`与`path`的关联。若保存成功，更新文件路径`url`和文件名`name`。 若保存失败，弹窗警告。 
返回值表示是否成功保存文件。  
其他相关说明见Detailed Description部分中“文件保存与关闭”一节。

[private] `void TextFile::addCommand(std::shared_ptr<EditCommand> command)`  
向`historyList`中添加一条新命令。  
在压入新命令前，会将`nextCommand`（included）到`historyList`末尾的所有命令清除掉。压入新命令后会将`nextCommand`置为``historyList`的尾后迭代器。最后将“文件是否已修改”的标记为（`isModified`）置为`true`，表示文件已被修改过。  
更多相关说明见Detailed Description部分中“撤销&重做（undo&redo）”一节。

[private] `void highlightAll(int length = 1);`


