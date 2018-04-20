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
LIBS += -lmimetic
LIBS += -letpan

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
    ui/mainwindow.cpp \
    ui/accountswindow.cpp \
    core/generate.cpp \
    core/maildir.cpp \
    core/emlparser.cpp \
    core/mailfetch.cpp \
    third-party/Qt-Quoted-Printable/quotedprintable.cpp \
    utils/settings.cpp \
    utils/utils.cpp \
    ui/settingsdialog.cpp \
    ui/newaccdialog.cpp \
    ui/aboutdialog.cpp \
    ui/searchdialog.cpp

HEADERS  += ui/mainwindow.h \
    ui/accountswindow.h \
    core/generate.h \
    utils/settings.h \
    utils/utils.h \
    core/maildir.h \
    core/emlparser.h \
    core/mailfetch.h \
    third-party/Qt-Quoted-Printable/quotedprintable.h \
    ui/settingsdialog.h \
    ui/newaccdialog.h \
    ui/aboutdialog.h \
    ui/searchdialog.h
    
FORMS    += ui/mainwindow.ui \
    ui/accountswindow.ui \
    ui/settingsdialog.ui \
    ui/newaccdialog.ui \
    ui/aboutdialog.ui \
    ui/searchdialog.ui

RESOURCES += \
    ui/resources.qrc
