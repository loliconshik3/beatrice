#include "../../include/ui/infopanel.h"
#include "../../include/ui/textbox.h"

#include "../../include/api/utils.h"

#include <QTextCharFormat>
#include <QtWidgets>

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;

InfoPanel::InfoPanel(MainWindow *parent) :
    QLabel(parent)
{
    root = parent;
    setContentsMargins(2, 0, 0, 0);

    //#2e2f30
    //setStyleSheet("QLabel { font-size: 17px; color: #1f222d; border: none; background: lightGray; font-family: Source Code Pro;}");

    updateWidgetStyle();
}

void InfoPanel::updateText() {
    string totalText    = "";
    string fname        = root->currentFile->name;
    string cursorPos    = root->textbox->getCursorPos();
    string tabSize      = to_string(root->textbox->tabSize);
    string fontSize     = to_string(root->textbox->font().pixelSize());
    string directory    = root->currentFile->directory;
    string extension    = root->currentFile->extension;
    string currentdir   = replaceHomeDir(getPathDir(root->currentFile->path));//CURDIR replaceHomeDir(root->currentDirectory);
    string syntax       = root->currentFile->syntax;

    string fileIndex    = to_string(root->currentFile->getFileIndexInList(root->files) + 1);
    string maxFiles     = to_string(root->files.size());

    if (!root->currentFile->isSaved()) {
        fname += "*";
    }

    int selLength = root->textbox->getSelectionText().length();
    if (selLength > 0) {
        cursorPos = to_string(selLength);
    }

    list <string> args = {
        "["+fileIndex+"/"+maxFiles+"] " + fname + " @ " + directory + " (" + cursorPos + ")",
        "ext: "  + extension + "/" + syntax,
        "tab: "  + tabSize,
        "font: " + fontSize + "px"
        //"dir: "  + currentdir
    };

    for (const string &arg : args) {
        totalText += arg + root->cfg->infoPanelSeparator;
    }
    totalText = totalText.substr(0, totalText.length()-root->cfg->infoPanelSeparator.length());

    setText(totalText.c_str());
}

void InfoPanel::updateWidgetStyle() {
    QString style = QString("QLabel { color: %1; border: none; background: %2; }")
                    .arg(root->theme["infopanelFontColor"].c_str(),
                         root->theme["infopanelBackground"].c_str());
    setStyleSheet(style);

    QFont fnt(root->cfg->infopanelFontFamily.c_str());
    fnt.setPixelSize(root->cfg->infopanelFontSize);
    setFont(fnt);
}
