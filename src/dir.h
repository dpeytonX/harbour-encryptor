#ifndef FILELIST_H
#define FILELIST_H

#include <QStandardPaths>
#include <QDir>
#include <QObject>
#include <QString>

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
    Q_PROPERTY(QString XdgConfig MEMBER m_configDir CONSTANT)
    Q_PROPERTY(QString XdgData MEMBER m_dataDir CONSTANT)
    Q_PROPERTY(QString XdgCache MEMBER m_cacheDir CONSTANT)
    Q_PROPERTY(QString XdgHome MEMBER m_homeDir CONSTANT)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit Dir(QObject *parent = 0);

    QString path() const {
        return QDir::path();
    }

    void setPath(const QString &path) {
        QDir::setPath(path);
        emit pathChanged();
    }

signals:
    void pathChanged();

private:
   static const QString m_configDir;
   static const QString m_dataDir;
   static const QString m_cacheDir;
   static const QString m_homeDir;
   QString m_path;

signals:

public slots:

};

#endif // FILELIST_H
