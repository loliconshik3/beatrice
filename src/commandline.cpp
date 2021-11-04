#include "commandline.h"
#include "mainwindow.h"
#include "textbox.h"
#include "utils.h"

#include <QtWidgets>

MainWindow::CommandLine::CommandLine(MainWindow *parent) :
    QLineEdit(parent)
{
    root = parent; //#1f1f1f //2e2f30
    setStyleSheet("QLineEdit { font-size: 15px; color: lightGray; border: none; background: #1f222d; font-family: Source Code Pro; }");

    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_launchCommand1), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(launchCommand()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_completeCommand), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(completeCommand()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_launchCommand2), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(launchCommand()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_cmdEscape), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(escape()));
}

void MainWindow::CommandLine::escape() {
    if (root->focusWidget() == this) {
        root->textbox->setFocus();
        clear();
    }
}

void MainWindow::CommandLine::launchCommand() {
    std::string commandText = text().toStdString();

    clear();

    char sep = ' ';
    vector<string> out;
    split(commandText, sep, out);

    if (out.size() > 1) {
        if (out[0] == "tabsize") {
            try {
                int size = stoi(out[1]);
                if (size > 0 and size < 13) {
                    root->textbox->setTabSize(size);
                }
            }
            catch (const std::exception& e) { return; }
        }
        else if (out[0] == "open") {
            root->openFile(out[1].c_str()); //FILE | root->OpenFile(out[1].c_str());
        }
        else if (out[0] == "save") {
            root->saveFile(out[1].c_str()); //FILE | root->SaveFile(out[1].c_str());
        }
        else if (out[0] == "find") {
            QString pattern = out[1].c_str();
            QRegExp rx = QRegExp(pattern);
            root->textbox->find(rx);
        }
        else {
            return;
        }
    }
    else {
        if (commandText == "quit") {
            std::exit(0);
        }
        else if (commandText == "about"){
            root->aboutScreen();
        }
        else if (commandText == "new"){
            root->NewFile();
        }
        else {
            return;
        }
    }

    setPlaceholderText(commandText.c_str());
}

void MainWindow::CommandLine::completeCommand() {
    clear();
    insert(placeholderText());
}
