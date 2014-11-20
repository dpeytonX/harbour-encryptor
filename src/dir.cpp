#include "dir.h"
#include "qtdeclarative-helper/declarativelist.h"

const QString Dir::m_configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
const QString Dir::m_dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
const QString Dir::m_cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
const QString Dir::m_homeDir = QDir::homePath();

Dir::Dir(QObject *parent) :
    QObject(parent),
    QDir()
{
}

QQmlListProperty<QString> Dir::fileList() {
    DeclarativeList<QString>* declarativeListImpl = new DeclarativeList<QString>();
    QQmlListProperty<QString> fileList = declarativeListImpl->populate(entryList(), this);
    delete declarativeListImpl;
    return fileList;
}
