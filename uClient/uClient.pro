#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T20:09:04
#
#-------------------------------------------------

QT       += core gui network websockets
QMAKE_CXXFLAGS += -std=c++0x
LIBS += -LC:/OpenSSL-Win32/lib/MinGW -llibeay32
LIBS += -LC:/OpenSSL-Win32/lib/MinGW -lssleay32

INCLUDEPATH += C:/OpenSSL-Win32/include

include(3rdparty/qtxlsx/xlsx/qtxlsx.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connector.cpp \
    core.cpp \
    loginwidget.cpp \
    itemgroup.cpp \
    item.cpp \
    computer.cpp \
    iteminfowidget.cpp \
    itemeditwidget.cpp \
    userwidget.cpp \
    cryptor.cpp

HEADERS  += mainwindow.h \
    connector.h \
    core.h \
    loginwidget.h \
    itemgroup.h \
    item.h \
    computer.h \
    iteminfowidget.h \
    itemeditwidget.h \
    userwidget.h \
    cryptor.h
