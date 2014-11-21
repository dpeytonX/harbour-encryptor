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
    Q_PROPERTY(QString dirName READ dirName)
    Q_PROPERTY(QStringList entries READ entryList CONSTANT)
    Q_PROPERTY(QQmlListProperty<File> files READ fileList CONSTANT)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString XdgConfig MEMBER m_configDir CONSTANT)
    Q_PROPERTY(QString XdgData MEMBER m_dataDir CONSTANT)
    Q_PROPERTY(QString XdgCache MEMBER m_cacheDir CONSTANT)
    Q_PROPERTY(QString XdgHome MEMBER m_homeDir CONSTANT)

public:
    explicit Dir(QObject *parent = 0);

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

    void setPath(const QString &path) {
        QDir::setPath(path);
        emit pathChanged();
    }

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
    }

public slots:
    void refresh() {clearList();}

signals:
    void pathChanged();

private:
   static const QString m_configDir;
   static const QString m_dataDir;
   static const QString m_cacheDir;
   static const QString m_homeDir;
   QString m_path;

   QList<File*> list;
   bool manageMemory;

signals:

public slots:

};

#endif // FILELIST_H
