#-------------------------------------------------
#
# Project created by QtCreator 2016-05-30T16:34:13
#
#-------------------------------------------------

QT       -= gui

LIBS += -LC:/OpenSSL-Win32/lib/MinGW -llibeay32
LIBS += -LC:/OpenSSL-Win32/lib/MinGW -lssleay32

INCLUDEPATH += C:/OpenSSL-Win32/include

TARGET = QACrypt
TEMPLATE = lib

DEFINES += QACRYPT_LIBRARY

SOURCES += qacrypt.cpp

HEADERS += qacrypt.h\
        qacrypt_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
