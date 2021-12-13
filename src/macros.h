#ifndef MACROS_H
#define MACROS_H

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Macros
{
public:
    Macros();
    map<string, string> macrosList;
    map<string, vector<string>> macrosExtList;

    bool isMacrosHasExtension(string name, string extension);
    vector<string> getMacrosExtensions(string name);
    void parseSettings(string name);
    string getMacros(string name);
    void loadMacros();
};

#endif // MACROS_H
