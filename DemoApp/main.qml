import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtWebView
import QtMultimedia 5.15
import backend 1.0
import FluidLayouts 1.0
import "Qml/Delegates"
import QtQuick.Controls 6.3

Window {
    id: window
    width: 1200
    height: 1000
    visible: true
    color: "#ffffff"
    title: qsTr("Hello World")



    Drawer{
        id: drawer
        interactive: true


    }

    RestApiManager{
        id: restApiManager
        onResponseReady: function processResponse (response){
            print(JSON.stringify(response))
        }
    }


    QDotEnv{
        id: env
    }

    FirebaseAuthWrapper{

        id: firebaseAuthenticator
        onGoogleUserInfoChanged: {
            print(JSON.stringify(googleUserInfo))
        }
    }

//    FluidGridLayout{

//        id: grid
//        visible: true
//        anchors.top: parent.top
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//        anchors.topMargin: 0
//        anchors.rightMargin: 0
//        backgroundColor: "red"//"#f4f4f4"
//        Repeater{

//            model:10

//            ReceipeCard{
//                Layout.fillWidth:true
//                Layout.fillHeight: true

//            }
//        }
//    }



    ListModel{

        id: recipeModel
        ListElement {
            reciepeId: 716429
            title: "1 - Pasta with Garlic, Scallions, Cauliflower & Breadcrumbs"
            image: "https://spoonacular.com/recipeImages/716429-312x231.jpg"
            imageType: "jpg"
        }

        ListElement {
            reciepeId: 716429
            title: "2 - Pasta with Garlic, Scallions, Cauliflower & Breadcrumbs"
            image: "https://spoonacular.com/recipeImages/716429-312x231.jpg"
            imageType: "jpg"
        }


        ListElement {
            reciepeId: 716429
            title: "3 - Pasta with Garlic, Scallions, Cauliflower & Breadcrumbs"
            image: "https://spoonacular.com/recipeImages/716429-312x231.jpg"
            imageType: "jpg"
        }


        ListElement {
            reciepeId: 716429
            title: "4 - Pasta with Garlic, Scallions, Cauliflower & Breadcrumbs"
            image: "https://spoonacular.com/recipeImages/716429-312x231.jpg"
            imageType: "jpg"
        }
    }

    FluidGridView{

        id: receipeGrid
        anchors.fill: parent
        anchors.topMargin: 200
        model: recipeModel

        delegate: RecipeCard{


        }

        focus: true
    }


    Button {
        id: button
        x: 542
        width: 250
        height: 50
        text: qsTr("Get Receipes")
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 10
        anchors.topMargin: 75

        onClicked: {

            let parameters = {
                apiKey: env.getenv("apiKey"),
                query: recipeDescription.text
            }

            restApiManager.get(env.getenv("backendUrl")+"/recipes/complexSearch", parameters)

        }

    }

    TextField {
        id: recipeDescription
        y: 75
        height: 50
        anchors.left: parent.left
        anchors.right: button.left
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        placeholderText: qsTr("Text Field")
    }

    onWidthChanged: {

    }

    Component.onCompleted:{
        env.load_dotenv()
        print(env.getenv("apiKey"))
    }


}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}D{i:12}D{i:13}
}
##^##*/
