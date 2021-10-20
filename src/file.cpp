#include "file.h"
#include "utils.h"

File::File(string name, string text, string path, string directory, bool isNew)
{
    this->name = name;
    this->text = text;
    this->path = path;
    this->extension = getFileExt(name);
    this->directory = directory;
    this->isNew = isNew;
}
