#-------------------------------------------------
#
# Project created by QtCreator 2015-02-11T12:29:49
#
#-------------------------------------------------

QT       += core gui xml multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console

TARGET = "Mood Finder"
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    datamodel.cpp \
    VideoDockWidget.cpp \
    player.cpp \
    playercontrols.cpp \
    videowidget.cpp

HEADERS  += mainwindow.h \
    datamodel.h \
    commons.h \
    VideoDockWidget.h \
    player.h \
    playercontrols.h \
    videowidget.h

FORMS    += mainwindow.ui \
    videodockwidget.ui
