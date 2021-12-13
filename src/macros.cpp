#include "macros.h"
#include "utils.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <QFile>

namespace fs = std::filesystem;

Macros::Macros()
{
    loadMacros();
}

string Macros::getMacros(string name) {
    string macros = "";

    if (macrosList.find(name) != macrosList.end()) {
        macros = macrosList[name];
    }

    return macros;
}

vector<string> Macros::getMacrosExtensions(string name) {
    vector<string> ext = {};

    if (macrosExtList.find(name) != macrosExtList.end()) {
        ext = macrosExtList[name];
    }

    return ext;
}

bool Macros::isMacrosHasExtension(string name, string extension) {
    vector<string> ext = getMacrosExtensions(name);

    if (ext.empty()) {
        return true;
    }

    if ( std::find(ext.begin(), ext.end(), extension) != ext.end() ) {
        return true;
    }

    return false;
}

void Macros::parseSettings(string name) {
    string macros = getMacros(name);

    if (macros != "") {
        QRegExp rx = QRegExp("\\b(Extensions=)(.*)( )\\b");
        int index = rx.indexIn(macros.c_str());
        int length = rx.matchedLength();

        if (index == -1 || length == -1) {
            return;
        }

        string ExtText = macros.substr(index, index+length);
        string replaceMacText = ExtText;
        char rsep = '\n';
        vector<string> rout;
        split(replaceMacText, rsep, rout);
        ExtText = rout[0];

        string mcText = macros;
        replaceStr(mcText, ExtText+"\n", "");
        macrosList[name] = mcText;

        replaceStr(ExtText, "Extensions=", "");
        char sep = ',';
        vector<string> out;
        split(ExtText, sep, out);
        vector<string> extensions = {};
        for (const auto &ext : out) {
            extensions.insert(extensions.end(), ext);
        }
        macrosExtList[name] = extensions;
    }
}

void Macros::loadMacros() {
    string homedir = getHomeDir();
    string path = homedir + "/.config/beatrice/macros";
    string fileName;

    macrosList.clear();

    for (const auto & entry : fs::directory_iterator(path)){
        string path = entry.path();
        QFile file(path.c_str());

        if ( file.exists() ) { // always check whether the file is open
            fileName = entry.path().filename();

            if ( file.open(QIODevice::ReadOnly) ) {
                log("Load macros: " + fileName);
                macrosList[fileName] = file.readAll().toStdString();
            }
            parseSettings(fileName);
        }

        file.close();
    }
}
