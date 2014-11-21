#include "dir.h"
#include <qdebug.h>

const QString Dir::m_configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
const QString Dir::m_dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
const QString Dir::m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
const QString Dir::m_homeDir = QDir::homePath();

Dir::Dir(QObject *parent) :
    QObject(parent),
    QDir()
{
}

QStringList Dir::entryList() {
    qDebug() << "filter: " << m_filter;
    return QDir::entryList(Filters(m_filter), QDir::DirsFirst | QDir::Name);
}

QQmlListProperty<File> Dir::fileList() {
    if(!getList().size())
        foreach(QString s, entryList()) {
            getList().append(new File(absoluteFilePath(s), this));
        }

    QQmlListProperty<File> myQmlList = QQmlListProperty<File>(this, 0,
                                                              &Dir::dclAppendObject,
                                                              &Dir::dclCountObject,
                                                              &Dir::dclAtIndex,
                                                              &Dir::dclClearObject);
    emit fileListChanged();
    return myQmlList;
}

