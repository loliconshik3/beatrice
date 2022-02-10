#ifndef MACROS_H
#define MACROS_H

#include <map>
#include <vector>
#include <string>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;

class Macros
{
private:
    vector<string> extensions;

public:
    string name;
    string text;

    Macros(string name, string text);

    void addExtension(string extension);
    bool hasExtension(string extension);
};

#endif // MACROS_H
