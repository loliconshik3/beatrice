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
    setMinimumSize(QSize(1000, 600));
    currentDir = fs::current_path();
    currentFile = File("Untitled", "", currentDir+"/Untitled", getFilename(currentDir), true);
    //setMaximumSize(QSize(1000, 600));

    setStyleSheet("QWidget { background: #1f222d}");//#262728 }");//#2e2f30

    updateShortcuts();
}

void MainWindow::UpdateTitle() {
    /*string fname = getFilename(filename);
    if (isFileOld()) {
        fname += "*";
    }
    auto windowName = fname + " @ " + getFilename(currentDir) + " - beatrice";

    setWindowTitle(windowName.c_str());*/

    string fname = currentFile.name;
    /*if (!currentFile->isSaved()) {
        fname += "*";
    }*/
    auto windowName = fname + " @ " + currentFile.directory + " - beatrice";
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
    textbox->clear();
    filename = "Untitled";
    filetext = "";

    UpdateTitle();
    infopanel->updateText();
    saveLastFile();
    textbox->isNew = true;

    bool contain = false;
    for (const File & file : files) {
        if (QString(file.path.c_str()).contains(filename.c_str(), Qt::CaseInsensitive)) {
            contain = true;
            break;
        }
    }
    if (!contain) {
        File file("Untitled", "", currentDir+"/Untitled", currentDir, true);
        files.insert(files.begin(), file);
        currentFile = file;
    }
}

void MainWindow::openFile(QString path) {
    string homedir = getHomeDir();
    bool createNewFile = false;

    QString fileDir = "";
    QString fileName = "";
    QString fileText = "";
    QString filePath = path;

    if (path != nullptr) {
        filePath = path.replace("~/", (homedir+"/").c_str());

        QFile pathFile(filePath);
        if (!pathFile.exists()) {
            if (path.contains(homedir.c_str())) {
                filePath = path;
                createNewFile = true;
            }
            else {
                filePath = currentDir.c_str() + QString("/") + path;
                QFile pathFile(filePath);
                if (!pathFile.exists()) {
                    createNewFile = true;
                }
            }
            cout << filePath.toStdString() << endl;
        }
        fileDir = getPathDir(filePath.toStdString()).c_str();

        textbox->clear();

        if (!createNewFile) {
            QFile file(filePath);

            File fl = File(fileName.toStdString(), fileText.toStdString(), filePath.toStdString(), fileDir.toStdString(), false);

            if (containFile(files, fl)) {
                fl = getContainedFile(files, fl);
                textbox->setText(fl.text.c_str());
                currentFile = fl;

                textbox->setFocus();
                infopanel->updateText();
                UpdateTitle();

                saveLastFile();
                return;
            }
            else if (file.open(QIODevice::ReadWrite)) { // always check whether the file is open
                textbox->clear();
                fileName = getFilename(filePath.toStdString()).c_str();
                textbox->setText(file.readAll());
                fileText = textbox->toPlainText().toUtf8();
                fileDir = getFilename(getPathDir(filePath.toStdString())).c_str();
            }

            file.close();
        }
        else {
            textbox->clear();
            fileName = fileName.toUtf8();
            fileText = "";
        }

        File contFile = File(fileName.toStdString(), fileText.toStdString(), filePath.toStdString(), fileDir.toStdString(), createNewFile);
        bool contain = containFile(files, contFile);

        if (!contain) {
            files.insert(files.begin(), contFile);
        }

        currentFile = contFile;

        cout << contFile.path << endl;
        cout << currentFile.name << endl;

        textbox->setFocus();
        infopanel->updateText();
        UpdateTitle();

        saveLastFile();
    }
}

void MainWindow::OpenFile(QString path) {
    string homedir = getHomeDir();
    QString testPath = path;
    bool createNewFile = false;
    QString fileName = "";
    string oldPath = path.toStdString();
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

            if (file.open(QIODevice::ReadWrite)) { // always check whether the file is open
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

        File contFile;
        bool contain = false;
        for (const File & file : files) {
            if (QString(file.path.c_str()).contains(filename.c_str(), Qt::CaseInsensitive)) {
                contain = true;
                contFile = file;
                break;
            }
        }
        if (!contain) {
            File file(getFilename(filename), filetext, filename, currentDir, false);
            files.insert(files.begin(), file);
            contFile = file;
        }

        currentFile = contFile;

        saveLastFile();
    }
}

void MainWindow::insertSaveCommand(bool saveas) {
    if (currentFile.isNew or saveas) { //textbox->isNew or saveas
        commandline->clear();
        commandline->insert("save ");
        commandline->setFocus();
    }
    else {
        saveFile(); //SaveFile();
    }
}

void MainWindow::saveFile(QString path) {
    string homedir = getHomeDir();

    QString fileName = "";
    QString filePath = path;

    if (!path.isEmpty()) {
        filePath = currentDir.c_str() + QString("/") + path;
        fileName = path;
    }
    else {
        filePath = currentFile.path.c_str();
        fileName = currentFile.name.c_str();
    }

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QTextStream out(&file);
        currentFile.name = fileName.toUtf8().toStdString();
        currentFile.text = textbox->toPlainText().toUtf8().toStdString();
        currentFile.path = filePath.toUtf8().toStdString();
        currentFile.directory = getPathDir(currentFile.path);
        currentFile.savedText = currentFile.text;
        infopanel->updateText();
        UpdateTitle();

        out << currentFile.text.c_str();

        file.close();
        saveLastFile();
    }
}

void MainWindow::SaveFile(QString name) {
    string homedir = getHomeDir();
    QString fileName;

    if (not name.isEmpty()) {
        fileName = currentDir.c_str() + QString("/") + name;
    }
    else {
        /*if (textbox->isNew) {
            QString path = filename.c_str();
            if (not path.contains(homedir.c_str())) {
                path = currentDir.c_str() + QString("/") + path;
            }

            fileName = QFileDialog::getSaveFileName(this,
                tr("Save File"), path,
                tr("All Files (*)"));
        }
        else {*/
        fileName = filename.c_str();
        //}
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

        return false;
    }
    return false;
}

void MainWindow::loadLastFile() {
    string homedir = getHomeDir();
    string path = homedir + "/.beatrice/cache/lastfile";
    ifstream myfile;
    string line;

    myfile.open(path);

    /*QFile file(path.c_str());
    if (file.open(QIODevice::ReadOnly)) {
        QString text = file.readAll();
    }*/

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
            cout << "contain" << endl;
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
                OpenFile(files[index-1].path.c_str());
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
                OpenFile(files[index+1].path.c_str());
            }
        }
    }
}

void MainWindow::updateShortcuts() {
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+q"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(Exit()));

    shortcut = new QShortcut(QKeySequence("Ctrl+s"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(insertSaveCommand()));//SaveFile()));

    shortcut = new QShortcut(QKeySequence("Ctrl+Shift+s"), this);
    connect(shortcut, &QShortcut::activated, this, [this]{ insertSaveCommand(true); });

    shortcut = new QShortcut(QKeySequence("Ctrl+o"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showCurrentDirFiles()));//OpenFile()));

    shortcut = new QShortcut(QKeySequence("Ctrl+k"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(OpenFolder()));

    shortcut = new QShortcut(QKeySequence("Ctrl+n"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(NewFile()));

    shortcut = new QShortcut(QKeySequence("Alt+e"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(changeFocus()));

    shortcut = new QShortcut(QKeySequence("Ctrl+i"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(aboutScreen()));

    shortcut = new QShortcut(QKeySequence("Alt+s"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showLastFiles()));

    shortcut = new QShortcut(QKeySequence("Ctrl+`"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showCurrentDirFiles()));

    shortcut = new QShortcut(QKeySequence("Ctrl+p"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showTabFiles()));

    shortcut = new QShortcut(QKeySequence("Alt+Up"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openUpFile()));

    shortcut = new QShortcut(QKeySequence("Alt+Down"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openDownFile()));
}
