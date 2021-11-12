#include "filelist.h"
#include "filelistsearch.h"
#include "filelisttext.h"
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

FileList::FileList(FileListWidget *parent) :
    QListWidget(parent)
{
    root = parent->root;
    rootParent = parent;

    QString style = QString("QListWidget { color: %1; border: none; background: %2; }")
                    .arg(root->theme["flistwidgetFontColor"].c_str(),
                         root->theme["flistwidgetBackground"].c_str());
    setStyleSheet(style);

    QFont fnt("Source Code Pro");
    fnt.setPixelSize(16);
    setFont(fnt);

    //setStyleSheet("QListWidget { font-size: 16px; font-family: Source Code Pro; color: lightGray; border: none; background: #2a2b2e; }");

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setGeometry(0, root->cfg->fileListTopPadding, root->cfg->fileListWidth, root->cfg->fileListHeight);

    updateShortcuts();
}

void FileList::currentItemChanged() {
    if (count() > 0) {
        QListWidgetItem *item = currentItem();
        if (item != NULL) {
            QString text = item->text();
            string path = files[text.toStdString()];
            rootParent->flinfo->setText(path.c_str());

            rootParent->fltext->setText(filesText[text.toStdString()].c_str());
        }
        else {
            rootParent->flinfo->setText("");
            rootParent->fltext->setText("");
        }
    }
    else {
        rootParent->flinfo->setText("");
        rootParent->fltext->setText("");
    }
}

void FileList::setFileText(string name, QFile &file) {
    QTextStream in(&file);
    int count = 15;
    while (count > 0 and !in.atEnd())
    {
       QString line = in.readLine();
       filesText[name] += line.toStdString() + "\n";
       --count;
    }
}

void FileList::loadLastFiles() {
    QString homedir = getHomeDir().c_str();
    QString path    = homedir + "/.config/beatrice/cache/lastfile";

    filesText.clear();
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
                string name = line.toStdString();

                if (name == root->currentFile->path) {
                    name += " (Current)";
                }

                replaceStr(name, homedir.toStdString(), "~");

                files[name] = line.toStdString();
                addItem(name.c_str());

                if (loadedFile.open(QIODevice::ReadOnly)) {
                   setFileText(name, loadedFile);
                   loadedFile.close();
                }
            }
        }
        file.close();
    }

    sortItems();
    if (count() > 0) {
        setCurrentItem(item(0));
    }
}

void FileList::loadDirectoryFiles(string path) {
    ifstream myfile;
    string fileName;

    filesText.clear();
    files.clear();
    clear();

    for (const auto & entry : fs::directory_iterator(path)){
        string path = entry.path();
        QFile file(path.c_str());
        //myfile.open(entry.path());

        if ( file.exists() ) { //if ( myfile.is_open()) { // always check whether the file is open
            fileName = entry.path().filename();

            if (entry.is_directory()) {
                fileName = "[" + fileName + "]";
            }

            if (entry.path() == root->currentFile->path) {
                fileName += " (Current)";
            }

            files[fileName] = entry.path();
            addItem(fileName.c_str());

            if ( file.open(QIODevice::ReadOnly) ) {
                setFileText(fileName, file);
            }
        }

        file.close(); //myfile.close();
    }
    fileName        = BACK_NAME;
    files[fileName] = getPathDir(path);
    addItem(fileName.c_str());
    fileName        = PICK_NAME;
    files[fileName] = path;
    addItem(fileName.c_str());

    sortItems();
    setCurrentItem(item(0));
}

void FileList::loadTabFiles() {
    string homedir = getHomeDir();
    string fileName = "";

    filesText.clear();
    files.clear();
    clear();

    for (const auto & file : root->files) {
        fileName = file->path;

        replaceStr(fileName, homedir, "~");

        if (!file->isSaved()) {
            fileName += "*";
        }

        if (file->path == root->currentFile->path) {
            fileName += " (Current)";
        }

        files[fileName] = file->path;
        addItem(fileName.c_str());

        filesText[fileName] = file->text;
    }

    sortItems();
    setCurrentItem(item(0));	
}

void FileList::openFile() {
    if (count() > 0) {
        QString curItem = currentItem()->text();
        string path = files[curItem.toStdString()];

        rootParent->flsearch->clear();

        if (curItem == BACK_NAME.c_str() or curItem.contains("[")) {
            loadDirectoryFiles(path);
        }
        else if (curItem == PICK_NAME.c_str()) {
            root->openFolder(path.c_str());
            rootParent->hide();
        }
        else {
            root->openFile(path.c_str()); //FILE | root->OpenFile(path.c_str());
            rootParent->hide();
        }
    }
    else {
        rootParent->hide();
    }
}

void FileList::deleteFile() {
    if (count() > 0) {
        QString filename = currentItem()->text();
        string path      = files[filename.toStdString()];
        files.erase(filename.toStdString());
        takeItem(currentRow());

        if (filename == BACK_NAME.c_str()) {
            return;
        }

        if (path == root->currentFile->path) {
            log("Close current file: '" + path + "'");

            root->currentFile->removeFileFromList(root->files);
            if (root->files.size() > 0) {
                root->openFile(root->files[0]->path.c_str());
            }
            else {
                root->newFile();
            }
        }

        if (filename.contains("[")) {
            log("Remove directory: '" + path + "'");
            system(("rmdir '" + path + "'").c_str());
        }
        else {
            log("Remove file: '" + path + "'");
            system(("rm '" + path + "'").c_str());
        }
    }
    else {
        rootParent->hide();
    }
}

void FileList::redrawFiles() {
    clear();

    for (const auto & file : files) {
        string fileName    = QString(file.first.c_str()).toLower().toUtf8().constData();
        string searchQuery = rootParent->flsearch->text().toLower().toUtf8().constData();

        if ( fileName.find(searchQuery) != string::npos ) {
            addItem(QString(file.first.c_str()));
        }
    }

    sortItems();
    setCurrentItem(item(0));
    currentItemChanged();
}

void FileList::updateShortcuts() {
    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_listOpenFile1), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openFile()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_listOpenFile2), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openFile()));

    connect(this, &FileList::doubleClicked, this, &FileList::openFile);

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_listDeleteFile), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteFile()));

    connect(this, &FileList::itemSelectionChanged, this, &FileList::currentItemChanged);
}
