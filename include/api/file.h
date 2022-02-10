#ifndef FILE_H
#define FILE_H

#include <vector>
#include <string>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;

class File
{   
public:
    File(string name="Untitled", string text="", string path="", string directory="", bool isNew=true);

    bool isNew;
    string name;
    string text;
    string path;
    string syntax = "none";
    string extension;
    string directory;
    string savedText;
    int cursorPosition;

    bool isSaved();
    bool setSyntax(string snt);
    bool inList(vector<File*> &fls);
    int getFileIndexInList(vector<File*> &fls);
    File *getFileInList(vector<File*> &fls);
    bool removeFileFromList(vector<File*> &fls);

    bool operator == (const File &f1)
    {
        if(f1.path == this->path)
          return true;
        else
          return false;
    }
};

#endif // FILE_H
