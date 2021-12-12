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
                macrosList[fileName] = file.readAll().toStdString();
            }
        }

        file.close();
    }
}
