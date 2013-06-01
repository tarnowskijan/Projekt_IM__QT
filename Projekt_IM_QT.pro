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
    mouthStatetDet.cpp

HEADERS  += mainwindow.h \
        mouthStateDet.h \
    configwindow.h

FORMS    += mainwindow.ui \
    configwindow.ui

RESOURCES += \
    resources.qrc

INCLUDEPATH += \
    C:\Users\joHn_ny\opencv\opencv\build\include

LIBS += \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_core244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_highgui244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_imgproc244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_nonfree244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_video244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_videostab244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_features2d244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_contrib244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_gpu244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_legacy244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_ml244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_objdetect244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_photo244.dll.a \
    C:\Users\joHn_ny\opencv\opencv\own_build\lib\libopencv_stitching244.dll.a
