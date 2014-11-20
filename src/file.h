#ifndef FILE_H
#define FILE_H

#include <QFile>

class File : public QFile
{
    Q_PROPERTY(bool exists READ exists CONSTANT)
    Q_PROPERTY(QString fileName READ fileName CONSTANT)
public:
    File(const QString &name, QObject *parent);
};

#endif // FILE_H
