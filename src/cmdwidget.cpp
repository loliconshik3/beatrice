#include "cmdwidget.h"

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
