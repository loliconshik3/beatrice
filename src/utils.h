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

#include "file.h"

using namespace std;

void split(string &str, char delim, vector<string> &out);
string getPathDir(string path);
string getFilename(string path);
string getFileExt(string path);
string getHomeDir();
bool containFile(vector<File> files, File fl);
File getContainedFile(vector<File> files, File fl);

void writeStandartConfig();

#endif // UTILS_H
