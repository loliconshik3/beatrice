QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 g++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/commandline.cpp \
    src/config.cpp \
    src/file.cpp \
    src/filelistsearch.cpp \
    src/filelistwidget.cpp \
    src/fileslist.cpp \
    src/infopanel.cpp \
    src/linenumberarea.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/syntaxhighlighter.cpp \
    src/textbox.cpp \
    src/utils.cpp

HEADERS += \
    src/commandline.h \
    src/config.h \
    src/file.h \
    src/filelistsearch.h \
    src/filelistwidget.h \
    src/fileslist.h \
    src/infopanel.h \
    src/linenumberarea.h \
    src/mainwindow.h \
    src/syntaxhighlighter.h \
    src/textbox.h \
    src/utils.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
