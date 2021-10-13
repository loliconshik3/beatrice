#include "infopanel.h"
#include "textbox.h"
#include "utils.h"

#include <QtWidgets>
#include <QTextCharFormat>

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;

MainWindow::InfoPanel::InfoPanel(MainWindow *parent) :
    QLabel(parent)
{
    root = parent;
    setContentsMargins(2, 0, 0, 0);
    //#2e2f30
    setStyleSheet("QLabel { font-size: 18px; color: #1f222d; border: none; background: lightGray; font-family: Source Code Pro;}");
}

void MainWindow::InfoPanel::updateText() {
    string totalText = "";
    string fname = root->filename;
    string cursorPos = root->textbox->getCursorPos();
    string tabSize = to_string(root->textbox->tabSize);
    string fontSize = to_string(root->textbox->font().pixelSize());
    string directory = getFilename(root->currentDir);
    string extension = getFileExt(fname);

    char sep = '/';
    vector<string> out;
    split(fname, sep, out);
    if (out.size() > 1) {
        fname = out[out.size()-1];
    }

    if (root->isFileOld()) {
        fname += "*";
    }

    list <string> args = {
        fname + " (" + cursorPos + ")",
        "ft:" + extension,
        "Tab: " + tabSize,
        "Font: " + fontSize + "px",
        "Directory: " + directory,
        "beatrice",
        "by loliconshik3"
    };

    for (const string &arg : args) {
        totalText += arg + " | ";
    }
    totalText = totalText.substr(0, totalText.length()-3);

    setText(totalText.c_str());
}
