import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.encryptor.SailfishWidgets.Components 1.1

/*!
  Only handles selecting files and returns those files back to user

  Qml                           Instantiates
  This dialog -------------> Qml DirectoryQuery (handles all operations)
                                Sends back
  dynamically <-------------- list of QFiles matching request
  populates list view

  TODO:
      -write basic signal to return data
      -create a parent page to mimick real work flow
      -get data back from signal
      -start backend directoryquery
  */

Dialog {
    property Item contextMenu
    property alias header: listView.header

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
            property bool menuVisible: !!contextMenu && contextMenu.parent == this

            id: contentItem
            height: menuVisible ? contextMenu.height + fileItem.height : fileItem.height


            BackgroundItem {
                id: fileItem

                InformationalLabel {
                    anchors.verticalCenter: parent.verticalCenter
                    x: Theme.paddingLarge
                    text: name
                }
                //TODO: choose icon based on fileType
                //TODO: create a context menu on press and hold

                onPressAndHold: {
                    if(!contextMenu)
                        contextMenu = contextMenuComponent.createObject(listView)
                    contextMenu.show(contentItem)
                }
            }
        }

        Component {
            id:contextMenuComponent
            ContextMenu {
                StandardMenuItem {
                    text: "Option 1"
                }
            }
        }

        VerticalScrollDecorator {}
    }

}


