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

#INCLUDEPATH += "D:/OpenCV-2.4.10-android-sdk/sdk/native/jni/include"
#android {
#    LIBS += \
#        -L"D:/OpenCV-2.4.10-android-sdk/sdk/native/3rdparty/libs/armeabi-v7a"\
#        -L"D:/OpenCV-2.4.10-android-sdk/sdk/native/libs/armeabi-v7a"\
#        -llibtiff\
#        -llibjpeg\
#        -llibjasper\
#        -llibpng\
#        -lIlmImf\
#        -ltbb\
#        -lopencv_core\
#        -lopencv_imgproc\
#        -lopencv_java

#ANDROID_PACKAGE_SOURCE_DIR=$$_PRO_FILE_PWD_/android
#}

RESOURCES += \
    resources.qrc

