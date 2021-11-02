#include "fileslist.h"
#include "filelistsearch.h"
#include "infopanel.h"
#include "textbox.h"
#include "utils.h"

#include <QShortcut>
#include <QDesktopWidget>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <QFile>

namespace fs = std::filesystem;
using namespace std;

MainWindow::FileListWidget::FilesList::FilesList(FileListWidget *parent) :
    QListWidget(parent)
{
    root = parent->root;
    rootParent = parent;

    setStyleSheet("QListWidget { font-size: 16px; font-family: Source Code Pro; color: lightGray; border: none; background: #2a2b2e; }");

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setGeometry(0, root->cfg->fileListTopPadding, root->cfg->fileListWidth, root->cfg->fileListHeight);

    QShortcut *shortcut = new QShortcut(QKeySequence("Return"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openFile()));
    shortcut = new QShortcut(QKeySequence("Enter"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openFile()));
    connect(this, &FilesList::doubleClicked, this, &FilesList::openFile);

    shortcut = new QShortcut(QKeySequence("Ctrl+Delete"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteFile()));
}

void MainWindow::FileListWidget::FilesList::loadLastFiles() {
    QString homedir = getHomeDir().c_str();
    QString path = homedir + "/.beatrice/cache/lastfile";

    files.clear();
    clear();

    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd())
        {
           QString line = in.readLine();

           QFile loadedFile(line);
           if (loadedFile.exists()) {
               files[line.toStdString()] = line.toStdString();
               addItem(line);
           }
        }
        file.close();
    }

    sortItems();
    if (count() > 0) {
        setCurrentItem(item(0));
    }
}

void MainWindow::FileListWidget::FilesList::loadDirectoryFiles(string path) {
    ifstream myfile;
    string fileName;
    string line;

    //string curdir = root->currentDir;
    files.clear();
    clear();

    //string path = curdir;
        for (const auto & entry : fs::directory_iterator(path)){
            myfile.open(entry.path());

            if ( myfile.is_open()) { // always check whether the file is open
                fileName = entry.path().filename();

                if (entry.is_directory()) {
                    fileName = "[" + fileName + "]";
                }

                files[fileName] = entry.path();
                addItem(fileName.c_str());
            }

            myfile.close();
        }
        fileName = BACK_NAME;
        files[fileName] = getPathDir(path);
        addItem(fileName.c_str());

    sortItems();
    setCurrentItem(item(0));
}

void MainWindow::FileListWidget::FilesList::loadTabFiles() {
    string fileName = "";
    files.clear();
    clear();

    for (const File & file : root->files) {
        fileName = file.path;

        files[fileName] = file.path;
        addItem(fileName.c_str());
    }

    sortItems();
    setCurrentItem(item(0));	
}

void MainWindow::FileListWidget::FilesList::openFile() {
    if (count() > 0) {
        QString curItem = currentItem()->text();
        string path = files[curItem.toStdString()];

        if (curItem == BACK_NAME.c_str() or curItem.contains("[")) {
            rootParent->flsearch->clear();
            //root->OpenFolder(path.c_str());
            loadDirectoryFiles(path);
        }
        else {
            root->OpenFile(path.c_str());
            rootParent->hide();
        }
    }
}

void MainWindow::FileListWidget::FilesList::deleteFile() {
    if (count() > 0) {
        QString filename = currentItem()->text();
        string path = files[filename.toStdString()];
        files.erase(filename.toStdString());
        takeItem(currentRow());

        if (path == root->filename) {
            //root->textbox->isNew = true;
            root->filetext = "";
            root->infopanel->updateText();
        }

        if (filename == BACK_NAME.c_str() or filename.contains("[")) {
            system(("rmdir '" + path + "'").c_str());
        }
        else {
            system(("rm '" + path + "'").c_str());
        }
    }
    else {
        rootParent->hide();
    }
}

void MainWindow::FileListWidget::FilesList::redrawFiles() {
    clear();

    for (const auto & file : files) {
        std::string fileName = QString(file.first.c_str()).toLower().toUtf8().constData();
        std::string searchQuery = rootParent->flsearch->text().toLower().toUtf8().constData();

        if ( fileName.find(searchQuery) != std::string::npos ) {
            addItem(QString(file.first.c_str()));
        }
    }

    sortItems();
    setCurrentItem(item(0));
}
