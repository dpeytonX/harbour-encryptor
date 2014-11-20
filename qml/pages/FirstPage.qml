import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.encryptor.SailfishWidgets.Components 1.1

Page {

    PageColumn {
        title: "GPG Decryption"

        Button {
            text: "Select File"
            onClicked: {
                pageStack.push(fileSelector)
            }
        }

        Label {
            id: results
            text: fileSelector.selectedFiles.toString()
        }

        FileSelector {
            acceptText: canAccept ? selectedFiles.length + " file(s)" : ""
            headerTitle: "Select encrypted file"
            id: fileSelector
            quickSelect: true
        }
    }
}
