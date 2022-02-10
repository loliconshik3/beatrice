#include "../../include/ui/cmdtext.h"

#include <QScrollBar>

CmdText::CmdText(CmdWidget *parent) : QTextEdit(parent)
{
    root = parent->root;
    rootParent = parent;

    setDisabled(true);
    updateWidgetStyle();
}

void CmdText::moveScrollbarToEnd() {
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void CmdText::printCommand(QString command) {
    string homedir = getHomeDir();
    QString curDir = replaceHomeDir(getPathDir(root->currentFile->path)).c_str() + QString("$ ");

    QString totCom = command.replace((getPathDir(root->currentFile->path) + "/").c_str(), "./");
    totCom = totCom.replace((homedir+"/").c_str(), "~/");

    QString totalOut = curDir + totCom + QString("\n");

    insertPlainText(totalOut);
    moveScrollbarToEnd();
}

void CmdText::printOutput(QString out) {
    insertPlainText(out);
    moveScrollbarToEnd();
}

void CmdText::updateWidgetStyle() {
    QString style = QString("QTextEdit { color: %1; border: none; background: %2; }")
                    .arg(root->theme["clipboardFontColor"].c_str(),
                         root->theme["clipboardBackground"].c_str());
    setStyleSheet(style);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QFont fnt(root->cfg->clipboardFontFamily.c_str());
    fnt.setPixelSize(root->cfg->clipboardFontSize);
    setFont(fnt);

    //setGeometry(0, root->cfg->cmdListTopPadding, root->cfg->cmdListWidth, root->cfg->cmdListHeight);
}
