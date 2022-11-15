import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
Rectangle {

    id: root
    property double elementMinWidth: 50
    property double elementDefaultWidth: 300
    property double elementMinHeigth: 250
    property double padding: 10
    property double dynamicContentHeight: 0
    property color backgroundColor: "lightgray"
    color: backgroundColor

    onWidthChanged: {

        if ( gridLayout.columns > 1 && width < (gridLayout.children[0].implicitWidth*gridLayout.columns +gridLayout.rowSpacing *(gridLayout.columns-1))){
            gridLayout.columns = gridLayout.columns-1
        }
        else if ( width > (gridLayout.children[0].implicitWidth*(gridLayout.columns+1) +gridLayout.rowSpacing *(gridLayout.columns-1))){
            gridLayout.columns = gridLayout.columns+1
        }

        gridLayout.rows = Math.ceil(gridLayout.children.length / gridLayout.columns)
        var tempHeight = 0

        for (let i = 0; i<gridLayout.children.length; i+=gridLayout.columns){
            tempHeight += gridLayout.children[i].height
        }
        tempHeight += (gridLayout.rows)* padding
        dynamicContentHeight = tempHeight
    }

    Component.onCompleted: {

        if(root.children.length <= 1){
            return
        }

        while(root.children.length >1){
            root.children[1].parent = gridLayout
        }

        if ( gridLayout.columns > 1 && width < (gridLayout.children[0].implicitWidth*gridLayout.columns +gridLayout.rowSpacing *(gridLayout.columns-1))){
            gridLayout.columns = gridLayout.columns-1
        }
        else if ( width > (gridLayout.children[0].implicitWidth*(gridLayout.columns+1) +gridLayout.rowSpacing *(gridLayout.columns-1))){
            gridLayout.columns = gridLayout.columns+1
        }


        gridLayout.rows = Math.ceil(gridLayout.children.length / gridLayout.columns)
        var tempHeight = 0

        for (let i = 0; i<gridLayout.children.length; i+=gridLayout.columns){
            tempHeight += gridLayout.children[i].height
        }
        tempHeight += (gridLayout.rows)* padding
        dynamicContentHeight = tempHeight



    }

    Flickable {
        id: flick1
        objectName: "nativeItem"

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.fill: parent
        anchors.rightMargin: padding
        anchors.leftMargin: padding
        anchors.bottomMargin: padding
        anchors.topMargin: padding
        clip: true
        contentWidth: gridLayout.width;
        //        contentHeight: gridLayout.children[0].implicitHeight*Math.ceil(gridLayout.children.length /   gridLayout.columns) +gridLayout.rowSpacing*Math.ceil(gridLayout.children.length /   gridLayout.columns)
        contentHeight: dynamicContentHeight



        GridLayout {
            id: gridLayout
            width: root.width - (root.padding*2)


            columns: 3
            rows: 3

            columnSpacing: root.padding
            rowSpacing: 10


        }
    }
}
