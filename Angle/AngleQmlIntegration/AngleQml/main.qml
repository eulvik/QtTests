import QtQuick 2.0

Rectangle {
    width: 300
    height: 300
    color: Qt.rgba(0, 0, 0, 0.2)
    Rectangle {
        x:50
        y: 50
        color: Qt.rgba(0.0, 1.0, 0, 0.5)
        width: 200
        height: 200
        transform: Rotation { origin.x: 100; origin.y: 100; angle: vm.rotation }
		Text {
			text: "Hello World"
			font.family: "Helvetica"
			font.pointSize: 24
			color: "black"
		}
    }
}
