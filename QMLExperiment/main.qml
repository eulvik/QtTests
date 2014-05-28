import QtQuick 2.2
import QtQuick.Controls 1.1

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Button {
        text: "Add Item"
        anchors.centerIn: parent
        onClicked: nameModel.addNew()
    }

    ListView {
        width: 300; height: parent.height

        model: nameModel
        delegate: Text { text: "Name: " + names + " value: " + values }
    }
}
