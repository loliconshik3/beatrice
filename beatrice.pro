QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 g++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/clipboard.cpp \
    src/commandline.cpp \
    src/config.cpp \
    src/file.cpp \
    src/filelist.cpp \
    src/filelistinfo.cpp \
    src/filelistsearch.cpp \
    src/filelisttext.cpp \
    src/filelistwidget.cpp \
    src/findtext.cpp \
    src/findwidget.cpp \
    src/infopanel.cpp \
    src/linenumberarea.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/replacetext.cpp \
    src/syntaxhighlighter.cpp \
    src/textbox.cpp \
    src/utils.cpp

HEADERS += \
    src/clipboard.h \
    src/commandline.h \
    src/config.h \
    src/file.h \
    src/filelist.h \
    src/filelistinfo.h \
    src/filelistsearch.h \
    src/filelisttext.h \
    src/filelistwidget.h \
    src/findtext.h \
    src/findwidget.h \
    src/infopanel.h \
    src/linenumberarea.h \
    src/mainwindow.h \
    src/replacetext.h \
    src/syntaxhighlighter.h \
    src/textbox.h \
    src/utils.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
