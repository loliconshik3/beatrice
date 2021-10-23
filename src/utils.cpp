#include "utils.h"

bool containFile(vector<File> files, File fl) {
    bool contain = false;
    for (const File & file : files) {
        if (fl.path == file.path) {
            contain = true;
            break;
        }
    }
    return contain;
}

File getContainedFile(vector<File> files, File fl) {
    File contFile;
    for (const File &file : files) {
        if (fl == file) {
            contFile = file;
            break;
        }
    }
    return contFile;
}

void writeStandartConfig() {
    QString fileName = (getHomeDir() + "/.beatrice/config.json").c_str();

    vector<QString> config = {
        "{",
        "    \"window-minimum-width\" : 1000, ",
        "    \"window-minimum-height\" : 600, ",
        "",
        "    \"main-window-background\" : \"#1f222d\", ",
        "",
        "    \"textbox-border\" : \"none\", ",
        "    \"textbox-background\" : \"#1f222d\", ",
        "    \"textbox-font-color\" : \"lightGray\" ",
        "}"
    };

    QFile file(fileName);
    if (file.exists()) {
        return;
    }
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QTextStream out(&file);

    for (const auto &line : config) {
        out << line+"\n";
    }

    file.close();
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
