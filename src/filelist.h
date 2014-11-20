#ifndef FILELIST_H
#define FILELIST_H

#include "file.h"
#include "qtdeclarative-helper/declarativelist.h"

class FileList: public DeclarativeList<File>
{
public:
    FileList();
};

#endif // FILELIST_H
