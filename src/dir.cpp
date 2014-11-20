#include "dir.h"

#include<QDebug>

const QString Dir::m_configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
const QString Dir::m_dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
const QString Dir::m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
const QString Dir::m_homeDir = QDir::homePath();

Dir::Dir(QObject *parent) :
    QObject(parent),
    QDir(),
    DeclarativeList<File>()
{
}

QStringList Dir::entryList() {
    return QDir::entryList(QDir::NoFilter, QDir::DirsFirst | QDir::Name);
}

QQmlListProperty<File> Dir::fileList() {
    //FileList* files = new FileList(this);
    getList().clear();

    QList<File*> myFiles;

    foreach(QString s, entryList()) {
        myFiles.append(new File(absoluteFilePath(s), this));
    }

    qDebug() << myFiles << myFiles.size();
    QQmlListProperty<File> myList = populate(myFiles, this);
    qDebug() << this;
    return myList;
}
