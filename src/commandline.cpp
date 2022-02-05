#include "commandline.h"
#include "cmdtext.h"
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

void CommandLine::bashCommandError(QProcess::ProcessError) {
    QProcess *process = qobject_cast<QProcess *>(sender());

    QString stderr = process->readAllStandardError();

    if (!stderr.isEmpty()) {
        rootParent->cmdText->printOutput(stderr);
    }

    this->setDisabled(false);
    this->setFocus();
}

void CommandLine::printOutput(QProcess *pr) {
    QString stdout = pr->readAllStandardOutput();

    if (!stdout.isEmpty()) {
        rootParent->cmdText->printOutput(stdout);
    }
}

void CommandLine::bashCommandFinished(int, QProcess::ExitStatus) {
    QProcess *process = qobject_cast<QProcess *>(sender());

    printOutput(process);

    this->setDisabled(false);
    this->setFocus();
}

void CommandLine::killCurrentProcess() {
    QProcess *process = currentProcess;

    printOutput(process);

    process->kill();
    this->setDisabled(false);
    this->setFocus();
}

void CommandLine::launchCommand() {
    string commandText = text().toStdString();
    bool isBashCommand = false;

    //rootParent->hide();
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
        else if (out[0] == "syntax") {
            string syntax = out[1];
            root->currentFile->setSyntax(syntax);
            QString text = root->textbox->toPlainText();
            root->textbox->setPlainText(text);
        }
        else {
            log("Command not found!");
            isBashCommand = true;
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
        else if (commandText == "clear"){
            rootParent->cmdText->clear();
        }
        else if (commandText == "cfg" or commandText == "config") {
            QString homedir = getHomeDir().c_str();
            root->openFile(homedir + "/.config/beatrice/config.ini");
        }
        else {
            log("Command not found!");
            isBashCommand = true;
        }
    }

    if (isBashCommand) {
        string command = outToCommand(out);

        rootParent->cmdText->printCommand(command.c_str());

        this->setDisabled(true);
        currentProcess = new QProcess(this);
        currentProcess->setWorkingDirectory(getPathDir(root->currentFile->path).c_str());
        connect(currentProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(bashCommandFinished(int, QProcess::ExitStatus)));
        connect(currentProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(bashCommandError(QProcess::ProcessError)));
        currentProcess->start(command.c_str());
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
    string command  = "";//root->cfg->terminalCommand;
    int length      = out.size() - 1;
    int index       = 0;

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

void CommandLine::completeCommand() {
    clear();
    insert(placeholderText());
}

void CommandLine::updateWidgetStyle() {
    QString style = QString("QLineEdit { color: %1; border: none; background: %2; }")
                    .arg(root->theme["commandlineFontColor"].c_str(),
                         root->theme["commandlineBackground"].c_str());
    setStyleSheet(style);

    //setGeometry(0, 0, root->cfg->commandLineWidth, root->cfg->commandLineHeight);

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
    connect(shortcut, &QShortcut::activated, this, [this]{ previousCommand(); } );

    shortcut = new QShortcut(QKeySequence("Down"), this);
    connect(shortcut, &QShortcut::activated, this, [this]{ nextCommand(); } );

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_launchCommand2), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(launchCommand()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_cmdEscape), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(escape()));
}
