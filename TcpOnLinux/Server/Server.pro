#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T10:10:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
        serverwindow.cpp \
    tcpserver.cpp \
    log.cpp \
    thread.cpp \
    acceptthread.cpp \
    recvthread.cpp

HEADERS  += serverwindow.h \
    tcpserver.h \
    log.h \
    thread.h \
    acceptthread.h \
    recvthread.h

FORMS    += serverwindow.ui
