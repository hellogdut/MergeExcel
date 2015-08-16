#-------------------------------------------------
#
# Project created by QtCreator 2015-08-09T22:23:25
#
#-------------------------------------------------

QT       += core gui
CONFIG += C++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mergeExcel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    person.cpp \
    customedit.cpp

HEADERS  += mainwindow.h \
    person.h \
    customedit.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
