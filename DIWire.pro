#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T08:40:24
#
#-------------------------------------------------

QT       += core gui\
            xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DIWire
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    bezier.cpp \
    pathdata.cpp \
    ellipticarc.cpp

HEADERS  += dialog.h \
    datadef.h \
    bezier.h \
    pathdata.h \
    ellipticarc.h

FORMS    += dialog.ui
