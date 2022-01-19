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
