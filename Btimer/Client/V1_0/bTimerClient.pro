#-------------------------------------------------
#
# Project created by QtCreator 2014-09-26T12:14:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bTimerClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gpio.c \
    myscrsaver.cpp

HEADERS  += mainwindow.h \
    gpio.h \
    myscrsaver.h

FORMS    += mainwindow.ui
