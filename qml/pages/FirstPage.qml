import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.encryptor.SailfishWidgets.Components 1.1

/*!
  Only handles selecting files and returns those files back to user

  Qml                           Instantiates
  This dialog -------------> Qml DirectoryInfo (handles all operations)
                                Sends back
  dynamically <-------------- list of QFiles matching request
  populates list view

  TODO:
      -mock navigation
      -create a parent page to mimick real work flow
      -start backend directoryquery
  */

Dialog {
    property Item contextMenu
    property alias header: listView.header
    property string selectText: "Select"
    property string deselectText: "Deselect"
    //TODO: selected files should not be list objects because they will be destroyed
    property variant selectedFiles: []
    property bool multiSelect: false

    signal filesSelected(variant files)

    BasicListView {
        anchors.fill: parent
        id: listView

        header: DialogHeader {
            title: "Select encrypted file"
        }

        //TODO: create fileType enum
        model: ListModel {
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
        }


        Component {
            id:contextMenuComponent
            ContextMenu {
                id: context
                StandardMenuItem {
                    text: !!context.parent ? (context.parent.selected ? deselectText : selectText) : ""
                    onClicked: {
                        var li = context.parent
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
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }

    onAccepted: {
        filesSelected(selectedFiles)
        console.log("accepted: " + selectedFiles)
    }
}


