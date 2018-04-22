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

    FileDialog {
        id: fileDialog
        title: "Open"
        nameFilters: [ "TextFiles (*.txt)","All Files (*)" ]
        modality: Qt.WindowModal
        onAccepted: {
            //console.log(fileDialog.fileUrl)
            app.addFile(fileDialog.fileUrl)
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
                ToolButton {
                    text: "\uf07c"
                    font.family: "FontAwesome"
                }
                ToolSeparator {}
            }
            Row {
                ToolButton {
                    text: "\uf01e"
                    font.family: "FontAwesome"
                }
                ToolButton {
                    text: "\uf0e2"
                    font.family: "FontAwesome"
                }
                ToolSeparator {}
            }
        }
    }


    ScrollView {
        id:view
        width: parent.width
        anchors.top: iconBar.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 5
        height: root.height - (tabs.height + iconBar.height) - this.anchors.topMargin

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn

        ListView{
            model: textModel
            delegate: ListView{
                    orientation: ListView.Horizontal
                    model: attributes
                    delegate: Text {
                        property bool isSelected: false
                        z:2
                        text: description
                        Rectangle{
                            z:-1 //bellow the text
                            anchors.fill: parent
                            color: parent.isSelected ? "#66a9c9" : "white"
                        }
                        MouseArea{
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: {
                                console.log(index)
                            }

                            onClicked: console.log(index);//index in line
                        }
                    }
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
                ListElement { description: "哦" }
            ]
        }
        ListElement {
            attributes: [
                ListElement { description: "A" }
            ]
        }
        ListElement {
            attributes: [
                ListElement { description: "sss" },
                ListElement { description: "g" }
            ]
        }
    }


//        /*Passage Render*/
//        ListView{
//            model: textModel
//            //define how to display a line
//            delegate: ItemDelegate{
//                height: 16
//                Row{
//                    Repeater{
//                        model: attributes
//                        Text{
//                            text: description
//                            MouseArea{
//                                anchors.fill: parent
//                                onClicked: console.log(view.height)
//                            }
//                        }
//                    }
//                }
//            }
//        }

//        Rectangle {
//            width: 200; height: 400

//            Component {
//                id: textDelegate
//                Item {
//                    id: row
//                    property int ind: index
//                    width: 200; height: 20
//                    Row {
//                        spacing: 0
//                        Repeater {
//                            model: attributes

//                            Text {
//                                id: cha
//                                text: description
//                                MouseArea {
//                                    anchors.fill: parent
//                                    onClicked: {
//                                        console.log(row.ind +"  "+model.index);
//                                        cursor.x = cha.x;
//                                        cursor.y = row.y;
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }

//            ListView {
//                anchors.fill: parent
//                model: textModel
//                delegate: textDelegate
//            }

//            Rectangle {
//                id: cursor
//                height: 20//row.height
//                width: 3
//                color: "black"
//                OpacityAnimator {
//                    target: cursor
//                    from: 1
//                    to: 0
//                    duration: 3000
//                    loops: Animator.Infinite
//                    running: true
//                }
//            }
//        }

}
