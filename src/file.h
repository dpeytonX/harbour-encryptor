#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QFileInfo>
#include <QString>

class File : public QFile, public QFileInfo
{
    Q_OBJECT

    Q_PROPERTY(bool dir READ isDir NOTIFY fileNameChanged)
    Q_PROPERTY(bool executable READ isExecutable NOTIFY fileNameChanged)
    Q_PROPERTY(bool exists READ exists NOTIFY fileNameChanged)
    Q_PROPERTY(bool file READ isFile NOTIFY fileNameChanged)
    Q_PROPERTY(bool hidden READ isHidden NOTIFY fileNameChanged)
    Q_PROPERTY(bool readable READ isReadable NOTIFY fileNameChanged)
    Q_PROPERTY(bool root READ isRoot NOTIFY fileNameChanged)
    Q_PROPERTY(bool symLink READ isSymLink NOTIFY fileNameChanged)
    Q_PROPERTY(bool writable READ isWritable NOTIFY fileNameChanged)
    Q_PROPERTY(QString absoluteFilePath READ absoluteFilePath CONSTANT)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
public:
    explicit File();
    File(const QString &name, QObject *parent=0);

    bool exists() const;
    QString fileName();

    bool isReadable() const;
    bool isWritable() const;

    void setFileName(const QString& name);

signals:
    void fileNameChanged();
};

#endif // FILE_H
