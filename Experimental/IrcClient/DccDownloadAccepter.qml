import QtQuick 2.12
import QtQuick.Dialogs 1.3
import com.dcc.dccdownloadobj 1.0

Item{

    id: dccDownloadDirDialogId


    DccDownloadObj {
        id: dccDownloadId
    }

    Connections
    {
        target: _dccDownloadRequestController
        onDownloadRequestSignal:
        {
            dccDownloadId.currentInstance = downloadObj
            print(dccDownloadId.fileName)
            folderDialogId.open()
        }
    }

    FileDialog {
        id: folderDialogId
        title: "Please choose a folder"
        folder: dccDownloadId.downloadDirPath
        selectFolder: true
        onAccepted: {
           console.log("User has selected " + folderDialogId.folder);
           var dir = String(folderDialogId.folder).replace(/^(file:\/{3})/,"")
           console.log("User has selected (filtered) " + dir)
           _dccDownloadRequestController.changeDownloadDirectorySlot(dccDownloadId.currentInstance, dir);
           _dccDownloadRequestController.acceptDownloadRequestSlot(dccDownloadId.currentInstance);
           folderDialogId.close()
       }
    }
}


