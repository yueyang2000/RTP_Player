#-------------------------------------------------
#
# Project created by QtCreator 2019-07-24T21:55:25
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = player1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        videostream.cpp

HEADERS += \
        packet.h \
        videostream.h

FORMS += \
        videostream.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH += /usr/local/Cellar/ffmpeg/4.1.1/include

LIBS += /usr/local/Cellar/ffmpeg/4.1.1/lib/libavcodec.dylib \
        /usr/local/Cellar/ffmpeg/4.1.1/lib/libavdevice.dylib \
        /usr/local/Cellar/ffmpeg/4.1.1/lib/libavfilter.dylib \
        /usr/local/Cellar/ffmpeg/4.1.1/lib/libavformat.dylib \
        /usr/local/Cellar/ffmpeg/4.1.1/lib/libavutil.dylib \
        /usr/local/Cellar/ffmpeg/4.1.1/lib/libpostproc.dylib \
        /usr/local/Cellar/ffmpeg/4.1.1/lib/libswresample.dylib \
        /usr/local/Cellar/ffmpeg/4.1.1/lib/libswscale.dylib
