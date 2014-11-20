#include "file.h"

File::File() : QFile(), QFileInfo() {

}

File::File(const QString &name, QObject *parent) : QFile(name, parent),
    QFileInfo(name)
{
}
