import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.encryptor.SailfishWidgets.Components 1.1
import harbour.encryptor.SailfishWidgets.JS 1.1
import harbour.encryptor.Encryptor 1.0

/*!
  Only handles selecting files and returns those files back to user

  Qml                           Instantiates
  This dialog -------------> Qml DirectoryInfo (handles all operations)
                                Sends back
  dynamically <-------------- list of QFiles matching request
  populates list view
  */
//TODO: seg fault on long list (possibly due to listitems being deleted out of view)
Dialog {
    property alias directory: fileList
    property alias filter: fileList.filter
    property alias header: listView.header
    property alias sort: fileList.sort
    property bool multiSelect: false
    property bool quickSelect: true //should be based on file filter
    property int selectionFilter: Dir.AllEntries | Dir.Readable | Dir.Hidden
    property string acceptText: directory.dirName
    property string baseDirectory: fileList.XdgHome
    property string cancelText
    property string deselectText
    property string headerTitle
    property string selectText
    property variant selectedFiles: []

    canAccept: !!selectedFiles && !!(selectedFiles.length)
    id: fileSelector

    BasicListView {
        anchors.fill: parent
        id: listView

        signal updateSelected

        //property variant selectedListItems: []

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
            showMenuOnPressAndHold: matchSelectionFilter(file)
            property bool selected: false//selectedFiles.indexOf(file) != -1
            property File file: modelData

            Image {
                x: Theme.paddingSmall
                height: file.dir ? parent.height - Theme.paddingSmall : 0
                width: height
                id: icon
                source: file.dir ? IconThemes.iconDirectory : ""
            }

            InformationalLabel {
                anchors.left: icon.right
                anchors.leftMargin: Theme.paddingSmall
                anchors.verticalCenter: parent.verticalCenter
                color: selected ? Theme.primaryColor : (matchSelectionFilter(file) ? Theme.highlightColor : Theme.secondaryHighlightColor)
                text: file.fileName
            }

            onClicked: {
                if(file.dir) {
                    fileList.path = file.absoluteFilePath
                } else {
                    if(quickSelect) makeSelection(file)
                }
            }

            Component.onCompleted: listView.updateSelected()
        }


        Component {
            id:contextMenuComponent
            ContextMenu {
                id: context
                StandardMenuItem {
                    text: !!context.parent ? (context.parent.selected ? deselectText : selectText) : ""
                    onClicked: {
                        makeSelection(context.parent.file)
                    }
                }
            }
        }

        VerticalScrollDecorator {}

        onUpdateSelected: {
            for(var i = 0; i < listView.contentItem.children.length; i++) {
                var child = listView.contentItem.children[i]
                if(!!child.file)
                    child.selected = selectedFiles.indexOf(child.file) != -1
            }
        }
    }

    Dir {
        id: fileList
        filter: Dir.AllEntries | Dir.Hidden
        sort: Dir.DirsFirst | Dir.Name

        onPathChanged: {clearSelection(); refresh()}
    }

    onBaseDirectoryChanged: fileList.path = baseDirectory

    onRejected: clearSelection()

    function clearSelection() {
        for(var i = 0; !!selectedFiles && i < selectedFiles.length; i++) makeSelection(selectedFiles[i])
    }

    function makeSelection(file) {
        console.log("make selection: " + file)
        if(selectedFiles.indexOf(file) != -1) {
            //deselect
            //listView.selectedListItems = Variant.remove(listView.selectedListItems, li)
            //selectedFiles = Variant.remove(selectedFiles, li.file)
            selectedFiles = Variant.remove(selectedFiles, file)

            //li.selected = false
        } else {
            if(!matchSelectionFilter(file))
                return

            if(!multiSelect) {
                //remove previous entries
                /*for(var i = 0; i < listView.selectedListItems.length; i++) {
                    listView.selectedListItems[i].selected = false
                }
                listView.selectedListItems = []*/
                selectedFiles = []
            }
            //listView.selectedListItems = Variant.add(listView.selectedListItems, li)
            //selectedFiles = Variant.add(selectedFiles, li.file)
            selectedFiles = Variant.add(selectedFiles, file)

            //li.selected = true
        }

        listView.updateSelected()
        canAccept = selectedFiles.length
    }

    function matchSelectionFilter(file) {
        if((selectionFilter & Dir.Readable) && !file.readable) return false
        if((selectionFilter & Dir.Writable) && !file.writable) return false
        if((selectionFilter & Dir.Executable) && !file.executable) return false
        if((selectionFilter & Dir.NoSymLinks) && file.symLink) return false
        if(!(selectionFilter & Dir.Hidden) && file.hidden) return false

        if(selectionFilter & Dir.AllEntries) return true
        if(selectionFilter & Dir.Dirs & Dir.Files) return true

        if((selectionFilter & Dir.Dirs) && !file.dir) return false
        if((selectionFilter & Dir.AllDirs) && !file.dir) return false
        if((selectionFilter & Dir.Files) && file.dir) return false
        return true
    }
}


