#include "filelisttext.h"

MainWindow::FileListWidget::FileListText::FileListText(FileListWidget *parent)
    : QTextEdit(parent)
{
    root = parent->root;
    setStyleSheet("QWidget { font-size: 14px; font-family: Source Code Pro; color: lightGray; border: none; background: #2a2b2e; }");
    setReadOnly(true);

    setGeometry(root->cfg->fileListTextLeftPadding, root->cfg->fileListTextTopPadding, root->cfg->fileListTextWidth, root->cfg->fileListTextHeight);
}
