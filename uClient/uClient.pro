#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T20:09:04
#
#-------------------------------------------------

QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connector.cpp \
    core.cpp \
    loginwidget.cpp

HEADERS  += mainwindow.h \
    connector.h \
    core.h \
    loginwidget.h
