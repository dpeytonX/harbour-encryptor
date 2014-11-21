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
    property real filter: Dir.NoFilter
    property alias header: listView.header
    property string headerTitle
    property bool multiSelect: false
    property bool quickSelect: false //should be based on file filter
    property variant selectedFiles: []
    property string selectText: "Select"

    canAccept: !!selectedFiles && !!(selectedFiles.length)
    id: fileSelector

    BasicListView {
        anchors.fill: parent
        id: listView

        property variant selectedListItems: []

        header: DialogHeader {
            acceptText: fileSelector.acceptText
            cancelText: fileSelector.cancelText
            id: dialogHeader
            title: fileSelector.headerTitle
        }

        model: fileList.files

        delegate: ListItem {
            id: contentItem
            menu: contextMenuComponent
            property bool selected: false
            property File model: modelData

            InformationalLabel {
                anchors.verticalCenter: parent.verticalCenter
                color: selected ? Theme.highlightColor : Theme.primaryColor
                text: modelData.fileName
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

    onBaseDirectoryChanged: fileList.path = baseDirectory

    onRejected: { for(var i = 0; i < selectedFiles.length; i++) makeSelection(selectedFiles[i]) }

    function makeSelection(li) {
        if(li.selected) {
            //deselect
            var i = listView.selectedListItems.indexOf(li)
            var temp = listView.selectedListItems;
            temp.splice(i, 1)
            listView.selectedListItems = temp

            i = selectedFiles.indexOf(li.model)
            temp = selectedFiles;
            temp.splice(i, 1)
            selectedFiles = temp

            console.log("selectedFiles[] : " + selectedFiles)
            li.selected = false
        } else {
            if(!multiSelect) {
                //remove previous entries
                for(var i = 0; i < listView.selectedListItems.length; i++) {
                    listView.selectedListItems[i].selected = false
                }
                listView.selectedListItems = []
                selectedFiles = []
                console.log("selectedFiles[] : " + selectedFiles)
            }

            listView.selectedListItems = listView.selectedListItems.concat(li)
            selectedFiles = selectedFiles.concat(li.model)
            console.log("selectedFiles[] : " + selectedFiles)

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
        console.log("files " + fileList.files)
    }
}


