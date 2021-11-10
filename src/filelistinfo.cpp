#include "filelistinfo.h"

FileListInfo::FileListInfo(FileListWidget *parent) :
    QLabel(parent)
{
    root = parent->root;
    rootParent = parent;

    setAlignment(Qt::AlignLeft);
    setContentsMargins(2, 0, 0, 0);
    setStyleSheet("QLabel { font-size: 13px; color: #1f222d; border: none; background: lightGray; font-family: Source Code Pro;}");
    setGeometry(0, root->cfg->fileListInfoTopPadding, root->cfg->fileListInfoWidth, root->cfg->fileListInfoHeight);
}
