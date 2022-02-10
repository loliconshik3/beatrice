#include "../../include/ui/filelisttext.h"

FileListText::FileListText(FileListWidget *parent)
    : QTextEdit(parent)
{
    root = parent->root;

    //setStyleSheet("QTextEdit { font-size: 14px; font-family: Source Code Pro; color: lightGray; border: none; background: #3a3b3e; }");
    setReadOnly(true);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setGeometry(root->cfg->fileListTextLeftPadding, root->cfg->fileListTextTopPadding, root->cfg->fileListTextWidth, root->cfg->fileListTextHeight);

    updateWidgetStyle();
}

void FileListText::updateWidgetStyle() {
    QString style = QString("QTextEdit { color: %1; border: none; background: %2; }")
                    .arg(root->theme["fltextFontColor"].c_str(),
                         root->theme["fltextBackground"].c_str());
    setStyleSheet(style);

    QFont fnt("Source Code Pro");
    fnt.setPixelSize(14);
    setFont(fnt);
}
