#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QFileInfo>

class File : public QFile, QFileInfo
{
    Q_OBJECT

    Q_PROPERTY(bool dir READ isDir NOTIFY fileNameChanged)
    Q_PROPERTY(bool exists READ exists NOTIFY fileNameChanged)
    Q_PROPERTY(bool file READ isFile NOTIFY fileNameChanged)
    Q_PROPERTY(bool hidden READ isHidden NOTIFY fileNameChanged)
    Q_PROPERTY(QString absoluteFilePath READ absoluteFilePath CONSTANT)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
public:
    explicit File();
    File(const QString &name, QObject *parent=0);

    bool exists() {return QFileInfo::exists();}
    QString fileName() {return QFileInfo::fileName();}
    void setFileName(const QString& name) {
        QFile::setFileName(name);
        QFileInfo::setFile(name);
        emit fileNameChanged();
    }

signals:
    void fileNameChanged();
};

#endif // FILE_H
