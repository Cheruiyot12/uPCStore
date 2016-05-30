#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T20:09:51
#
#-------------------------------------------------

QT       += core gui network sql websockets xml
QMAKE_CXXFLAGS += -std=c++0x
LIBS += -LC:/OpenSSL-Win32/lib/MinGW -llibeay32
LIBS += -LC:/OpenSSL-Win32/lib/MinGW -lssleay32

INCLUDEPATH += C:/OpenSSL-Win32/include
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    core.cpp \
    client.cpp \
    clientdatabase.cpp \
    cryptor.cpp

HEADERS  += mainwindow.h \
    core.h \
    client.h \
    clientdatabase.h \
    commands.h \
    cryptor.h
