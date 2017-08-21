import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Tree View")

    Item {
        anchors.fill: parent
        TreeView {
            id: treeBlock
            anchors.fill: parent
            alternatingRowColors: false
            backgroundVisible: false
            style: TreeViewStyle {
               itemDelegate: Item {
                   Text {
                       renderType: Text.NativeRendering
                       font.pointSize: 12
                       anchors.verticalCenter: parent.verticalCenter
                       text: styleData.value.text
                       //color: styleData.textColor
                       color: styleData.selected ? "white" : "gray"
                       elide: styleData.elideMode
                   }
               }

            }

            model: treeModel
            TableViewColumn {
               title: "Список упражнений"
               role: "title"
               width: treeBlock.width
            }
        }
    }

}
