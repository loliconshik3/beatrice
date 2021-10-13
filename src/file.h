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
    string directory;
};

#endif // FILE_H
