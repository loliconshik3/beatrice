#ifndef UTILS_H
#define UTILS_H

#include <stdexcept>
#include <memory>
#include <cstdio>
#include <vector>
#include <array>

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#include <QTextDocumentFragment>
#include <QTextStream>
#include <QTextCursor>
#include <QFile>

using namespace std;

string exec(const char* cmd);
bool isSyntaxExists(string syntax);
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
