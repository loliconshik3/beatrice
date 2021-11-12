#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "commandline.h"
#include "mainwindow.h"
#include "textbox.h"

#include <QApplication>
#include <QListWidget>
#include <QClipboard>

#include <vector>

class Clipboard : public QListWidget
{
    Q_OBJECT
private:
    MainWindow *root;
    QClipboard *clipboard = QApplication::clipboard();
    vector<QString> clipboardList;

private slots:
    void clipboardDataChanged();
    void insertClipboardText();

public:
    Clipboard(MainWindow *parent=nullptr);
    void updateWidgetStyle();

public slots:
    void loadClipboardList();
};

#endif // CLIPBOARD_H
