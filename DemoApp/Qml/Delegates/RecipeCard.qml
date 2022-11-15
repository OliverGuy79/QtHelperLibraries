import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt5Compat.GraphicalEffects
import QtQuick.Layouts

Rectangle {

    id: recipeCardDelegate



    property string imgSrc: image// "qrc:/icons/rounded/Qml/icons/add_home.svg"
    property string title2: model.title
    implicitHeight: 250
    implicitWidth: 450

    Layout.fillWidth:true
    Layout.fillHeight: true



    ColumnLayout{
        id: rowLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        anchors.rightMargin: 10
        anchors.leftMargin: 10

        Image {
            id: recipeImage
            property bool rounded: true
            property bool adapt: true

            source: recipeCardDelegate.imgSrc
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillHeight: false
            Layout.fillWidth: false
            fillMode: Image.Pad

            layer.enabled: rounded
            layer.effect: OpacityMask {
                maskSource: Item {
                    width: recipeImage.width
                    height: recipeImage.height
                    Rectangle {
                        anchors.centerIn: parent
                        width: recipeImage.adapt ? recipeImage.width : Math.min(recipeImage.width, recipeImage.height)
                        height: recipeImage.adapt ? recipeImage.height : width
                        radius: 10
                    }
                }
            }
        }

        Text {
            id: recipeTitle
            text: qsTr(recipeCardDelegate.title2)

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            font.pointSize: 18
            Layout.columnSpan: 1
            Layout.fillWidth: true
            clip: true
            fontSizeMode: Text.Fit
            anchors.bottomMargin: 0
            anchors.topMargin: 10
            anchors.rightMargin: 10
            anchors.leftMargin: 10
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
