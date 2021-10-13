#include "filelistsearch.h"
#include "fileslist.h"

#include <QDesktopWidget>
#include <QShortcut>

MainWindow::FileListWidget::FileListSearch::FileListSearch(FileListWidget *parent) :
    QLineEdit(parent)
{
    root = parent->root;
    rootParent = parent;

    setStyleSheet("QLineEdit { font-size: 16px; font-family: Source Code Pro; color: lightGray; border: none; background: #2a2b2e; }");
    setPlaceholderText("Type to filter...");

    QRect rect = QDesktopWidget().availableGeometry();
    int width = rect.width() / 2;
    setGeometry(0, 0, width, 25);

    connect(this, &FileListSearch::textChanged, this, &FileListSearch::filterList);

    QShortcut *shortcut = new QShortcut(QKeySequence("Down"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(Down()));

    shortcut = new QShortcut(QKeySequence("Up"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(Up()));
}

void MainWindow::FileListWidget::FileListSearch::filterList() {
    rootParent->fileslist->redrawFiles();
}

void MainWindow::FileListWidget::FileListSearch::Up() {
    if (not rootParent->fileslist->isHidden()) {
        int itemIndex = rootParent->fileslist->currentIndex().row();
        auto flist = rootParent->fileslist;
        int maxIndex = 0;

        if (itemIndex > maxIndex) {
           flist->setCurrentItem(flist->item(itemIndex-1));
        }
        else {
            rootParent->fileslist->setCurrentItem(flist->item(flist->count()-1));
        }
    }
}

void MainWindow::FileListWidget::FileListSearch::Down() {
    if (not rootParent->fileslist->isHidden()) {
        int itemIndex = rootParent->fileslist->currentIndex().row();
        int maxIndex = rootParent->fileslist->count()-1;
        auto flist = rootParent->fileslist;

        if (itemIndex < maxIndex) {
           flist->setCurrentItem(flist->item(itemIndex+1));
        }
        else {
            rootParent->fileslist->setCurrentItem(flist->item(0));
        }
    }
}
