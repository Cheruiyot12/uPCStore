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
    clientdatabase.cpp

HEADERS  += mainwindow.h \
    core.h \
    client.h \
    clientdatabase.h \
    commands.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QACrypt/release/ -lQACrypt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QACrypt/debug/ -lQACrypt
else:unix:!macx: LIBS += -L$$OUT_PWD/../QACrypt/ -lQACrypt

INCLUDEPATH += $$PWD/../QACrypt
DEPENDPATH += $$PWD/../QACrypt
