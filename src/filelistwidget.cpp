#include "filelistwidget.h"
#include "filelistsearch.h"

#include <QDesktopWidget>

MainWindow::FileListWidget::FileListWidget(MainWindow *parent) : QWidget(parent)
{
    root = parent;
    setStyleSheet("QWidget { font-size: 16px; font-family: Source Code Pro; color: lightGray; border: none; background: #2a2b2e; }");
    setWindowFlags(Qt::Popup);

    QRect rect = QDesktopWidget().availableGeometry();
    int width = rect.width() / 2;
    int height = rect.height() / 3;
    setGeometry(0, 0, width, height);

    int x = (rect.width()-width)/2;
    int y = (rect.height()-height)/2;
    move(x, y);
}

void MainWindow::FileListWidget::closeEvent(QCloseEvent *e) {
    flsearch->clear();
}

void MainWindow::FileListWidget::hideEvent(QHideEvent *event) {
    flsearch->clear();
}
