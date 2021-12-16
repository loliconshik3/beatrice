#include "cmdwidget.h"
#include "cmdlist.h"

CmdWidget::CmdWidget(MainWindow *parent) : QWidget(parent)
{
    root = parent;

    updateWidgetStyle();
}

void CmdWidget::updateWidgetStyle() {
    setWindowFlags(Qt::Popup);

    setGeometry(0, 0, root->cfg->cmdWidgetWidth, root->cfg->cmdWidgetHeight);

    move(root->cfg->cmdWidgetX, root->cfg->cmdWidgetY);

}

void CmdWidget::previousItem() {
    int itemIndex   = cmdList->currentIndex().row();
    int maxIndex    = 0;

    if (itemIndex > maxIndex) {
        cmdList->setCurrentItem(cmdList->item(itemIndex-1));
    }
    else {
        cmdList->setCurrentItem(cmdList->item(cmdList->count()-1));
    }
}

void CmdWidget::nextItem() {
    int itemIndex   = cmdList->currentIndex().row();
    int maxIndex    = cmdList->count()-1;

    if (itemIndex < maxIndex) {
        cmdList->setCurrentItem(cmdList->item(itemIndex+1));
    }
    else {
        cmdList->setCurrentItem(cmdList->item(0));
    }
}
