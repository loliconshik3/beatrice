#include "macroslist.h"
#include "utils.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <QFile>

namespace fs = std::filesystem;

MacrosList::MacrosList()
{
    loadMacroses();
}

Macros* MacrosList::getMacros(string name) {
    Macros *macros = NULL;

    if (macroses.find(name) != macroses.end()) {
        macros = macroses[name];
    }

    return macros;
}

void MacrosList::parseExtensions(Macros *macros) {
    if (macros->text != "") {
        QRegExp rx = QRegExp("\\b(Extensions=)(.*)( )\\b");
        int index = rx.indexIn(macros->text.c_str());
        int length = rx.matchedLength();

        if (index == -1 || length == -1) {
            return;
        }

        string ExtText = macros->text.substr(index, index+length);
        string replaceMacText = ExtText;
        char rsep = '\n';
        vector<string> rout;
        split(replaceMacText, rsep, rout);
        ExtText = rout[0];

        string mcText = macros->text;
        replaceStr(mcText, ExtText+"\n", "");
        macros->text = mcText;

        replaceStr(ExtText, "Extensions=", "");
        char sep = ',';
        vector<string> out;
        split(ExtText, sep, out);
        for (const auto &ext : out) {
            macros->addExtension(ext);
        }
    }
}

void MacrosList::parseSettings(string name) {
    Macros *macros = getMacros(name);

    parseExtensions(macros);
}

void MacrosList::loadMacroses() {
    string homedir = getHomeDir();
    string path = homedir + "/.config/beatrice/macros";
    string fileName;

    macroses.clear();

    for (const auto & entry : fs::directory_iterator(path)){
        string path = entry.path();
        QFile file(path.c_str());

        if ( file.exists() ) {
            fileName = entry.path().filename();

            if ( file.open(QIODevice::ReadOnly) ) {
                log("Load macros: " + fileName);
                string text = file.readAll().toStdString();
                Macros *macros = new Macros(fileName, text);
                macroses[fileName] = macros;
                parseSettings(fileName);
            }
        }

        file.close();
    }
}
