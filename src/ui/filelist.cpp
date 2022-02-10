#include "../../include/ui/filelistinsidelist.h"
#include "../../include/ui/filelistsearch.h"
#include "../../include/ui/filelisttext.h"
#include "../../include/ui/infopanel.h"
#include "../../include/ui/filelist.h"
#include "../../include/ui/textbox.h"

#include "../../include/api/utils.h"

#include <QShortcut>
#include <QDesktopWidget>

#include <QFile>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

FileList::FileList(FileListWidget *parent) :
    QListWidget(parent)
{
    root = parent->root;
    rootParent = parent;

    //setStyleSheet("QListWidget { font-size: 16px; font-family: Source Code Pro; color: lightGray; border: none; background: #2a2b2e; }");

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setGeometry(0, root->cfg->fileListTopPadding, root->cfg->fileListWidth, root->cfg->fileListHeight);

    updateWidgetStyle();
    updateShortcuts();
}

void FileList::currentItemChanged() {
    if (count() > 0) {
        QListWidgetItem *item = currentItem();
        if (item != NULL) {
            QString text = item->text();
            string path = "";

            if (files.find(text.toStdString()) != files.end()) {
                path  = files[text.toStdString()];

                rootParent->flinfo->setText(path.c_str());
                rootParent->fltext->setPlainText(filesText[text.toStdString()].c_str());

                rootParent->flinsideList->hide();
                rootParent->fltext->show();
            }
            if (path != "" && directoryFiles.find(path) != directoryFiles.end()) {
                vector<string> flist = directoryFiles[path];
                rootParent->flinsideList->loadDirectoryFiles(flist);

                rootParent->flinsideList->show();
                rootParent->fltext->hide();
            }
        }
        else {
            rootParent->flinfo->setText("");
            rootParent->fltext->setText("");
            rootParent->flinsideList->clear();
        }
    }
    else {
        QString name = rootParent->flsearch->text();
        QString infoText = "";

        string totalName = name.toStdString();
        replaceStr(totalName, "!", "");
        replaceStr(totalName, "@", "");

        if (name.length() > 0) {
            if (name.at(0) == "!") {
                infoText = QString("Create file: ") + totalName.c_str();
            }
            else if (name.at(0) == "@") {
                infoText = QString("Create directory: ") + totalName.c_str();
            }

            if (currentDirectory == "") {
                infoText = "Can't create any files or directories here!";
            }
        }
        else {
            infoText = "";
        }

        rootParent->flinfo->setText(infoText);
        rootParent->fltext->setText("");
        rootParent->flinsideList->clear();
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

void FileList::setDirectoryFiles(string path) {
    string fileName = "";

    directoryFiles[path] = {};
    vector<string> filesList = {};

    for (const auto & entry : fs::directory_iterator(path)){
        string path = entry.path();
        QFile file(path.c_str());

        if ( file.exists() ) {
            fileName = entry.path().filename();

            if (entry.is_directory()) {
                fileName = "[" + fileName + "]";
            }
            filesList.insert(filesList.end(), fileName);
        }

        file.close();
    }
    directoryFiles[path] = filesList;
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
    openMode = "LAST";
    currentDirectory = "";

    sortItems();
    if (count() > 0) {
        setCurrentItem(item(0));
    }
}

void FileList::loadDirectoryFiles(string path) {
    string fileName;

    filesText.clear();
    files.clear();
    clear();

    for (const auto & entry : fs::directory_iterator(path)){
        string path = entry.path();
        QFile file(path.c_str());

        if ( file.exists() ) { // always check whether the file is open
            fileName = entry.path().filename();

            if (entry.is_directory()) {
                fileName = "[" + fileName + "]";
                try {
                    setDirectoryFiles(path);
                }
                catch (const std::exception& e) { }
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

        file.close();
    }
    if (path != "/") {
        string backPath = getPathDir(path);
        fileName        = BACK_NAME;
        files[fileName] = backPath;
        addItem(fileName.c_str());
        setDirectoryFiles(backPath);
    }
    //==========Add . item==========
    fileName = CURRENT_DIR_NAME;
    files[fileName] = path;
    addItem(fileName.c_str());
    setDirectoryFiles(path);
    //==============================

    openMode = "STANDART";
    currentDirectory = path;

    sortItems();
    setCurrentItem(item(0));
}

void FileList::loadTabFiles() {
    string homedir  = getHomeDir();
    string name     = "";

    filesText.clear();
    files.clear();
    clear();

    for (const auto & file : root->files) {
        name = file->path;

        replaceStr(name, homedir, "~");

        if (!file->isSaved()) {
            name += "*";
        }

        if (file->path == root->currentFile->path) {
            name += " (Current)";
        }

        log(name);

        filesText[name] = file->text;
        files[name]     = file->path;
        addItem(name.c_str());
    }
    openMode = "TAB";
    currentDirectory = "";

    //sortItems();
    setCurrentItem(item(0));	
}

void FileList::openFile() {
    if (count() > 0) {
        QString curItem = currentItem()->text();
        string path = files[curItem.toStdString()];

        rootParent->flsearch->clear();

        if (curItem == BACK_NAME.c_str() or curItem.contains("[") or curItem == CURRENT_DIR_NAME.c_str()) {
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
        if (currentDirectory == "") {
            rootParent->hide();
            return;
        }

        QString name     = rootParent->flsearch->text();
        string dir       = currentDirectory;

        string totalName = name.toStdString();
        replaceStr(totalName, "!", "");
        replaceStr(totalName, "@", "");

        string path      = dir + "/" + totalName;

        rootParent->flsearch->clear();

        if (name.at(0) == "!") {
            string touch = "touch " + path;
            system(touch.c_str());
            root->openFile(path.c_str());
            rootParent->hide();
        }
        else if (name.at(0) == "@") {
            string mkdir = "mkdir " + path;
            system(mkdir.c_str());
            loadDirectoryFiles(path);
        }
        else {
            rootParent->hide();
        }
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

void FileList::closeFile() {
    if (currentDirectory != "" || openMode != "TAB" || count() <= 0) {
        return;
    }

    QString filename = currentItem()->text();
    string path      = files[filename.toStdString()];

    File *curFile = root->currentFile;
    File *fl = new File("", "", path, "", true);
    File *file = fl->getFileInList(root->files);
    root->currentFile = file;
    root->closeCurrentFile();
    root->currentFile = curFile;

    files.erase(filename.toStdString());
    takeItem(currentRow());
}

void FileList::redrawFiles() {
    clear();

    string searchQuery = rootParent->flsearch->text().toLower().toStdString();
    for (const auto & file : files) {
        string fileName    = QString(file.first.c_str()).toLower().toStdString();

        if ( fileName.find(searchQuery) != string::npos ) {
            QString name = file.first.c_str();
            addItem(name);
        }
    }

    //sortItems();
    setCurrentItem(item(0));
    currentItemChanged();
}

void FileList::updateWidgetStyle() {
    QString style = QString("QListWidget { color: %1; border: none; background: %2; }")
                    .arg(root->theme["flistwidgetFontColor"].c_str(),
                         root->theme["flistwidgetBackground"].c_str());
    setStyleSheet(style);

    QFont fnt(root->cfg->filelistFontFamily.c_str());
    fnt.setPixelSize(root->cfg->filelistFontSize);
    setFont(fnt);
}

void FileList::updateShortcuts() {
    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_listOpenFile1), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openFile()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_listOpenFile2), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(openFile()));

    connect(this, &FileList::doubleClicked, this, &FileList::openFile);

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_listDeleteFile), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteFile()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_closeCurrentFile), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(closeFile()));

    connect(this, &FileList::itemSelectionChanged, this, &FileList::currentItemChanged);
}
