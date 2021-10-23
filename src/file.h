#ifndef FILE_H
#define FILE_H

#include <string>

using namespace std;

class File
{   
public:
    File(string name="Untitled", string text="", string path="", string directory="", bool isNew = true);

    bool isNew;
    string name;
    string text;
    string path;
    string extension;
    string directory;
    string savedText = "";

    bool isSaved();

    bool operator == (const File &f1)
    {
       if(f1.path == this->path)
         return true;
       else
         return false;
    }

    bool operator == (const File *f1)
    {
       if(f1->path == this->path)
         return true;
       else
         return false;
    }
};

#endif // FILE_H
