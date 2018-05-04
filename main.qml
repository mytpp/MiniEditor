import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

ApplicationWindow {
    id:root
    visible: true
    width: 1280
    height: 720
    title: qsTr("Scroll")
    //TODO:maybe handle rezise event?

    FileDialog {
        id: fileDialog
        title: "Open"
        nameFilters: [ "TextFiles (*.txt)","All Files (*)" ]
        modality: Qt.WindowModal
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
            ListElement{
                name:' 社会主义从空想到科学的发展'
            }
        }

        ListView{
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
            delegate: Rectangle{
                height: 40
                width: 250
                color: 'white'
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

                    ToolButton{
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 15

                        width: 20
                        height: 20

                        font.family: "FontAwesome"
                        text: "\uf00d"
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
                    text: "\uf07c"
                    font.family: "FontAwesome"
                    onClicked: {
                        fileDialog.open();
                    }
                }
                ToolSeparator {}
            }
            Row {
                ToolButton {//save
                    text: "\uf0c7"
                    font.family: "FontAwesome"
                }
                ToolButton {//redo
                    text: "\uf01e"
                    font.family: "FontAwesome"
                }
                ToolButton {//undo
                    text: "\uf0e2"
                    font.family: "FontAwesome"
                }
                ToolSeparator {}
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
        width: 300
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

        TextInput{
            id:searchInput
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
            height: 20
            width: parent.width*0.9 - 5
            clip: true
            font.pixelSize: 20

            onEditingFinished: {
                //TODO:Search()
            }
        }
        Button{
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 5
            height: parent.height
            width: parent.width*0.1 - 5
            font.family: "FontAwesome"
            text: "\uf00d"

            onClicked: {
                searchBar._enable = false;
                inputBus.focus = true;
                searchInput.clear();
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
        height: root.height - (tabs.height + iconBar.height) - this.anchors.topMargin
        clip: true
        /*Clip the ListView inside, means this.height < columnView.height, and width.*/

        property int fontPixelSize: 16//font size

        /*定义每行的结构*/
        //TODO:Animation
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
                        property bool isSelected: (rowRec.isSelected
                                                   && index >= ((rowRec._index === columnView._sp.y) ? columnView._sp.x : 0)
                                                   && index < ((rowRec._index === columnView._ep.y) ? columnView._ep.x : rowView.count)) ?
                                                      true : false
                        property bool isHighlight: false
                        text: description
                        font.pixelSize: view.fontPixelSize
                        Rectangle{
                            z:-1 //below text
                            anchors.fill: parent
                            color: parent.isSelected ? "#5698c3" : (parent.isHighlight ? "#fed71a" : "white")
                        }
                    }
                }
            }
        }

        /*container of lines*/
        ListView{
            id:columnView
            x:-hbar.position*width
            y:-vbar.position*height
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

            /*change the state of items, set to high light
             *@params: {Qt.point} startPoint point to start drawing color
             *@params: {Qt.point} endPoint point to end drawing color
             *@params: {string} mode "clear" or "highlight" or maybe more
             */
            function drawHighlightRange(startPoint, endPoint, mode){
                var _sp, _ep;
                if(startPoint.x > endPoint.x || startPoint.y > endPoint.y){
                    _sp = endPoint;
                    _ep = startPoint;
                }
                else{
                    _sp = startPoint;
                    _ep = endPoint;
                }
                for(var i = _sp.y; i <= _ep.y; i++){
                    var rowTargetItem = columnView.contentItem.children[i];//row rectangle
                    var length = rowTargetItem.children[0].count;//number of row items
                    console.log(length);
                    for(var j = (i === _sp.y ? _sp.x : 0); j <= (i === _ep.y ? _ep.x : length - 1); j++){
                        var targetItem = rowTargetItem.children[0].contentItem.children[j];
                        /*rowTargetItem.children[0] is the rectangle.*/
                        //console.log(targetItem.text);
                        if(mode === "highlight") targetItem.isHighlight = true;
                        else if (mode === "clear") targetItem.isHighlight = false;
                    }
                }
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    inputBus.focus = true;//activate inputBus
                    var targetRow = columnView.itemAt(mouseX, mouseY);
                    var targetItem = targetRow.children[0].itemAt(mouseX, 0);
//                    var element = {
//                        attributes:[]
//                    };
//                    for(var t = 0; t < Math.random() * 10; t++){
//                        element.attributes.push({description:"n"})
//                    }
//                    textModel.append(element);

                    cursor.x = targetItem.x;
                    cursor.y = targetRow.y;
                }
                onPositionChanged: {
                    /*since the hoverEnable is false, will only matter when pressed down*/
                    var rowIndex = columnView.indexAt(mouseX, mouseY);//y-axis
                    var columnIndex = rowIndex !== -1 ? columnView.itemAt(mouseX, mouseY).children[0].indexAt(mouseX, 0) : -1;//x-axis
                    if(rowIndex >= 0 && columnIndex >= 0){
                        var _end = Qt.point(parent.selectEnd.x, parent.selectEnd.y)
                        parent.selectEnd.y = columnView.indexAt(mouseX, mouseY);
                        parent.selectEnd.x = columnView.itemAt(mouseX, mouseY).children[0].indexAt(mouseX, 0)
                        if(!parent.isSelecting){//init selected range
                            parent.selectStart.y = parent.selectEnd.y
                            parent.selectStart.x = parent.selectEnd.x
                            parent.isSelecting = true;
                        }
                    }
                    /*handle out of range*/
                    var speed = 0.1;//scroll speed
                    if(mouseX - columnView.x > root.width){
                        var relativeX = mouseX - columnView.x;
                        if(columnView.width - columnView.x >= root.width){
                            hbar.position += speed * relativeX;
                        }
                    }
                    if(mouseY - columnView.x > root.height){
                        var relativeY = mouseY - columnView.y;
                        if(columnView.height - columnView.y >= root.height){
                            vbar.position += speed * relativeY;
                        }
                    }
                }
                onReleased: {
                    if(!parent.isSelecting){
                        var rowIndex = columnView.indexAt(mouseX, mouseY);//y-axis
                        var columnIndex = rowIndex !== -1 ? columnView.itemAt(mouseX, mouseY).children[0].indexAt(mouseX, 0) : -1;//x-axis
                        if(rowIndex >= 0 && columnIndex >= 0){
                            var _end = Qt.point(parent.selectEnd.x, parent.selectEnd.y)
                            parent.selectEnd.y = columnView.indexAt(mouseX, mouseY);
                            parent.selectEnd.x = columnView.itemAt(mouseX, mouseY).children[0].indexAt(mouseX, 0)
                            if(!parent.isSelecting){//init selected range
                                parent.selectStart.y = parent.selectEnd.y
                                parent.selectStart.x = parent.selectEnd.x
                            }
                        }
                    }
                    parent.isSelecting = false;
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
                radius: 1
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
                        duration: 400
                    }
                }
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

        /*ListView的纵坐标绑定到此*/
        ScrollBar {
            id: vbar
            hoverEnabled: true
            active: hovered || pressed
            orientation: Qt.Vertical
            size: view.height / columnView.contentHeight
            anchors.top: parent.top
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
            //TODO:insert
            inputBus.clear();
        }

        /*Handle shortcut event*/
        Shortcut{//search
            sequence: "Ctrl+F"
            onActivated: {
                searchBar._enable = true;
            }
        }
        Shortcut{//copy
            sequence: "Ctrl+C"
            onActivated: {
                //TODO:copy()
            }
        }
        Shortcut{//paste
            sequence: "Ctrl+V"
            onActivated: {
                //TODO:paste()
            }
        }
        Shortcut{//undo
            sequence: "Ctrl+Z"
            onActivated: {
                //TODO:undo()
            }
        }
        Shortcut{//save
            sequence: "Ctrl+S"
            onActivated: {
                //TODO:save()
            }
        }
    }

    /*Passage Here*/
    ListModel {
        id: textModel

        ListElement {
            attributes: [
                ListElement { description: "F" },
                ListElement { description: "D" },
                ListElement { description: "S" },
                ListElement { description: "哈" },
                ListElement { description: "哦" },
                ListElement { description: "哈" },
                ListElement { description: "哦" },
                ListElement { description: "哈" },
                ListElement { description: "哦" },
                ListElement { description: "哈" },
                ListElement { description: "哦" },
                ListElement { description: "哈" },
                ListElement { description: "哦" },
                ListElement { description: "哈" },
                ListElement { description: "哦" },
                ListElement { description: " "}
            ]
        }
        ListElement {
            attributes: [
                ListElement { description: "A" },
                ListElement { description: "哈" },
                ListElement { description: "哦" },
                ListElement { description: "哈" },
                ListElement { description: " " }
            ]
        }
        ListElement {
            attributes: [
                ListElement { description: "s" },
                ListElement { description: "g" },
                ListElement { description: " " }
            ]
        }
    }
}

