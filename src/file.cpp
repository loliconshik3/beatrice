#include "file.h"
#include "utils.h"

File::File(string name, string text, string path, string directory, bool isNew)
{
    this->name = name;
    this->text = text;
    this->path = path;
    this->extension = getFileExt(name);
    this->directory = directory;
    this->isNew     = isNew;
    this->savedText = text;
    this->cursorPosition = 0;
}

bool File::isSaved() {
    if (text == savedText) {
        return true;
    }

    return false;
}

int File::getFileIndexInList(vector<File *> &fls) {
    int index = 0;

    for (const auto &fl : fls) {
        if (fl->path == this->path) {
            return index;
        }
        ++index;
    }

    return -1;
}

bool File::inList(vector<File*> &fls) {
    for (const auto &fl : fls) {
        if (fl->path == this->path) {
            return true;
        }
    }

    return false;
}

File *File::getFileInList(vector<File*> &fls) {
    File *file = new File();

    for (const auto &fl : fls) {
        if (fl->path == this->path) {
            return fl;
        }
    }

    return file;
}

bool File::removeFileFromList(vector<File*> &fls) {
    File *fl = getFileInList(fls);

    auto it = std::find(fls.begin(), fls.end(), fl);

    if (it != fls.end())
    {
        int index = it - fls.begin();
        fls.erase(fls.begin()+index);
        return true;
    }

    return false;
}
