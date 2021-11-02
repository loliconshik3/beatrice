#ifndef CONFIG_H
#define CONFIG_H

#include <QDesktopWidget>
#include <QRect>

#include <string>
using namespace std;

class Config
{
public:

    QRect rect = QDesktopWidget().availableGeometry();

    int windowMinWidth = 1000;
    int windowMinHeight = 600;

    int fileListSearchWidth = rect.width() / 2;
    int fileListSearchHeight = 25;

    int fileListWidgetWidth = rect.width() / 2;
    int fileListWidgetHeight = rect.height() / 3;
    int fileListWidgetX = ( rect.width() - fileListWidgetWidth ) / 2;
    int fileListWidgetY = ( rect.height() - fileListWidgetHeight ) / 2;

    int fileListTopPadding = 25;
    int fileListWidth = rect.width() / 2;
    int fileListHeight = rect.height() / 3 - fileListTopPadding;

    string infoPanelSeparator = " | ";

    Config();
};

#endif // CONFIG_H
