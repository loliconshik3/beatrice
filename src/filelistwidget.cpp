#include "filelistwidget.h"
#include "filelistsearch.h"
#include "utils.h"

#include <QDesktopWidget>
#include <QHideEvent>

MainWindow::FileListWidget::FileListWidget(MainWindow *parent) : QWidget(parent)
{
    root = parent;
    setStyleSheet("QWidget { font-size: 16px; font-family: Source Code Pro; color: lightGray; border: none; background: #2a2b2e; }");
    setWindowFlags(Qt::Popup);

    setGeometry(0, 0, root->cfg->fileListWidgetWidth, root->cfg->fileListWidgetHeight);

    move(root->cfg->fileListWidgetX, root->cfg->fileListWidgetY);
}

void MainWindow::FileListWidget::closeEvent(QCloseEvent *e) {
    flsearch->clear();
    log("Close file list widget"); e->type();
}

void MainWindow::FileListWidget::hideEvent(QHideEvent *event) {
    flsearch->clear();
    log("Hide file list widget"); event->type();
}
