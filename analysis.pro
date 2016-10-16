#-------------------------------------------------
#
# Project created by QtCreator 2014-08-25T14:12:11
#
#-------------------------------------------------

QT       += core gui
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = analysis
TEMPLATE = app


SOURCES += main.cpp\
        hello.cpp \
    analysislog.cpp \
    domparser.cpp \
    finddialog.cpp

HEADERS  += hello.h \
    analysislog.h \
    domparser.h \
    finddialog.h

RESOURCES += \
    hello.qrc

FORMS += \
    hello.ui

CONFIG += qaxcontainer

OTHER_FILES += \
    config.xml
