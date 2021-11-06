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
#include <QClipboard>

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
    string path = fs::current_path();
    currentFile = new File("Untitled", "", path+"/Untitled", getFilename(path), true);
    setMinimumSize(QSize(cfg->windowMinWidth, cfg->windowMinHeight));
    currentDir = fs::current_path();
    UpdateTitle();

    setStyleSheet("QWidget { background: #1f222d}");//#262728 }");//#2e2f30

    updateShortcuts();
}

void MainWindow::UpdateTitle() {
    string fname = currentFile->name;
    if (!currentFile->isSaved()) {
        fname += "*";
    }
    auto windowName = fname + " @ " + currentFile->directory + " - beatrice";

    setWindowTitle(windowName.c_str());

    /*FILE | string fname = getFilename(filename);
    if (isFileOld()) {
        fname += "*";
    }
    auto windowName = fname + " @ " + getFilename(currentDir) + " - beatrice";

    setWindowTitle(windowName.c_str());*/
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
        currentDirectory = fileName.toStdString();
    }
    infopanel->updateText();
    UpdateTitle();
}

void MainWindow::newFile() {
    log("Create new file");
    string fdir = currentDirectory;
    string name = "Untitled" + to_string(cfg->newFileCount);
    ++cfg->newFileCount;

    File *file = new File(name, "", fdir+"/"+name, getFilename(fdir), true);
    files.insert(files.begin(), file);
    updateCurrentFile(file);
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

void MainWindow::updateCurrentFile(File *file) {
    currentFile = file;
    textbox->setText(currentFile->text.c_str());
    UpdateTitle();
    infopanel->updateText();
}

void MainWindow::openFile(QString path) {
    File *file          = new File("Untitled", "", path.toStdString(), "", true);
    string homedir      = getHomeDir();
    QString testPath    = path;
    bool createNewFile  = false;

    QString filePath    = "";
    QString fileText    = "";

    if (file->inList(files)) {
        File *fl = file->getFileInList(files);
        int pos = fl->cursorPosition;
        updateCurrentFile(fl);

        QTextCursor cursor = textbox->textCursor();
        cursor.setPosition(pos);
        textbox->setTextCursor(cursor);
    }
    else {
        testPath = path.replace("~/", (homedir+"/").c_str());

        QFile pathFile(testPath);
        if (pathFile.exists()) {
            filePath = testPath;
        }
        else {
            if (path.contains(homedir.c_str())) {
                filePath = path;
                createNewFile = true;
            }
            else {
                string dir = fs::current_path();
                filePath = dir.c_str() + QString("/") + path;
                QFile pathFile(filePath);
                if (!pathFile.exists()) {
                    createNewFile = true;
                }
            }
            cout << filePath.toStdString() << endl;
        }

        QFile fl(filePath);
        if (fl.open(QIODevice::ReadOnly)) {
            fileText = fl.readAll();
        }

        string fpath = filePath.toStdString();
        string ftext = fileText.toStdString();
        string fname = getFilename(fpath);
        string fdir  = getFilename(getPathDir(fpath));
        bool fisNew  = createNewFile;

        if (fname.find("Untitled") != string::npos) {
            fisNew = false;
        }

        if (files.size() == 1 && files[0]->name.find("Untitled") != string::npos && files[0]->text == "") {
            files[0]->removeFileFromList(files);
        }

        File *file = new File(fname, ftext, fpath, fdir, fisNew);
        files.insert(files.begin(), file);
        updateCurrentFile(file);
        saveLastFile();
    }
}

void MainWindow::saveFile(QString path) {
    string homedir = getHomeDir();
    QString fileName;

    log(currentFile->path);
    if (!path.isEmpty()) {
        fileName = getPathDir(currentFile->path).c_str() + QString("/") + path;
    }
    else {
        fileName = currentFile->path.c_str();
    }

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);

            string fname = getFilename(fileName.toStdString());
            string ftext = textbox->toPlainText().toStdString();
            string fpath = fileName.toStdString();
            string fdir = getFilename(getPathDir(fpath));
            string fsavedText = currentFile->text;

            auto text = textbox->toPlainText().toUtf8();
            out << text;
            file.close();

            if (!currentFile->inList(files)) {
                File *file = new File(fname, ftext, fpath, fdir, false);
                files.insert(files.begin(), file);
                updateCurrentFile(file);
            }
            else {
                currentFile->name       = fname;
                currentFile->savedText  = fsavedText;
                currentFile->isNew      = false;
                currentFile->path       = fpath;
                currentFile->directory  = fdir;
                UpdateTitle();
                infopanel->updateText();
            }

            log("Save file '" + currentFile->path + "'");
            saveLastFile();
        }
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
    }
}

void MainWindow::insertSaveCommand(bool saveas) {
    if (currentFile->isNew or saveas) { //FILE | textbox->isNew or saveas
        log("Insert save command in to cmd");
        commandline->clear();
        commandline->insert("save ");
        commandline->setFocus();
    }
    else {
        saveFile();
    }
}

void MainWindow::insertFindCommand() {
    log("Insert find command in to cmd");
    commandline->clear();
    commandline->insert("find ");
    commandline->setFocus();
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

void MainWindow::closeCurrentFile() {
    currentFile->removeFileFromList(files);
    if (files.size() > 0) {
        openFile(files[0]->path.c_str());
    }
    else {
        newFile();
    }
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
        QString name = currentFile->path.c_str(); //FILE | QString name = filename.c_str();

        if (fullText.contains(name, Qt::CaseInsensitive)) {
            file.close();
            return;
        }

        out << name + QString("\n");

        file.close();
    }
}

void MainWindow::showCurrentDirFiles() {
    flwidget->fileslist->loadDirectoryFiles(currentDirectory);
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
                openFile(files[index-1]->path.c_str()); //FILE | OpenFile(files[index-1]->path.c_str());
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
                openFile(files[index+1]->path.c_str()); //FILE | OpenFile(files[index+1]->path.c_str());
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
    connect(shortcut, SIGNAL(activated()), this, SLOT(showCurrentDirFiles()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_new), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(newFile())); //FILE | NewFile()

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

    shortcut = new QShortcut(QKeySequence(cfg->sct_closeCurrentFile), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(closeCurrentFile()));

    shortcut = new QShortcut(QKeySequence(cfg->sct_find), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(insertFindCommand()));
}
