#include "file.h"

File::File(string name, string text, string path, string directory, bool isNew)
{
    this->name = name;
    this->text = text;
    this->path = path;
    this->directory = directory;
    this->isNew = isNew;
}
