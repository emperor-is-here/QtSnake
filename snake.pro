#-------------------------------------------------
#
# Project created by QtCreator 2015-06-18T16:52:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snake
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    field.cpp

HEADERS  += mainwindow.h \
    field.h

FORMS    += mainwindow.ui
