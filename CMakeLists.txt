cmake_minimum_required(VERSION 3.5)

project(beatrice VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    src/ui/clipboard.cpp
    src/ui/cmdtext.cpp
    src/ui/cmdwidget.cpp
    src/ui/commandline.cpp
    src/ui/filelist.cpp
    src/ui/filelistinfo.cpp
    src/ui/filelistinsidelist.cpp
    src/ui/filelistsearch.cpp
    src/ui/filelisttext.cpp
    src/ui/filelistwidget.cpp
    src/ui/findtext.cpp
    src/ui/findwidget.cpp
    src/ui/infopanel.cpp
    src/ui/linenumberarea.cpp
    src/ui/mainwindow.cpp
    src/ui/replacetext.cpp
    src/ui/syntaxhighlighter.cpp
    src/ui/textbox.cpp
    src/main.cpp

    src/api/macros.cpp
    src/api/macroslist.cpp
    src/api/config.cpp
    src/api/file.cpp
    src/api/utils.cpp



    include/ui/clipboard.h
    include/ui/cmdtext.h
    include/ui/cmdwidget.h
    include/ui/commandline.h
    include/ui/filelist.h
    include/ui/filelistinfo.h
    include/ui/filelistinsidelist.h
    include/ui/filelistsearch.h
    include/ui/filelisttext.h
    include/ui/filelistwidget.h
    include/ui/findtext.h
    include/ui/findwidget.h
    include/ui/infopanel.h
    include/ui/linenumberarea.h
    include/api/macros.h
    include/api/macroslist.h
    include/ui/mainwindow.h
    include/ui/replacetext.h
    include/ui/syntaxhighlighter.h
    include/ui/textbox.h

    include/api/config.h
    include/api/macros.h
    include/api/macroslist.h
    include/api/file.h
    include/api/utils.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(beatrice
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET beatrice APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(beatrice SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(beatrice
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(beatrice PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(beatrice PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(beatrice)
endif()
