#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T18:32:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FR
TEMPLATE = app


SOURCES     += main.cpp\
               mainwindow.cpp \
               facedetector.cpp \
    facerecognizer.cpp

HEADERS     += mainwindow.h \
               facedetector.h \
    facerecognizer.h

FORMS       += mainwindow.ui

INCLUDEPATH += C:\\OpenCV2\\opencv_bin\\install\\include


LIBS        += -LC:\\OpenCV2\\opencv_bin\\bin\
               -llibopencv_core2411d \
               -llibopencv_highgui2411d \
               -llibopencv_imgproc2411d \
               -llibopencv_features2d2411d \
               -llibopencv_objdetect2411d \
               -llibopencv_video2411d
