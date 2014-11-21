#ifndef FILELIST_H
#define FILELIST_H

#include <QStandardPaths>
#include <QDir>
#include <QObject>
#include <QString>
#include <QQmlListProperty>

#include <QDebug>

#include "file.h"

/**
 * @brief The Dir class
 *
 * No point in reinventing the wheel.
 * This class will be a QDir wrapper with functions exposed to the QML Engine.
 *
 * TODO:
 * Copy over SailfishWidgets.JS folder
 *
 */
class Dir : public QObject,QDir
{
    Q_OBJECT

    Q_ENUMS(DirFilter)
    Q_ENUMS(DirSortFlag)

    Q_PROPERTY(bool root READ isRoot NOTIFY pathChanged)
    Q_PROPERTY(int filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(int sort READ sort WRITE setSort NOTIFY sortChanged)
    Q_PROPERTY(QQmlListProperty<File> files READ files NOTIFY filesChanged)
    Q_PROPERTY(QString dirName READ dirName NOTIFY pathChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString XdgCache MEMBER m_cacheDir CONSTANT)
    Q_PROPERTY(QString XdgConfig MEMBER m_configDir CONSTANT)
    Q_PROPERTY(QString XdgData MEMBER m_dataDir CONSTANT)
    Q_PROPERTY(QString XdgHome MEMBER m_homeDir CONSTANT)
    Q_PROPERTY(QStringList entries READ entryList CONSTANT)

public:
    explicit Dir(QObject *parent = 0);

    /*** START: Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies). Under GPL Version 3 License ***/
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
    /*** End: Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies). Under GPL Version 3 License ***/

    QStringList entryList();

    QQmlListProperty<File> files();

    Q_INVOKABLE void refresh() {
        clearList();
        files();
        emit filesChanged();
    }

    QString dirName() const {
        return isRoot() ? root().path() : QDir::dirName();
    }

    int filter() const {
        return m_filter;
    }

    void setFilter(int filter) {
        m_filter = filter;
        emit filterChanged();
    }

    void setPath(const QString &p) {
        QDir nPath(p);
        //Weird workaround for inifinite /../..
        QDir::setPath(nPath.path() == "/.." ? "/" : nPath.absolutePath());
        qDebug() << "path is now " << path();
        emit pathChanged();
    }

    int sort() const {
        return m_sort;
    }

    void setSort(int sort) {
        m_sort = sort;
        emit sortChanged();
    }

    QList<File *>& getList() {
        return m_list;
    }

    void clearList() {
        foreach(File *o, m_list) {
            if(o) o->deleteLater(); //Could still be in use by QML
        }
        m_list.clear();
    }

    // QQmlListProperty helpers
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

signals:
    void pathChanged();
    void filterChanged();
    void filesChanged();
    void sortChanged();

private:
   int m_filter;
   int m_sort;
   QList<File*> m_list;
   QQmlListProperty<File> m_fileList;

   static const QString m_cacheDir;
   static const QString m_configDir;
   static const QString m_dataDir;
   static const QString m_homeDir;
};

#endif // FILELIST_H
