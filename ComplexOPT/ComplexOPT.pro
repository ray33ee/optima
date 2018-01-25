#-------------------------------------------------
#
# Project created by QtCreator 2017-08-31T23:39:02
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ComplexOPT
TEMPLATE = app

RC_FILE = icon.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    drawdialog.cpp \
    parseformula.cpp

HEADERS  += mainwindow.h \
    complexview.h \
    drawdialog.h \
    parseformula.h

FORMS    += mainwindow.ui \
    drawdialog.ui

RESOURCES += \
    icons.qrc

DISTFILES += \
    info.txt \
    icon.rc \
    changelog.txt
