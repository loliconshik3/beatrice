#include "../../include/ui/filelistsearch.h"
#include "../../include/ui/filelist.h"

#include <QDesktopWidget>
#include <QShortcut>

FileListSearch::FileListSearch(FileListWidget *parent) :
    QLineEdit(parent)
{
    root = parent->root;
    rootParent = parent;

    //setStyleSheet("QLineEdit { font-size: 16px; font-family: Source Code Pro; color: lightGray; border: none; background: #2a2b2e; }");
    setPlaceholderText("Type to filter...");

    setGeometry(0, 0, root->cfg->fileListSearchWidth, root->cfg->fileListSearchHeight);

    updateWidgetStyle();
    updateShortcuts();
}

void FileListSearch::filterList() {
    rootParent->filelist->redrawFiles();
}

void FileListSearch::Up() {
    if (not rootParent->filelist->isHidden()) {
        int itemIndex   = rootParent->filelist->currentIndex().row();
        auto flist      = rootParent->filelist;
        int maxIndex    = 0;

        if (itemIndex > maxIndex) {
            flist->setCurrentItem(flist->item(itemIndex-1));
        }
        else {
            flist->setCurrentItem(flist->item(flist->count()-1));
        }
    }
}

void FileListSearch::Down() {
    if (not rootParent->filelist->isHidden()) {
        int itemIndex   = rootParent->filelist->currentIndex().row();
        int maxIndex    = rootParent->filelist->count()-1;
        auto flist      = rootParent->filelist;

        if (itemIndex < maxIndex) {
           flist->setCurrentItem(flist->item(itemIndex+1));
        }
        else {
            flist->setCurrentItem(flist->item(0));
        }
    }
}

void FileListSearch::updateWidgetStyle() {
    QString style = QString("QLineEdit { color: %1; border: none; background: %2; }")
                    .arg(root->theme["flsearchFontColor"].c_str(),
                         root->theme["flsearchBackground"].c_str());
    setStyleSheet(style);

    QFont fnt("Source Code Pro");
    fnt.setPixelSize(16);
    setFont(fnt);
}

void FileListSearch::updateShortcuts() {
    connect(this, &FileListSearch::textChanged, this, &FileListSearch::filterList);

    QShortcut *shortcut = new QShortcut(QKeySequence(root->cfg->sct_listMoveDown), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(Down()));

    shortcut = new QShortcut(QKeySequence(root->cfg->sct_listMoveUp), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(Up()));
}
