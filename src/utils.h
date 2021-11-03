#ifndef UTILS_H
#define UTILS_H

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <vector>

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include <QFile>
#include <QTextStream>

using namespace std;

void split(string &str, char delim, vector<string> &out);
string getPathDir(string path);
string getFilename(string path);
string getFileExt(string path);
string getHomeDir();

#endif // UTILS_H
