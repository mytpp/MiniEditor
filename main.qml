﻿import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

ApplicationWindow {
    id:root
    visible: true
    width: 1280
    height: 720
    title: qsTr("MiniEditor")

    FileDialog {
        id: fileDialog
        title: "Open"
        nameFilters: [ "TextFiles (*.txt)","All Files (*)" ]
        modality: Qt.WindowModal
        //selectExisting: false //allow to create new(maybe?)
        onAccepted: {
            console.log("open file:" + fileDialog.fileUrl);
            app.addFile(fileDialog.fileUrl);
        }
    }

    Rectangle{
        id:tabs
        width: parent.width
        height: 53
        color: '#2983bb'
        ListModel{
            id:openFiles
        }

        ListView{
            id:openFileTabs
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.bottomMargin: -5
            displayMarginBeginning: 30
            displayMarginEnd: 30
            width: parent.width
            height: 35
            spacing: 15
            orientation: ListView.Horizontal

            model: openFiles

            highlight: Rectangle{
                anchors.fill: parent
                color: 'white'
            }

            delegate: Rectangle{
                height: 40
                width: 250
                color: 'transparent'
                radius: 5
                Column{
                    anchors.fill: parent
                    Text{
                        anchors.baseline: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        color: '#333333'
                        font.family: "FontAwesome"
                        text: "\uf15c"
                    }

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 35
                        anchors.baseline: parent.verticalCenter
                        width: 150
                        color: '#333333'
                        font.family: "Microsoft Yahei"
                        text: model.name
                        elide: Text.ElideRight
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            openFileTabs.currentIndex = index;
                            textModel.clear();
                            cursor.x = 0;
                            cursor.y = 0;
                            columnView.selectStart.x = columnView.selectEnd.x = 0;
                            columnView.selectEnd.y = columnView.selectStart.y = 0;
                            app.setCurrentFile(index);
                        }
                    }

                    ToolButton{
                        visible: openFileTabs.currentIndex == index
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 15

                        width: 20
                        height: 20
                        z:5

                        font.family: "FontAwesome"
                        text: "\uf00d"

                        onClicked: {
                            app.close();
                        }
                    }
                }
            }
        }
    }

    Rectangle{
        id:iconBar
        anchors.top: tabs.bottom
        width: parent.width
        height: 65
        layer.enabled: true
        layer.effect: DropShadow{
            verticalOffset: 2
            transparentBorder: true
            radius: 5
            samples: 15
            color: "#55000000"
        }

        Flow{
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            Row {
                ToolButton {//open file
                    text: "\uf07c  打开"
                    font.family: "FontAwesome"
                    onClicked: {
                        fileDialog.open();
                    }
                }
                ToolSeparator {}
            }
            Row {
                ToolButton {//save
                    text: "\uf0c7 保存"
                    font.family: "FontAwesome"
                    onClicked: {
                        app.save();
                    }
                }
                ToolButton {//redo
                    text: "\uf01e 重做"
                    font.family: "FontAwesome"
                    onClicked: {
                        app.currentFile().redo();
                    }
                }
                ToolButton {//undo
                    text: "\uf0e2 撤销"
                    font.family: "FontAwesome"
                    onClicked: {
                        app.currentFile().undo();
                    }
                }
                ToolSeparator {}
            }
            Row{
                ToolButton {//menu
                    text: "\uf0c9 菜单"
                    font.family: "FontAwesome"
                    onClicked: {
                        contextMenu.popup()
                    }

                    Menu {
                        id: contextMenu
                        Menu{
                            title: "文件"
                            Action{
                                text: "新建"
                                onTriggered:  {
                                    console.log('trig');
                                    app.addFile();
                                    inputBus.focus = true;
                                    inputBus.forceActiveFocus();
                                }
                            }

                            Action{
                                text: "打开"
                                onTriggered: {
                                    fileDialog.open();
                                }
                            }
                            Action{
                                text: "保存"
                                onTriggered: {
                                    app.save();
                                }
                            }
                            Action{
                                text: "保存所有"
                                onTriggered: {
                                    app.saveAll();
                                }
                            }
                            Action{
                                text: "关闭"
                                onTriggered: {
                                    if(app.closeAll())
                                        Qt.quit();
                                }
                            }
                        }
                        MenuSeparator {}
                        Action {
                            text: "复制"
                            onTriggered: {
                                copyCommand.activated();
                            }
                        }
                        Action {
                            text: "粘贴"
                            onTriggered: {
                                pasteCommand.activated();
                            }
                        }
                    }
                }
            }

            Text{
                text:"_sp:" + columnView._sp
                color: "red"
            }
            Text{
                text:"_ep:" + columnView._ep
                color: "green"
            }
        }
    }

    Rectangle{
        id:searchBar
        property bool _enable: false
        width: 400
        height: 40
        border.color: "grey"
        border.width: 1
        radius: 2
        color: "white"

        anchors.top: iconBar.bottom
        anchors.topMargin: -10
        anchors.right: parent.right
        anchors.rightMargin: 10
        z:100//king of all

        layer.enabled: true
        layer.effect: DropShadow{
            verticalOffset: 2
            transparentBorder: true
            radius: 10
            samples: 10
            color: "#55000000"
        }

        Row{
            spacing: 5
            anchors.fill: parent
            TextInput{
                id:searchInput
                anchors.verticalCenter: parent.verticalCenter
                height: 20
                width: 215
                clip: true
                font.pixelSize: 20

                onAccepted: {
                    console.log('search');
                    app.currentFile().search(searchInput.text);
                }
            }
            Button{
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: 40
                font.family: "FontAwesome"
                text: "\uf060"

                onClicked: {
                    console.log('show pre');
                    app.currentFile().showPrevious();
                }
            }
            Button{
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: 40
                font.family: "FontAwesome"
                text: "\uf061"

                onClicked: {
                    console.log("show next");
                    app.currentFile().showNext();
                }
            }
            Button{
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: 40
                text: "替"
                onClicked: {
                    searchMenu.popup();
                }
                Menu{
                    id:searchMenu
                    Action{
                        text: '替换'
                        onTriggered: {
                            app.currentFile().replaceCurrent(searchInput.text);
                        }
                    }
                    Action{
                        text: '替换全部'
                        onTriggered: {
                            app.currentFile().replaceAll(searchInput.text);
                        }
                    }
                }
            }
            Button{
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: 40
                font.family: "FontAwesome"
                text: "\uf00d"

                onClicked: {
                    searchInput.clear();
                    columnView.clearHighlight();
                    searchBar._enable = false;
                    inputBus.focus = true;
                }
            }
        }

        //states depend on this._enable
        states: [
            State {
                name: "inSearch"
                when: searchBar._enable
                PropertyChanges {
                    target: searchBar
                    visible:true
                }
                PropertyChanges {
                    target: searchInput
                    focus:true
                }
            },
            State {
                name:"unSearch"
                when: !searchBar._enable
                PropertyChanges {
                    target: searchBar
                    visible:false
                }
                PropertyChanges {
                    target: searchInput
                    focus:false
                }
            }
        ]
    }


    /*Use Rectangle instead of ScrollView because
     *a ListView inside of which caused tons of problems
     *As the result, draw ScrollBar by self.*/
    Rectangle {
        id:view
        width: parent.width
        anchors.top: iconBar.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        clip: true
        /*Clip the ListView inside, means this.height < columnView.height, and width.*/

        property int fontPixelSize: 20//font size


        /*定义每行的结构*/
        Component{
            id:rowComp
            Rectangle{
                id:rowRec
                height: view.fontPixelSize
                width: rowView.width

                property int _index: index
                property bool isSelected: (index >= columnView._sp.y && index <= columnView._ep.y) ? true : false

                ListView{
                    id:rowView
                    model: attributes
                    //anchors.fill: parent
                    width: childrenRect.width
                    orientation:ListView.Horizontal
                    delegate:Text {
                        //property bool isSelected: isSelect
                        property bool isHL: isHighlight
                        property string highlightMode: ""
                        text: description
                        font.pixelSize: view.fontPixelSize
                        property bool isSelected: (rowRec.isSelected
                                                   && index >= ((rowRec._index === columnView._sp.y) ? columnView._sp.x : 0)
                                                   && index < ((rowRec._index === columnView._ep.y) ? columnView._ep.x : rowView.count)) ?
                                                      true : false
                        Rectangle{
                            z:-1 //below text
                            anchors.fill: parent
                            color: (parent.isSelected) ? "#5698c3" : ((parent.isHL) ? "#fed71a" : "white")
                        }
                    }
                }
            }
        }

        /*container of lines*/
        ListView{
            id:columnView
            x:-hbar.position*width
            interactive: false//disable drag
            width: contentItem.childrenRect.width
            height: parent.height
            model: textModel
            delegate: rowComp

            property bool isSelecting: false
            property point selectStart: Qt.point(0, 0);
            property point selectEnd: Qt.point(0, 0);

            /*using complex logic to pickup the real start point and end point*/
            property point _sp: (columnView.selectStart.y > columnView.selectEnd.y ||
                                     (columnView.selectStart.x > columnView.selectEnd.x &&
                                      columnView.selectStart.y === columnView.selectEnd.y)) ?
                                    columnView.selectEnd : columnView.selectStart

            property point _ep: (columnView.selectStart.y > columnView.selectEnd.y ||
                                 (columnView.selectStart.x > columnView.selectEnd.x &&
                                  columnView.selectStart.y === columnView.selectEnd.y)) ?
                                    columnView.selectStart : columnView.selectEnd
            property real lastFlicked: 0

            property var highLightList: []

            /*change the state of items, set to high light
             *@params: {Qt.point} startPoint point to start drawing color
             *@params: {number} dl length of drawing
             *@params: {string} mode "clear" or "highlight" or maybe more
             */
            function drawHighlightRange(startPoint, dl, mode){
                if(mode === 'na'){//仅高亮
                    for(var i = startPoint.x; i < dl + startPoint.x; i++){
                        textModel.get(startPoint.y).attributes.get(i).isHighlight = true;
                    }
                }
                else if (mode === 'ac'){//选中
                    columnView.selectStart.x = startPoint.x;
                    columnView.selectEnd.x = startPoint.x + dl;
                    columnView.selectStart.y = columnView.selectEnd.y = startPoint.y;
                    columnView.currentIndex = startPoint.y;
                    columnView.currentItem.children[0].currentIndex = startPoint.x + dl;
                    cursor.fixPosition();
                }
                else if (mode === "clear") {//清除状态
                    for(var i = startPoint.x;; i++){
                        if(textModel.get(startPoint.y).attributes.get(i).isHighlight)
                            textModel.get(startPoint.y).attributes.get(i).isHighlight = false;
                        else break;
                    }
                }
            }

            function clearHighlight(){
                for(var i = 0; i < columnView.highLightList.length; i++){
                    columnView.drawHighlightRange(columnView.highLightList[i], null, "clear");
                }
            }

            /*返回实际的起点、终点*/
            function getTruthPoint(){
                var _sp = (columnView.selectStart.y > columnView.selectEnd.y ||
                           (columnView.selectStart.x > columnView.selectEnd.x &&
                            columnView.selectStart.y === columnView.selectEnd.y)) ?
                          columnView.selectEnd : columnView.selectStart;
                var _ep = (columnView.selectStart.y > columnView.selectEnd.y ||
                           (columnView.selectStart.x > columnView.selectEnd.x &&
                            columnView.selectStart.y === columnView.selectEnd.y)) ?
                          columnView.selectStart : columnView.selectEnd;
                return {
                    "_sp" : _sp,
                    "_ep" : _ep
                }
            }

            MouseArea{
                anchors.fill: parent
                cursorShape: Qt.IBeamCursor

                onClicked: {
                    inputBus.focus = true;//activate inputBus
                    inputBus.forceActiveFocus();
                    var targetRow = columnView.itemAt(mouseX, mouseY + columnView.contentY);
                    var targetItem = targetRow.children[0].itemAt(mouseX, 0);

                    if ((mouse.button == Qt.LeftButton) && (mouse.modifiers & Qt.ShiftModifier)){
                        //shift + leftClick 设置选区
                        var rowIndex = columnView.indexAt(mouseX, mouseY);//y-axis
                        var columnIndex = rowIndex !== -1 ? columnView.itemAt(mouseX, mouseY + columnView.contentY).children[0].indexAt(mouseX, 0) : -1;//x-axis
                        if(rowIndex >= 0 && columnIndex >= 0){
                            // 获取之前光标所在位置，设置为选区起点
                            parent.selectStart.y = columnView.indexAt(cursor.x, cursor.y + columnView.contentY);
                            parent.selectStart.x = columnView.itemAt(cursor.x, cursor.y + columnView.contentY).children[0].indexAt(cursor.x, 0);
                            // 设置选区终点为当前鼠标所在位置
                            parent.selectEnd.y = columnView.indexAt(mouseX, mouseY + columnView.contentY);
                            parent.selectEnd.x = columnView.itemAt(mouseX, mouseY + columnView.contentY).children[0].indexAt(mouseX, 0);
                        }
                    }

                    // 设置光标位置
                    cursor.x = targetItem.x;
                    cursor.y = targetRow.y - columnView.contentY;
                }
                onPositionChanged: {
                    /*since the hoverEnable is false, will only matter when pressed down*/
                    var rowIndex = columnView.indexAt(mouseX, mouseY + columnView.contentY);//y-axis
                    var columnIndex = rowIndex !== -1 ? columnView.itemAt(mouseX, mouseY + columnView.contentY).children[0].indexAt(mouseX, 0) : -1;//x-axis
                    if(rowIndex >= 0 && columnIndex >= 0){
                        var _end = Qt.point(parent.selectEnd.x, parent.selectEnd.y)
                        parent.selectEnd.y = columnView.indexAt(mouseX, mouseY + columnView.contentY);
                        var lineView = columnView.itemAt(mouseX, mouseY + columnView.contentY).children[0];
                        parent.selectEnd.x = lineView.indexAt(mouseX, 0);
                        parent.currentIndex = parent.selectEnd.y;//set current index
                        lineView.currentIndex = parent.selectEnd.x;//set current index
                        if(!parent.isSelecting){//init selected range
                            parent.selectStart.y = parent.selectEnd.y
                            parent.selectStart.x = parent.selectEnd.x
                            parent.isSelecting = true;
                        }
                    }
                }
                onReleased: {
                    if(!parent.isSelecting){
                        var rowIndex = columnView.indexAt(mouseX, mouseY + columnView.contentY);//y-axis
                        var columnIndex = rowIndex !== -1 ? columnView.itemAt(mouseX, mouseY + columnView.contentY).children[0].indexAt(mouseX, 0) : -1;//x-axis
                        if(rowIndex >= 0 && columnIndex >= 0){
                            var _end = Qt.point(parent.selectEnd.x, parent.selectEnd.y)
                            parent.selectEnd.y = columnView.indexAt(mouseX, mouseY + columnView.contentY);
                            var lineView = columnView.itemAt(mouseX, mouseY + columnView.contentY).children[0];
                            parent.selectEnd.x = lineView.indexAt(mouseX, 0);
                            parent.currentIndex = parent.selectEnd.y;//set current index
                            lineView.currentIndex = parent.selectEnd.x;//set current index
                            if(!parent.isSelecting){//init selected range
                                parent.selectStart.y = parent.selectEnd.y
                                parent.selectStart.x = parent.selectEnd.x
                            }
                        }
                    }
                    parent.isSelecting = false;
                }

                onWheel: {
                    parent.flick(0, wheel.angleDelta.y * 5);
                }
            }

            /*光标*/
            Rectangle{
                id:cursor
                width: 2
                height: view.fontPixelSize
                x:0
                y:0
                color: "#1772b4"
                SequentialAnimation{//闪烁动画
                    running: true
                    loops: Animation.Infinite
                    NumberAnimation{
                        target: cursor
                        property: "opacity"
                        to: 0
                        duration: 600
                    }
                    NumberAnimation{
                        target: cursor
                        property: "opacity"
                        to: 1
                        duration: 600
                    }
                    NumberAnimation{
                        target: cursor
                        property: "opacity"
                        to: 1
                        duration: 400
                    }
                }
                function fixPosition(){
                    this.x = columnView.currentItem.children[0].currentItem.x;
                    this.y = columnView.currentItem.y - columnView.contentY;
                }
            }

            onMovementStarted: {
                lastFlicked = contentY;
            }
            onMovementEnded: {
                cursor.y -= contentY - lastFlicked;
            }
        }

        /*ListView的横坐标绑定到此*/
        ScrollBar {
            id: hbar
            hoverEnabled: true
            active: hovered || pressed
            orientation: Qt.Horizontal
            size: view.width / columnView.contentItem.childrenRect.width
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }

    /*处理所有的键盘输入事件、快捷键*/
    TextInput{
        id:inputBus
        visible: false
        focus:true
        onTextEdited: {
            console.log(inputBus.text);
            var _sp = (columnView.selectStart.y > columnView.selectEnd.y ||
                       (columnView.selectStart.x > columnView.selectEnd.x &&
                        columnView.selectStart.y === columnView.selectEnd.y)) ?
                      columnView.selectEnd : columnView.selectStart;

            console.log('insert');
            cF.insert(_sp.y, _sp.x, inputBus.text);
            inputBus.clear();
        }
        onAccepted: {
            console.log('enter');
            app.currentFile().insert(columnView._ep.y, columnView._ep.x, '\n');
        }

        Keys.priority: Keys.BeforeItem
        Keys.onPressed: {
            if(event.key == Qt.Key_Delete){
                console.log('delete');
                app.currentFile().erase(columnView.selectEnd.y, columnView.selectEnd.x);
            }
            else if(event.key == Qt.Key_Backspace){
                console.log('backspace');
                if(columnView.selectStart === columnView.selectEnd){
                    if(columnView.selectEnd.x != 0){
                        app.currentFile().erase(columnView.selectEnd.y, columnView.selectEnd.x - 1);
                    }
                    else{
                        var endPos = textModel.get(columnView.selectEnd.y - 1).attributes.count - 1;
                        app.currentFile().erase(columnView.selectEnd.y - 1, endPos);
                    }
                }
                else{

                    var _sp = columnView.getTruthPoint()["_sp"];
                    var _ep = columnView.getTruthPoint()["_ep"];
                    app.currentFile().erase(_sp.y, _sp.x, _ep.y, _ep.x);
                }
            }
            else if(event.key == Qt.Key_Up){
                console.log('up');
                if(columnView.selectEnd.y <= 0) return;
                var bound = textModel.get(columnView.selectEnd.y - 1).attributes.count - 1;
                if(columnView.selectEnd.x > bound){
                    var endPos = bound;
                    columnView.selectStart.x = columnView.selectEnd.x = endPos;
                    columnView.selectStart.y--;
                    columnView.selectEnd.y--;
                    columnView.currentIndex = columnView.selectEnd.y;
                    columnView.currentItem.children[0].currentIndex = columnView.selectEnd.x;
                }
                else{
                    columnView.selectStart.y--;
                    columnView.selectEnd.y--;
                    columnView.currentIndex = columnView.selectEnd.y;
                    columnView.currentItem.children[0].currentIndex = columnView.selectEnd.x;
                }
                cursor.fixPosition();
            }
            else if(event.key == Qt.Key_Down){
                if(columnView.selectEnd.y >= textModel.count - 2) return;
                console.log('down')
                var bound = textModel.get(columnView.selectEnd.y + 1).attributes.count - 1;
                if(columnView.selectEnd.x > bound){
                    var endPos = bound;
                    columnView.selectStart.x = columnView.selectEnd.x = endPos;
                    columnView.selectStart.y++;
                    columnView.selectEnd.y++;
                    columnView.currentIndex = columnView.selectEnd.y;
                    columnView.currentItem.children[0].currentIndex = columnView.selectEnd.x;
                }
                else{
                    columnView.selectStart.y++;
                    columnView.selectEnd.y++;
                    columnView.currentIndex = columnView.selectEnd.y;
                    columnView.currentItem.children[0].currentIndex = columnView.selectEnd.x;
                }
                cursor.fixPosition();
            }
            else if(event.key == Qt.Key_Left){
                console.log('left');
                if(columnView.selectEnd.x <= 0) return;
                columnView.selectEnd.x--;
                columnView.selectStart.x--;
                columnView.currentItem.children[0].currentIndex--;
                cursor.fixPosition();
            }
            else if(event.key == Qt.Key_Right){
                console.log('right')
                if(columnView.selectEnd.x >= textModel.get(columnView.selectEnd.y).attributes.count - 1) return;
                columnView.selectEnd.x++;
                columnView.selectStart.x++;
                columnView.currentItem.children[0].currentIndex++;
                cursor.fixPosition();
            }
        }

        /*Handle shortcut event*/
        Shortcut{//search
            sequence: "Ctrl+F"
            onActivated: {
                searchBar._enable = true;
            }
        }
        Shortcut{//copy
            id:copyCommand
            sequence: StandardKey.Copy //"Ctrl+C"
            onActivated: {
                console.log("copy");
                var _sp = columnView.getTruthPoint()["_sp"];
                var _ep = columnView.getTruthPoint()["_ep"];
                cF.copy(_sp.y, _sp.x, _ep.y, _ep.x);
            }
        }
        Shortcut{//paste
            id:pasteCommand
            sequence: StandardKey.Paste//"Ctrl+V"
            onActivated: {
                console.log("paste");
                var _ep = columnView.getTruthPoint()["_ep"];
                cF.paste(_ep.y, _ep.x);
            }
        }
        Shortcut{//undo
            sequence: "Ctrl+Z"
            onActivated: {
                app.currentFile().undo();
            }
        }
        Shortcut{//save
            sequence: "Ctrl+S"
            onActivated: {
                app.save();
            }
        }
    }

    /*Passage Here*/
    ListModel {
        id: textModel
    }

    Connections{
        target: app
        onFileLoaded:{
            console.log("loaded");
            var exist = 0;
            for(var i = 0; i < openFiles.count; i++){
                if(openFiles.get(i).name == name){
                    openFileTabs.currentIndex = i;
                    exist = 1;
                    break;
                }
            }

            if(!exist) {
                openFiles.append({name: name});
                openFileTabs.currentIndex = openFiles.count - 1;
            }
            textModel.clear();
            cursor.x = 0;
            cursor.y = 0;
            columnView.selectStart.x = columnView.selectEnd.x = 0;
            columnView.selectEnd.y = columnView.selectStart.y = 0;
            currentFile.target = cF;
            console.log("count "+openFiles.count);
        }
    }
    Connections{
        id:currentFile
        target: null
        ignoreUnknownSignals: true

        /*----------修改Model操作----------*/
        onAppend:{
            //console.log(str);
            if(textModel.count == 0) textModel.append({attributes:[]});
            for(var i = 0; i < str.length; i++){
                var cha = str[i];
                if(cha !== '\n'){
                    textModel.get(textModel.count - 1).attributes.append({description: cha, isHighlight:false});
                }
                else{
                    textModel.get(textModel.count - 1).attributes.append({description: ' ', isHighlight:false});
                    textModel.append({attributes:[]});
                }
            }
        }
        onInsertCha:{//插入字符
            if(cha !== '\n'){
                textModel.get(row).attributes.insert(column, {description: cha, isHighlight:false});
                //修复插入后光标位置
                columnView.selectEnd.x = columnView.selectStart.x = column + 1;
                columnView.currentItem.children[0].currentIndex = column + 1;
                cursor.fixPosition();
            }
            else{
                textModel.insert(row + 1, {attributes:[{description:' ', isHighlight:false}]});//插入末尾空字符
                var preLine = textModel.get(row).attributes;
                for(var i = preLine.count - 2; i >= column; i--){
                    textModel.get(row + 1).attributes.insert(0, preLine.get(i));
                    preLine.remove(i);
                }

                columnView.selectStart.x = columnView.selectEnd.x = 0;
                columnView.selectEnd.y = columnView.selectStart.y = row + 1;
                columnView.currentIndex = row + 1;
                cursor.fixPosition();
            }
        }
        onInsertStr:{//插入字符串
            console.log("ins str");
            console.log(str);
            var _row = row;
            var _column = column;
            for(var i = 0; i < str.length; i++){
                if(str[i] !== '\n'){
                    textModel.get(_row).attributes.insert(_column, {description: str[i], isHighlight:false});
                    columnView.selectEnd.x = columnView.selectStart.x = _column + 1;
                    columnView.currentItem.children[0].currentIndex = _column + 1;
                    _column++;
                }
                else{
                    var element = {
                        attributes:[]
                    }
                    element.attributes.push({description:' ', isHighlight:false});
                    textModel.insert(_row + 1, element);
                    var preLine = textModel.get(_row).attributes;
                    for(var j = preLine.count - 2; j >= _column; j--){
                        textModel.get(_row + 1).attributes.insert(0, preLine.get(j));
                        preLine.remove(j);
                    }
                    columnView.selectStart.x = columnView.selectEnd.x = 0;
                    columnView.selectEnd.y = columnView.selectStart.y = _row + 1;
                    columnView.currentIndex = _row + 1;
                    _row++;
                    _column = 0;
                }
            }
            cursor.fixPosition();
        }
        onEraseCha:{//删除字符
            var nowLine = textModel.get(row).attributes;//当前行
            if(column == nowLine.count - 1){//删除该行末尾换行符
                if(row == textModel.count - 1) return;//不允许删除最末位换行符
                var nextLine = textModel.get(row + 1).attributes;//下一行
                nowLine.remove(column);
                for(var j = 0; j < nextLine.count; j++){
                    nowLine.append(nextLine.get(j));
                }
                textModel.remove(row + 1);
            }
            else{
                nowLine.remove(column);
            }

            columnView.selectStart.y = columnView.selectEnd.y = row;
            columnView.selectStart.x = columnView.selectEnd.x = column;
            columnView.currentIndex = row;
            columnView.currentItem.children[0].currentIndex = column;
            cursor.fixPosition();
        }
        onEraseStr:{
            console.log('erase str');
            console.log(rowBegin + ':' + rowEnd);
            console.log(columnBegin + ':' + columnEnd);
            if(rowEnd == rowBegin){
                if(columnEnd - columnBegin == textModel.get(rowBegin).attributes.count){
                    textModel.remove(rowBegin);
                    columnView.selectStart.y = columnView.selectEnd.y = columnView.currentIndex = rowBegin - 1;
                    columnView.selectStart.x = columnView.selectEnd.x = textModel.get(rowBegin - 1).attributes.count - 1;
                    columnView.currentItem.children[0].currentIndex = textModel.get(rowBegin - 1).attributes.count - 1;
                }
                else{
                    textModel.get(rowBegin).attributes.remove(columnBegin, columnEnd - columnBegin);
                    columnView.selectStart.x = columnView.selectEnd.x = columnBegin;
                    columnView.currentItem.children[0].currentIndex = columnBegin;
                }
            }
            else {
                for(var i = rowEnd; i >= rowBegin; i--){
                    if(i == rowEnd){//最后一行
                        if(columnEnd == textModel.get(i).attributes.count || columnEnd == textModel.get(i).attributes.count - 1){
                            textModel.remove(i);
                        }
                        else{
                            textModel.get(i).attributes.remove(0, columnEnd);
                        }
                    }
                    else if(i == rowBegin){//第一行
                        var nextLine = textModel.get(i + 1).attributes;
                        var line = textModel.get(i).attributes;
                        line.remove(columnBegin, line.count - columnBegin);
                        for(var j = 0; j < nextLine.count; j++){
                            line.append(nextLine.get(j));
                        }
                        textModel.remove(i + 1);//remove nextLine
                    }
                    else{//中间行
                        textModel.remove(i);
                    }
                }
                columnView.selectStart.y = columnView.selectEnd.y = columnView.currentIndex = rowBegin;
                columnView.selectStart.x = columnView.selectEnd.x = columnBegin;
                columnView.currentItem.children[0].currentIndex = columnBegin;
            }
            cursor.fixPosition();
        }
        onEraseLine:{//删除一行末尾的换行符
            var nextLine = textModel.get(row + 1).attributes;//下一行
            var nowLine = textModel.get(row).attributes;//当前行
            nowLine.remove(column);
            for(var j = 0; j < nextLine.count; j++){
                nowLine.append(nextLine.get(j));
            }
            textModel.remove(row + 1);
            columnView.selectStart.y = columnView.selectEnd.y = row;
            columnView.selectStart.x = columnView.selectEnd.x = column;
            columnView.currentIndex = row;
            columnView.currentItem.children[0].currentIndex = column;
            cursor.fixPosition();
        }
        /*--------高亮操作--------*/
        onHighlight:{
            console.log(column + ':' + row + ':' + length);
            columnView.drawHighlightRange(Qt.point(column, row), length, 'na');
            columnView.highLightList.push(Qt.point(column, row));
        }
        onHighlightCurrent:{
            columnView.drawHighlightRange(Qt.point(columnView.selectStart.x, columnView.selectStart.y), length, 'ck');
            columnView.drawHighlightRange(Qt.point(column, row), length, 'ac');
        }
        onFileDestroyed: {
            openFiles.remove(openFileTabs.currentIndex);
            textModel.clear();
            cursor.x = 0;
            cursor.y = 0;
        }
    }

    onClosing: {
         close.accepted = false
         if(app.closeAll())
             Qt.quit();
     }

}

