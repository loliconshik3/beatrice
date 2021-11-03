#include "mainwindow.h"
#include "infopanel.h"
#include "fileslist.h"
#include "filelistsearch.h"
#include "commandline.h"
#include "textbox.h"
#include "utils.h"

#include <QShortcut>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include <QSettings>

namespace fs = std::filesystem;

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(QSize(cfg->windowMinWidth, cfg->windowMinHeight));
    currentDir = fs::current_path();
    UpdateTitle();

    setStyleSheet("QWidget { background: #1f222d}");//#262728 }");//#2e2f30

    updateShortcuts();
}

void MainWindow::UpdateTitle() {
    string fname = getFilename(filename);
    if (isFileOld()) {
        fname += "*";
    }
    auto windowName = fname + " @ " + getFilename(currentDir) + " - beatrice";

    setWindowTitle(windowName.c_str());
}

void MainWindow::OpenFolder(QString path) {
    QString fileName = path;

    if (path.isEmpty()) {
        path = fs::current_path().c_str();
        fileName = QFileDialog::getExistingDirectory(this,
            tr("Open File"), path);
    }

    if (fileName.isEmpty())
        return;
    else {
        currentDir = fileName.toStdString();
    }
    infopanel->updateText();
    UpdateTitle();
}

void MainWindow::NewFile() {
    log("Create new file");

    textbox->clear();
    filename = "Untitled";
    filetext = "";

    saveLastFile();
    UpdateTitle();
    infopanel->updateText();
    textbox->isNew = true;

    bool contain = false;
    for (const auto & file : files) {
        if (QString(file->path.c_str()).contains(filename.c_str(), Qt::CaseInsensitive)) {
            contain = true;
            break;
        }
    }
    if (!contain) {
        File *file = new File("Untitled", "", currentDir+"/Untitled", currentDir, true);
        files.insert(files.begin(), file);
        currentFile = file;
    }
}

void MainWindow::openFile() {
    if (currentFile->inList(files)) {
        log("work");
    }
}

void MainWindow::OpenFile(QString path) {
    string homedir      = getHomeDir();
    QString testPath    = path;
    bool createNewFile  = false;
    QString fileName    = "";
    string oldPath      = path.toStdString();
    ifstream myfile;
    string line;

    if (path != nullptr) {
        testPath = path.replace("~/", (homedir+"/").c_str());

        QFile pathFile(testPath);
        if (pathFile.exists()) {
            fileName = testPath;
        }
        else {
            if (path.contains(homedir.c_str())) {
                fileName = path;
                createNewFile = true;
            }
            else {
                fileName = currentDir.c_str() + QString("/") + path;
                QFile pathFile(fileName);
                if (not pathFile.exists()) {
                    createNewFile = true;
                }
            }
            cout << fileName.toStdString() << endl;
        }
    }
    else {
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"), homedir.c_str(),
            tr("Code Files (*)"));
    }

    if (fileName.isEmpty())
        return;
    else {
        textbox->clear();

        if (not createNewFile) {
            QFile file(fileName);

            if (file.open(QIODevice::ReadWrite)) {
                filename = fileName.toUtf8().toStdString();
                textbox->setText(file.readAll());
                filetext = textbox->toPlainText().toUtf8().toStdString();
            }

            file.close();
            textbox->isNew = false;
        }
        else {
            QString name = fileName.toUtf8();
            filename = name.toUtf8().toStdString();
            filetext = "";
        }

        currentDir = getPathDir(filename);
        textbox->setFocus();
        infopanel->updateText();
        UpdateTitle();

        File *contFile = NULL;
        bool contain = false;
        for (const auto & file : files) {
            if (QString(file->path.c_str()).contains(filename.c_str(), Qt::CaseInsensitive)) {
                contain = true;
                contFile = file;
                break;
            }
        }
        if (!contain) {
            File *file = new File(getFilename(filename), filetext, filename, currentDir, false);
            files.insert(files.begin(), file);
            contFile = file;
        }

        currentFile = contFile;

        log("Open file '" + filename + "'");
        saveLastFile();
        openFile();
    }
}

void MainWindow::insertSaveCommand(bool saveas) {
    if (textbox->isNew or saveas) {
        commandline->clear();
        commandline->insert("save ");
        commandline->setFocus();
    }
    else {
        SaveFile();
    }
}

void MainWindow::SaveFile(QString name) {
    string homedir = getHomeDir();
    QString fileName;

    if (not name.isEmpty()) {
        fileName = currentDir.c_str() + QString("/") + name;
    }
    else {
        fileName = filename.c_str();
    }

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QTextStream out(&file);
        filename = fileName.toUtf8().toStdString();
        filetext = textbox->toPlainText().toUtf8().toStdString();
        infopanel->updateText();
        UpdateTitle();

        auto text = textbox->toPlainText().toUtf8();

        out << text;

        file.close();

        bool contain = false;
        for (const auto & file : files) {
            if (QString(file->path.c_str()).contains(filename.c_str(), Qt::CaseInsensitive)) {
                contain = true;
                break;
            }
        }
        if (!contain) {
            File *file = new File(filename, filetext, filename, currentDir, true);
            files.insert(files.begin(), file);
            currentFile = file;
        }

        currentFile->savedText = filetext;
        openFile();

        log("Save file '" + filename + "'");
        saveLastFile();
        textbox->isNew = false;
    }
}

void MainWindow::aboutScreen() {
    QMessageBox::information(this, tr("About"),
        tr("beatrice - simple, minimalist code editor written on qt and c++.\n\nby loliconshik3"));
}

void MainWindow::changeFocus() {
    QWidget *focusedWidget = focusWidget();

    if (focusedWidget == commandline) {
        textbox->setFocus();
        return;
    }
    else if (focusedWidget == textbox) {
        commandline->setFocus();
        return;
    }
}

bool MainWindow::isFileOld() {
    if (textbox != 0) {
        string text = textbox->toPlainText().toUtf8().toStdString();

        if (filetext != text) {
            return true;
        }
    }
    return false;
}

void MainWindow::loadLastFile() {
    string homedir = getHomeDir();
    string path = homedir + "/.beatrice/cache/lastfile";
    ifstream myfile;
    string line;

    myfile.open(path);

    if ( myfile.is_open() ) {
        getline(myfile, line);
        QFile file(line.c_str());
        if (file.exists()) {
            OpenFile(line.c_str());
        }
    }
}

void MainWindow::saveLastFile() {
    string homedir = getHomeDir();
    string path = homedir + "/.beatrice/cache/lastfile";

    QFile file(path.c_str());
    if (file.open(QIODevice::ReadWrite)) {
        QString fullText = file.readAll();
        QTextStream out(&file);
        QString name = filename.c_str();

        if (fullText.contains(name, Qt::CaseInsensitive)) {
            file.close();
            return;
        }

        out << name + QString("\n");

        file.close();
    }
}

void MainWindow::showCurrentDirFiles() {
    flwidget->fileslist->loadDirectoryFiles(currentDir);
    flwidget->flsearch->setFocus();
    flwidget->show();
}

void MainWindow::showLastFiles() {
    flwidget->fileslist->loadLastFiles();
    flwidget->flsearch->setFocus();
    flwidget->show();
}

void MainWindow::showTabFiles() {
	flwidget->fileslist->loadTabFiles();
	flwidget->flsearch->setFocus();
	flwidget->show();
}

void MainWindow::openUpFile() {
    if (files.size() > 1) {
        auto it = std::find(files.begin(), files.end(), currentFile);

        if (it != files.end())
        {
            int index = it - files.begin();
            if (index > 0) {
                OpenFile(files[index-1]->path.c_str());
            }
        }
    }
}

void MainWindow::openDownFile() {
    if (files.size() > 1) {
        auto it = std::find(files.begin(), files.end(), currentFile);

        if (it != files.end())
        {
            int index = it - files.begin();
            int size = files.size()-1;
            if (index < size) {
                OpenFile(files[index+1]->path.c_str());
            }
        }
    }
}

void MainWindow::updateShortcuts() {
    QShortcut *shortcut = new QShortcut(QKeySequence(cfg->sct_quit), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(Exit()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_save), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(insertSaveCommand()));//SaveFile()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_saveAs), this);
    connect(shortcut, &QShortcut::activated, this, [this]{ insertSaveCommand(true); });

    shortcut = new QShortcut(QKeySequence(cfg->sct_open), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showCurrentDirFiles()));//OpenFile()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_openFolder), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(OpenFolder()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_new), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(NewFile()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_changeFocus), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(changeFocus()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_about), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(aboutScreen()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_showLastFiles), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showLastFiles()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_showCurrentDir), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showCurrentDirFiles()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_showTabFiles), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showTabFiles()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_openTopFile), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openUpFile()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_openBottomFile), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openDownFile()));
}
