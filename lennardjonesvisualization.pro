QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QT += charts
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    exportmanager.cpp \
    fitnessgraph.cpp \
    jdeworker.cpp \
    main.cpp \
    mainwindow.cpp \
    settingsmanager.cpp

HEADERS += \
    ColorPicker.h \
    autoexportdialog.h \
    exportmanager.h \
    fitnessgraph.h \
    jde.h \
    jdeworker.h \
    mainwindow.h \
    settingsmanager.h \
    utils.h \
    x.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
