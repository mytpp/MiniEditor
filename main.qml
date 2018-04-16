import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id:root
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    menuBar: MenuBar { 
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("New")
            }
            MenuItem {
                text: qsTr("Open")
                action: fileOpenAction
            }
            MenuItem {
                text: qsTr("Save")
            }
            MenuItem {
                text: qsTr("Save As...")
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("Exit")
            }
        }// end menu

        Menu {
            title: qsTr("Edit")
            MenuItem {
                text: qsTr("Undo")
            }
            MenuItem {
                text: qsTr("Redo")
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("Find")
            }

            MenuItem {
                text: qsTr("Replace")
            }

            MenuSeparator {}

            MenuItem {
                text: qsTr("Cut")
            }
            MenuItem {
                text: qsTr("Copy")
            }
            MenuItem {
                text: qsTr("Paste")
            }
        }//end menu
        Menu {
            title: qsTr("Help")
            MenuItem {
                text: qsTr("About")
            }
        }

    }//end munebar

    header: ToolBar {
        RowLayout {
            //anchors.fill: parent
            spacing: 5
            ToolButton {
                text: qsTr("👈")
            }
            ToolButton {
                text: qsTr("👉")
            }
            ToolButton {
                text: qsTr("🔎")
            }
        }
    }


    Action {
        id: fileOpenAction
        text: "Open File"
        shortcut: StandardKey.Open //ctrl+O
        onTriggered: fileDialog.open()
    }

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


    ScrollView {
        id:view
        anchors.fill: parent


        Rectangle {
             width: 200; height: 400

             ListModel {
                 id: textModel

                 ListElement {
                     attributes: [
                         ListElement { description: "Core" },
                         ListElement { description: "Core" },
                         ListElement { description: "Core" }
                     ]
                 }
                 ListElement {
                     attributes: [
                         ListElement { description: "Core" }
                     ]
                 }
                 ListElement {
                     attributes: [
                         ListElement { description: "Core" },
                         ListElement { description: "Core" }
                     ]
                 }
             }


             Component {
                 id: textDelegate
                 Item {
                     id: row
                     property int ind: index
                     width: 200; height: 20
                     Row {
                         spacing: 0
                         Repeater {
                             model: attributes

                             Text {
                                 id: cha
                                 text: description
                                 MouseArea {
                                     anchors.fill: parent
                                     onClicked: {
                                         console.log(row.ind +"  "+model.index);
                                         cursor.x = cha.x;
                                         cursor.y = row.y;
                                     }
                                }
                             }
                         }
                     }
//                     MouseArea {
//                          anchors.fill: parent
//                          onClicked: {
//                              //fruitModel.setProperty(index, "cost", cost * 2)
//                              var m="kk"
//                              for(var i=0;i<100;i++)
//                              fruitModel.get(index).attributes.append({description:m})
//                          }
//                    }
                 }
             }

             ListView {
                 anchors.fill: parent
                 model: textModel
                 delegate: textDelegate
             }

             Rectangle {
                 id: cursor
                 height: 20//row.height
                 width: 3
                 color: "black"
                 OpacityAnimator {
                           target: cursor
                           from: 1
                           to: 0
                           duration: 3000
                           loops: Animator.Infinite
                           running: true
                       }
             }
         }



//        Text {
//            id: atxt
//            color: "green"
//            text: qsTr("texth5")
//        }
//        Text {
//            id: btxt
//            anchors.left: atxt.right
//            text: qsTr("222")

//        }
//        Rectangle {
//            color: "yellow"
//            anchors.left: btxt.right
//            height: 300
//            width: 500
//            MouseArea {
//                anchors.fill: parent
//                onPressed: console.log("x:"+ mouseX +" y:"+ mouseY)
//                onReleased: console.log("x:"+ mouseX +" y:"+ mouseY)
//            }
//        }
//    }

//    property var lst: [1,2,3]
//    Component{
//        id:textModel
//        Text {
//            font.pointSize: 24
//        }
//    }

//    Rectangle{
//        color: "red"
//        y: 50
//        width: 50
//        height: 50
//        MouseArea{
//            anchors.fill: parent
//            onClicked: {
//                var te=app.addChar();
//                var i = textModel.createObject(view,{text:te});

////                app.currentFile().test();

////                console.log(root.lst);
////                root.lst.push(4);
////                console.log(root.lst);
//            }
//        }
//    }

//    Rectangle{
//        width: 50
//        height: 50
//        color: "green"
//        MouseArea{
//            anchors.fill: parent
//            onClicked: {
//                app.currentFile().test();
//                //shit.testFunction();
//            }
//        }
//        Text {
//            id: ttttt
//            text: qsTr("text")
//        }
//        Connections{
//            target: app.currentFile()
//            onTestSlot:{
//                console.log(event)
//            }
//        }
    }

}
