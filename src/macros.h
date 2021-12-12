#ifndef MACROS_H
#define MACROS_H

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <map>

using namespace std;

class Macros
{
public:
    Macros();
    map<string, string> macrosList;

    string getMacros(string name);
    void loadMacros();
};

#endif // MACROS_H
