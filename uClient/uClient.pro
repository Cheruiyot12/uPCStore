#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T20:09:04
#
#-------------------------------------------------

QT       += core gui network websockets
QMAKE_CXXFLAGS += -std=c++0x
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connector.cpp \
    core.cpp \
    loginwidget.cpp \
    itemgroup.cpp \
    item.cpp

HEADERS  += mainwindow.h \
    connector.h \
    core.h \
    loginwidget.h \
    itemgroup.h \
    item.h
