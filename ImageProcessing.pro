#-------------------------------------------------
#
# Project created by QtCreator 2015-06-22T05:09:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcessing
TEMPLATE = app



SOURCES += main.cpp\
        mainwindow.cpp \
    mylabel.cpp

HEADERS  += mainwindow.h \
    mylabel.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

INCLUDEPATH += D://opencv-mingw//install//include
LIBS += -L"D:\opencv-mingw\install\x86\mingw\bin"
LIBS += "D:\opencv-mingw\install\x86\mingw\lib\libopencv_core300.dll.a"
LIBS += "D:\opencv-mingw\install\x86\mingw\lib\libopencv_imgcodecs300.dll.a"
LIBS += "D:\opencv-mingw\install\x86\mingw\lib\libopencv_imgproc300.dll.a"
