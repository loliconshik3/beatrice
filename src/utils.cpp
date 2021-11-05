#include "utils.h"

void log(string text) {
    cout << "Log: " << text << endl;
}

int getSelectedLines(QTextCursor &cursor) {
    int selectedLines = 0; //<--- this is it
    if(!cursor.selection().isEmpty())
    {
        QString str = cursor.selection().toPlainText();
        selectedLines = str.count("\n")+1;
    }
    return selectedLines;
}

void split(string &str, char delim, vector<string> &out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(delim, end)) != string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

string getPathDir(string path) {
    string dir = "";
    char sep = '/';
    vector<string> out;

    split(path, sep, out);
    if (out.size() > 1) {
        path = out[out.size()-1];
    }

    int index = 0;
    int size = out.size() - 1;
    while (index < size) {
        dir += "/"+out[index];
        index++;
    }

    return dir;
}

string getFilename(string path) {
    char sep = '/';
    vector<string> out;

    split(path, sep, out);
    if (out.size() > 1) {
        path = out[out.size()-1];
    }

    return path;
}

string getFileExt(string path) {
    char sep = '.';
    vector<string> out;

    split(path, sep, out);
    if (out.size() > 1) {
        path = out[out.size()-1];
    }
    else {
        path = "none";
    }

    return path;
}

string getHomeDir() {
    string homedir;

    if ((homedir = getenv("HOME")).c_str() == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    return homedir;
}

string replaceHomeDir(string path) {
    string homedir = getHomeDir();
    QString newPath = path.c_str();
    newPath = newPath.replace((homedir+"/").c_str(), "~/");
    return newPath.toStdString();
}

bool replaceStr(string &str, const string &from, const string &to) {
    size_t start_pos = str.find(from);
    if(start_pos == string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
