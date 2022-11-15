import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Drawer {

    id: test


    ColumnLayout{
        anchors.fill: parent
        Rectangle{
            id: headerContentBackground
            Layout.fillWidth: true
            Layout.fillHeight: true

        }

    }

    ColumnLayout{
        id: headerContent


    }
}
