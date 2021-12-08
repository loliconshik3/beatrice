#include "filelistinsidelist.h"

FileListInsideList::FileListInsideList(FileListWidget *parent) : QListWidget(parent)
{
    root = parent->root;
    rootParent = parent;
    setEnabled(false);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setGeometry(root->cfg->fileListTextLeftPadding, root->cfg->fileListTextTopPadding, root->cfg->fileListTextWidth, root->cfg->fileListTextHeight);
    updateWidgetStyle();
}

void FileListInsideList::loadDirectoryFiles(vector<string> files) {
    clear();
    for (const auto &file : files) {
        addItem(file.c_str());
    }
    sortItems();
    setCurrentItem(item(0));
}

void FileListInsideList::updateWidgetStyle() {
    QString style = QString("QListWidget { color: %1; border: none; background: %2; }")
                    .arg(root->theme["fltextFontColor"].c_str(),
                         root->theme["fltextBackground"].c_str());
    setStyleSheet(style);

    QFont fnt(root->cfg->filelistFontFamily.c_str());
    fnt.setPixelSize(root->cfg->filelistFontSize);
    setFont(fnt);
}
