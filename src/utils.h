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
#include <QTextCursor>
#include <QTextDocumentFragment>

using namespace std;

bool replaceStr(string &str, const string &from, const string &to);
void split(string &str, char delim, vector<string> &out);
int getSelectedLines(QTextCursor &cursor);
string replaceHomeDir(string path);
string getPathDir(string path);
string getFilename(string path);
string getFileExt(string path);
string getConfigPath();
string getHomeDir();
void log(string text);
vector<int> betwStrToVectorInt(string str);

#endif // UTILS_H
