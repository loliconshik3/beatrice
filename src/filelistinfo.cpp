#include "filelistinfo.h"

MainWindow::FileListWidget::FileListInfo::FileListInfo(FileListWidget *parent) :
    QLabel(parent)
{
    root = parent->root;
    rootParent = parent;

    setAlignment(Qt::AlignLeft);
    setContentsMargins(2, 0, 0, 0);
    setStyleSheet("QLabel { font-size: 13px; color: #ffffff; border: none; background: #2a2b2e; font-family: Source Code Pro;}");
    setGeometry(0, root->cfg->fileListInfoTopPadding, root->cfg->fileListInfoWidth, root->cfg->fileListInfoHeight);
}
