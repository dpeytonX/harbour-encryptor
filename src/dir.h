#ifndef FILELIST_H
#define FILELIST_H

#include <QStandardPaths>
#include <QDir>
#include <QObject>
#include <QString>
#include <QQmlListProperty>

#include "file.h"

/**
 * @brief The Dir class
 *
 * No point in reinventing the wheel.
 * This class will be a QDir wrapper with functions exposed to the QML Engine.
 *
 * http://www.cprogramming.com/tutorial/enum.html
 * qt-project.org/doc/qt-5/qstandardpaths.html
 * https://github.com/sailfish-sdk/xdg-helper/blob/master/README.qt5
 * http://qt-project.org/doc/qt-5/qtqml-cppintegration-exposecppattributes.html
 * http://qt-project.org/doc/qt-5/qobject.html#Q_PROPERTY
 *
 * File(Info) List
 * https://github.com/prplmnky/qtdeclarative-helper
 *
 * Q_GADGET
 */
class Dir : public QObject,QDir
{
    Q_OBJECT

    Q_ENUMS(DirFilter)
    Q_ENUMS(DirSortFlag)

    Q_PROPERTY(QString dirName READ dirName)
    Q_PROPERTY(QStringList entries READ entryList CONSTANT)
    Q_PROPERTY(QQmlListProperty<File> files READ fileList NOTIFY fileListChanged)
    //Q_PROPERTY(int filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(int filter MEMBER m_filter NOTIFY filterChanged())
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString XdgConfig MEMBER m_configDir CONSTANT)
    Q_PROPERTY(QString XdgData MEMBER m_dataDir CONSTANT)
    Q_PROPERTY(QString XdgCache MEMBER m_cacheDir CONSTANT)
    Q_PROPERTY(QString XdgHome MEMBER m_homeDir CONSTANT)

public:
    explicit Dir(QObject *parent = 0);

    enum DirFilter { Dirs        = 0x001,
                  Files       = 0x002,
                  Drives      = 0x004,
                  NoSymLinks  = 0x008,
                  AllEntries  = Dirs | Files | Drives,
                  TypeMask    = 0x00f,

                  Readable    = 0x010,
                  Writable    = 0x020,
                  Executable  = 0x040,
                  PermissionMask    = 0x070,

                  Modified    = 0x080,
                  Hidden      = 0x100,
                  System      = 0x200,

                  AccessMask  = 0x3F0,

                  AllDirs       = 0x400,
                  CaseSensitive = 0x800,
                  NoDot         = 0x2000,
                  NoDotDot      = 0x4000,
                  NoDotAndDotDot = NoDot | NoDotDot,

                  NoFilter = -1
    };

    enum DirSortFlag { Name        = 0x00,
                    Time        = 0x01,
                    Size        = 0x02,
                    Unsorted    = 0x03,
                    SortByMask  = 0x03,

                    DirsFirst   = 0x04,
                    Reversed    = 0x08,
                    IgnoreCase  = 0x10,
                    DirsLast    = 0x20,
                    LocaleAware = 0x40,
                    Type        = 0x80,
                    NoSort = -1
    };

    Q_INVOKABLE QString absoluteFilePath(QFile* file) {
        return absoluteFilePath(file->fileName());
    }

    Q_INVOKABLE QString absoluteFilePath(const QString& name) {
        return QDir::absoluteFilePath(name);
    }

    QStringList entryList();

    QQmlListProperty<File> fileList();

    Q_INVOKABLE QString filePath(QFile* file) {
        return filePath(file->fileName());
    }

    Q_INVOKABLE QString filePath(const QString& name) {
        return QDir::filePath(name);
    }

    int getFilter() {
        return m_filter;
    }

    /*void setFilter(int filter) {
        m_filter = filter;
        emit filterChanged();
    }

    void setPath(const QString &path) {
        QDir::setPath(path);
        emit pathChanged();
    }*/

    //////////////////////
    static void dclAppendObject(QQmlListProperty<File> *obj, File *model) {
        Dir *backEnd = dynamic_cast<Dir*>(obj->object);
        if(backEnd) {
            backEnd->getList().append(model);
        }
    }

    static void dclClearObject(QQmlListProperty<File> *obj) {
        Dir *backEnd = dynamic_cast<Dir*>(obj->object);
        if(backEnd) {
            backEnd->clearList();
        }
    }

    static File* dclAtIndex(QQmlListProperty<File> *obj, int index) {
        Dir *backEnd = dynamic_cast<Dir*>(obj->object);
        if(backEnd) {
            return (backEnd->getList())[index];
        }
        return 0;
    }

    static int dclCountObject(QQmlListProperty<File> *obj) {
        Dir *backEnd = dynamic_cast<Dir*>(obj->object);
        if(backEnd) {
            return backEnd->getList().size();
        }
        return 0;
    }

    QList<File *>& getList() {
        return list;
    }

    void setManageMemory(bool manageMemory) {
        this->manageMemory = manageMemory;
    }

    bool doManageMemory() {
        return manageMemory;
    }

    void clearList() {
        foreach(File *o, list) {
            if(o && doManageMemory()) {
                delete o;
            }
        }
        list.clear();
        emit fileListChanged();
    }

public slots:
    void refresh() {clearList(); fileList();}

signals:
    void pathChanged();
    void filterChanged();
    void fileListChanged();

private:
   static const QString m_configDir;
   static const QString m_dataDir;
   static const QString m_cacheDir;
   static const QString m_homeDir;
   int m_filter;
   QString m_path;

   QList<File*> list;
   bool manageMemory;

};

#endif // FILELIST_H
