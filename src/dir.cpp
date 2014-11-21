#include "dir.h"
#include <qdebug.h>

const QString Dir::m_configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
const QString Dir::m_dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
const QString Dir::m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
const QString Dir::m_homeDir = QDir::homePath();

Dir::Dir(QObject *parent) :
    QObject(parent),
    QDir(),
    m_fileList(QQmlListProperty<File>(this, 0,
                                      &Dir::dclAppendObject,
                                      &Dir::dclCountObject,
                                      &Dir::dclAtIndex,
                                      &Dir::dclClearObject))
{

}

QStringList Dir::entryList() {
    return QDir::entryList(Filters(m_filter) | QDir::Readable, SortFlags(m_sort));
}

QQmlListProperty<File> Dir::files() {
    if(getList().isEmpty()) {
        foreach(QString s, entryList()) {
            qDebug() << "abs file path " << absoluteFilePath(s);
            getList().append(new File(absoluteFilePath(s), this));
        }
    }
    return m_fileList;
}

