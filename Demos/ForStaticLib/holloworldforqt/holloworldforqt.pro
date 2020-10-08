#-------------------------------------------------
#
# Project created by QtCreator 2020-09-13T14:10:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = holloworldforqt
TEMPLATE = app


SOURCES += main.cpp\
        holloworld.cpp

HEADERS  += holloworld.h

FORMS    += holloworld.ui

LIBS += -L../../ForDynamicLib/PreCompiled/ -lCChart
LIBS += -L../../ForDynamicLib/PreCompiled/ -lCChartu