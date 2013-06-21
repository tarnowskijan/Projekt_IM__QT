#-------------------------------------------------
#
# Project created by QtCreator 2013-06-01T15:55:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projekt_IM_QT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configwindow.cpp \
    mouthStateDet.cpp

HEADERS  += mainwindow.h \
        mouthStateDet.h \
    configwindow.h

FORMS    += mainwindow.ui \
    configwindow.ui

RESOURCES +=

INCLUDEPATH += \
   D:\opencv_bin\install\include

LIBS += \
    D:\opencv_bin\install\lib\libopencv_core244.dll.a \
    D:\opencv_bin\install\lib\libopencv_highgui244.dll.a \
    D:\opencv_bin\install\lib\libopencv_imgproc244.dll.a \
    D:\opencv_bin\install\lib\libopencv_nonfree244.dll.a \
    D:\opencv_bin\install\lib\libopencv_video244.dll.a \
    D:\opencv_bin\install\lib\libopencv_videostab244.dll.a \
    D:\opencv_bin\install\lib\libopencv_features2d244.dll.a \
    D:\opencv_bin\install\lib\libopencv_contrib244.dll.a \
    D:\opencv_bin\install\lib\libopencv_gpu244.dll.a \
    D:\opencv_bin\install\lib\libopencv_legacy244.dll.a \
    D:\opencv_bin\install\lib\libopencv_ml244.dll.a \
    D:\opencv_bin\install\lib\libopencv_objdetect244.dll.a \
    D:\opencv_bin\install\lib\libopencv_photo244.dll.a \
    D:\opencv_bin\install\lib\libopencv_stitching244.dll.a
