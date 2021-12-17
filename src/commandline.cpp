#include "commandline.h"
#include "cmdlist.h"
#include "textbox.h"
#include "utils.h"

#include <QtWidgets>

using namespace std;

CommandLine::CommandLine(CmdWidget *parent) :
    QLineEdit(parent)
{
    root = parent->root;
    rootParent = parent;

    //#1f1f1f //2e2f30
    //setStyleSheet("QLineEdit { font-size: 15px; color: lightGray; border: none; background: #1f222d; font-family: Source Code Pro; }");

    loadHistory();
    updateWidgetStyle();
    updateShortcuts();
}

void CommandLine::escape() {
    rootParent->hide();
    root->textbox->setFocus();
    clear();
}

void CommandLine::runLastCommand() {
    setText(placeholderText());
    launchCommand();
}

void CommandLine::launchCommand() {
    string commandText = text().toStdString();

    rootParent->hide();
    clear();

    if (commandText == "") {
        log("Empty command field!");
        return;
    }

    char sep = ' ';
    vector<string> out;
    split(commandText, sep, out);
    log("Launch command: " + commandText);

    if (out.size() > 1) {
        if (out[0] == "tabsize") {
            try {
                int size = stoi(out[1]);
                if (size > 0 and size < 13) {
                    root->textbox->setTabSize(size);
                }
            }
            catch (const std::exception& e) { log("Tabsize arg error!"); return; }
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
        else if (out[0] == "syntax") {
            string syntax = out[1];
            root->currentFile->setSyntax(syntax);
            QString text = root->textbox->toPlainText();
            root->textbox->setPlainText(text);
        }
        else {
            log("Command not found!");
            return;
        }
    }
    else {
        replaceStr(commandText, " ", "");
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
            log("Command not found!");
            return;
        }
    }

    log("Command succesfully completed!");

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

    log("Convert out to bash command");
    //log(command);

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

vector<string> CommandLine::splitCommand(string command) {
    char sep = ' ';
    vector<string> out;
    split(command, sep, out);
    return out;
}

void CommandLine::completeArg() {
    if (rootParent->cmdList->count() == 0) {
        insert(" ");
        return;
    }

    //QString text = rootParent->cmdList->currentItem()->text() + QString(" ");
    QString itemText = rootParent->cmdList->currentItem()->text();
    QString text = rootParent->COMMAND_LIST[itemText.toStdString()].c_str() + QString(" ");
    QString prevText = "";

    cursorBackward(true, 1);
    QString prevChar = selectedText();
    cursorForward(true, 1);

    if (prevChar != " ") {
        cursorWordBackward(true);
        prevText = selectedText();
    }

    log("Completing arg: " + prevText.toStdString() + " to: " + text.toStdString());

    insert(text);
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

    setGeometry(0, 0, root->cfg->commandLineWidth, root->cfg->commandLineHeight);

    QFont fnt(root->cfg->commandlineFontFamily.c_str());
    fnt.setPixelSize(root->cfg->commandlineFontSize);
    setFont(fnt);
}

void CommandLine::updateShortcuts() {
    connect(this, &CommandLine::textChanged, this, [this]{ rootParent->cmdList->redrawCommands(); });

    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_launchCommand1), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(launchCommand()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_completeCommand), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(completeCommand()));

    shortcut = new QShortcut(QKeySequence("Up"), this);
    connect(shortcut, &QShortcut::activated, this, [this]{ rootParent->previousItem(); } );

    shortcut = new QShortcut(QKeySequence("Down"), this);
    connect(shortcut, &QShortcut::activated, this, [this]{ rootParent->nextItem(); } );

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_launchCommand2), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(launchCommand()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_cmdEscape), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(escape()));
}
