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
    setStyleSheet("QLabel { font-size: 17px; color: #1f222d; border: none; background: lightGray; font-family: Source Code Pro;}");
}

void MainWindow::InfoPanel::updateText() {
    string totalText    = "";
    string fname        = root->currentFile->name;
    string cursorPos    = root->textbox->getCursorPos();
    string tabSize      = to_string(root->textbox->tabSize);
    string fontSize     = to_string(root->textbox->font().pixelSize());
    string directory    = root->currentFile->directory;
    string extension    = root->currentFile->extension;
    string currentdir   = replaceHomeDir(root->currentDirectory);

    if (!root->currentFile->isSaved()) {
        fname += "*";
    }

    /*FILE | string fname        = getFilename(root->filename);
    string cursorPos    = root->textbox->getCursorPos();
    string tabSize      = to_string(root->textbox->tabSize);
    string fontSize     = to_string(root->textbox->font().pixelSize());
    string directory    = getFilename(root->currentDir);
    string extension    = getFileExt(fname);

    if (root->isFileOld()) {
        fname += "*";
    }*/

    list <string> args = {
        fname + " @ " + directory + " (" + cursorPos + ")",
        "ext: " + extension,
        "tab: " + tabSize,
        "font: " + fontSize + "px",
        "dir: " + currentdir
    };

    for (const string &arg : args) {
        totalText += arg + root->cfg->infoPanelSeparator;
    }
    totalText = totalText.substr(0, totalText.length()-root->cfg->infoPanelSeparator.length());

    setText(totalText.c_str());
}
