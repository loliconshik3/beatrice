#include "commandline.h"
#include "textbox.h"
#include "utils.h"

#include <QtWidgets>

CommandLine::CommandLine(MainWindow *parent) :
    QLineEdit(parent)
{
    root = parent;

    //#1f1f1f //2e2f30
    //setStyleSheet("QLineEdit { font-size: 15px; color: lightGray; border: none; background: #1f222d; font-family: Source Code Pro; }");

    loadHistory();
    updateWidgetStyle();
    updateShortcuts();
}

void CommandLine::escape() {
    if (root->focusWidget() == this) {
        root->textbox->setFocus();
        clear();
    }
    else {
        root->textbox->clearSelection();
    }
}

void CommandLine::runLastCommand() {
    setText(placeholderText());
    launchCommand();
}

void CommandLine::launchCommand() {
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
        else if (out[0] == "cmd") {
            string command = outToCommand(out);
            system(command.c_str());
        }
        else {
            return;
        }
    }
    else {
        if (commandText == "quit" or commandText == "exit") {
            std::exit(0);
        }
        else if (commandText == "about"){
            root->aboutScreen();
        }
        else if (commandText == "new"){
            root->newFile();
        }
        else if (commandText == "cfg" or commandText == "config") {
            QString homedir = getHomeDir().c_str();
            root->openFile(homedir + "/.config/beatrice/config.ini");
        }
        else {
            return;
        }
    }

    setPlaceholderText(commandText.c_str());
    saveToHistory();
}

void CommandLine::loadHistory() {
    QString homedir = getHomeDir().c_str();
    QString path    = homedir + "/.config/beatrice/cache/commandHistory";

    history.clear();

    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd())
        {
           QString line = in.readLine();
           if (line != "" && !line.isEmpty()) {
               history.insert(history.end(), line.toStdString());
           }
        }
        file.close();
    }

    if (history.size() > 0) {
        historyIndex = history.size() - 1;
        setPlaceholderText(history[historyIndex].c_str());
        ++historyIndex;
    }
}

void CommandLine::saveToHistory() {
    string homedir  = getHomeDir();
    string path     = homedir + "/.config/beatrice/cache/commandHistory";

    QFile file(path.c_str());
    if (file.open(QIODevice::ReadWrite)) {
        QString text = file.readAll();
        QString name = placeholderText();
        QTextStream out(&file);

        out << name + QString("\n");

        file.close();
    }

    history.insert(history.end(), placeholderText().toStdString());
    historyIndex = history.size();
}

string CommandLine::outToCommand(vector<string> out) {
    string command  = root->cfg->terminalCommand;
    int length      = out.size() - 1;
    int index       = 1;

    while (index <= length) {
        command += out[index] + " ";
        ++index;
    }

    string homedir      = getHomeDir();
    QString newCommand  = command.c_str();
    newCommand          = newCommand.replace("~/", (homedir+"/").c_str());
    newCommand          = newCommand.replace("./", (getPathDir(root->currentFile->path) + "/").c_str());
    newCommand          = newCommand.replace("$dir$", (getPathDir(root->currentFile->path) + "/").c_str());

    command = newCommand.toStdString();

    log(command);

    return command;
}

void CommandLine::previousCommand() {
    int maxIndex = 0;

    if (historyIndex > maxIndex) {
        --historyIndex;
        clear();
        insert(history[historyIndex].c_str());
    }
}

void CommandLine::nextCommand() {
    int maxIndex = history.size() - 1;

    if (historyIndex < maxIndex) {
        ++historyIndex;
        clear();
        insert(history[historyIndex].c_str());
    }
    else {
        historyIndex = maxIndex + 1;
        clear();
    }
}

void CommandLine::completeCommand() {
    clear();
    insert(placeholderText());
}

void CommandLine::updateWidgetStyle() {
    QString style = QString("QLineEdit { color: %1; border: none; background: %2; }")
                    .arg(root->theme["commandlineFontColor"].c_str(),
                         root->theme["commandlineBackground"].c_str());
    setStyleSheet(style);

    QFont fnt(root->cfg->commandlineFontFamily.c_str());
    fnt.setPixelSize(root->cfg->commandlineFontSize);
    setFont(fnt);
}

void CommandLine::updateShortcuts() {
    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_launchCommand1), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(launchCommand()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_completeCommand), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(completeCommand()));

    shortcut = new QShortcut(QKeySequence("Up"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(previousCommand()));

    shortcut = new QShortcut(QKeySequence("Down"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(nextCommand()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_launchCommand2), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(launchCommand()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_cmdEscape), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(escape()));
}
