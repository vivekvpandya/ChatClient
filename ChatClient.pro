#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T01:02:30
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chatroom.cpp

HEADERS  += mainwindow.h \
    chatroom.h

FORMS    += mainwindow.ui \
    chatroom.ui
