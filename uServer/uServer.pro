#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T20:09:51
#
#-------------------------------------------------

QT       += core gui network sql websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    core.cpp \
    client.cpp \
    clientdatabase.cpp

HEADERS  += mainwindow.h \
    core.h \
    client.h \
    clientdatabase.h \
    commands.h
