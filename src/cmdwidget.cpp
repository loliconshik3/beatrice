#include "cmdwidget.h"
#include "commandline.h"
#include "cmdtext.h"

#include <QGridLayout>
#include <QShortcut>

CmdWidget::CmdWidget(MainWindow *parent) : QWidget()
{
    root = parent;

    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+c"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(killCurrentProcess()));
}

void CmdWidget::killCurrentProcess() {
    if (!cmd->isEnabled()) {
        cmd->killCurrentProcess();
    }
}

void CmdWidget::updateWidgetStyle() {
    //setWindowFlags(Qt::Popup);

    QString style = QString("QWidget { color: %1; border: none; background: %2; }")
                    .arg(root->theme["clipboardFontColor"].c_str(),
                         root->theme["clipboardBackground"].c_str());
    setStyleSheet(style);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(this->cmd, 0, 0);
    layout->addWidget(this->cmdText, 1, 0);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(0);
    setLayout(layout);

    //setGeometry(0, 0, root->cfg->cmdWidgetWidth, root->cfg->cmdWidgetHeight);

    move(root->cfg->cmdWidgetX, root->cfg->cmdWidgetY);
}
