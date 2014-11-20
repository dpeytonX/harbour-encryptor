import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.encryptor.SailfishWidgets.Components 1.1
import harbour.encryptor.Encryptor 1.0

/*!
  Only handles selecting files and returns those files back to user

  Qml                           Instantiates
  This dialog -------------> Qml DirectoryInfo (handles all operations)
                                Sends back
  dynamically <-------------- list of QFiles matching request
  populates list view

  TODO:
      -mock navigation
      -start backend directoryquery
  */
Dialog {
    property Item contextMenu
    property string acceptText
    property string baseDirectory: fileList.XdgHome
    property string cancelText
    property string deselectText: "Deselect"
    property alias header: listView.header
    property string headerTitle
    property bool multiSelect: false
    property bool quickSelect: false //should be based on file filter
    property variant selectedFiles: []
    property string selectText: "Select"

    signal filesSelected(variant files)

    canAccept: !!selectedFiles && !!(selectedFiles.length)
    id: fileSelector

    ListModel {
        id: list1
        ListElement {name: ".."; fileType: "dir"}
        ListElement {name: "."; fileType: "dir"}
        ListElement {name: "Folder 1"; fileType: "dir"}
        ListElement {name: "Folder 2"; fileType: "dir"}
        ListElement {name: "Folder 3"; fileType: "dir"}
        ListElement {name: "Folder 4"; fileType: "dir"}
        ListElement {name: "Folder 5"; fileType: "dir"}
        ListElement {name: "File 1"; fileType: "file"}
        ListElement {name: "File 2"; fileType: "file"}
        ListElement {name: "File 3"; fileType: "file"}
        ListElement {name: "File 4"; fileType: "file"}
        ListElement {name: ".hidden 1"; fileType: "file"}
        ListElement {name: ".hidden 2"; fileType: "file"}
    }

    ListModel {
        id: list2
        ListElement {name: "File 1"; fileType: "file"}
        ListElement {name: "File 2"; fileType: "file"}
        ListElement {name: "File 3"; fileType: "file"}
        ListElement {name: "File 4"; fileType: "file"}
    }

    BasicListView {
        anchors.fill: parent
        id: listView

        header: DialogHeader {
            acceptText: fileSelector.acceptText
            cancelText: fileSelector.cancelText
            id: dialogHeader
            title: fileSelector.headerTitle
        }

        //TODO: create fileType enum
        model: list1

        delegate: ListItem {
            id: contentItem
            menu: contextMenuComponent
            property bool selected: false

            InformationalLabel {
                anchors.verticalCenter: parent.verticalCenter
                color: selected ? Theme.highlightColor : Theme.primaryColor
                text: name
                x: Theme.paddingLarge
            }

            onClicked: if(quickSelect) makeSelection(this)
        }


        Component {
            id:contextMenuComponent
            ContextMenu {
                id: context
                StandardMenuItem {
                    text: !!context.parent ? (context.parent.selected ? deselectText : selectText) : ""
                    onClicked: {
                        makeSelection(context.parent)
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }

    onAccepted: filesSelected(selectedFiles)

    onBaseDirectoryChanged: fileList.path = baseDirectory

    onRejected: { for(var i = 0; i < selectedFiles.length; i++) makeSelection(selectedFiles[i]) }

    function makeSelection(li) {
        if(li.selected) {
            //deselect
            var i = selectedFiles.indexOf(li)
            var temp = selectedFiles;
            temp.splice(i, 1)
            selectedFiles = temp
            console.log("selectedFiles[] : " + selectedFiles.toString())
            li.selected = false
        } else {
            if(!multiSelect) {
                //remove previous entries
                for(var i = 0; i < selectedFiles.length; i++) {
                    selectedFiles[i].selected = false
                }
                selectedFiles = []
                console.log("selectedFiles[] : " + selectedFiles.toString())
            }

            selectedFiles = selectedFiles.concat(li)
            console.log("selectedFiles[] : " + selectedFiles.toString())
            li.selected = !li.selected
            li.selected = true
        }

        canAccept = selectedFiles.length
    }

    Dir {
        id: fileList

        onPathChanged: navigate()
    }

    function navigate() {
        console.log("navigate called " + fileList.path)
    }
}


