#-------------------------------------------------
#
# Project created by QtCreator 2017-10-22T11:09:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkit webkitwidgets

TARGET = CuteMail
TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/readmailbox.cpp \
    src/generate.cpp \
    src/fetchmail.cpp \
    src/mhonarc.cpp \
    src/settings.cpp \
    src/maildir.cpp

HEADERS  += src/mainwindow.h \
    src/readmailbox.h \
    src/generate.h \
    src/fetchmail.h \
    src/mhonarc.h \
    src/settings.h \
    src/maildir.h

FORMS    += src/mainwindow.ui
