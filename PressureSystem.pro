#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T19:05:14
#
#-------------------------------------------------

QT       += core gui

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
CONFIG += console


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PressureSystem
TEMPLATE = app

INCLUDEPATH += "C:/Users/Admin/Desktop/opencv/build/include"
INCLUDEPATH += "C:/Users/Admin/Desktop/Software/PressureSystem/FirgelliMotor"
INCLUDEPATH += "C:/Users/Admin/Desktop/Software/PressureSystem/Camera"

include("C:/Users/Admin/Desktop/Software/PressureSystem/qextserialport-1.2rc/src/qextserialport.pri")

QMAKE_CXXFLAGS += -Wall

LIBS += -LC:/Users/Admin/Desktop/opencv/build/x86/vc10/lib \
    -lopencv_core2410d \
    -lopencv_highgui2410d \
    -lopencv_imgproc2410d

LIBS += -LC:/Users/Admin/Desktop/opencv/build/x86/vc10/bin \
    -lopencv_core2410d \
    -lopencv_highgui2410d \
    -lopencv_imgproc2410d


LIBS += "C:/Users/Admin/Desktop/Software/PressureSystem/FirgelliMotor/mpusbapi.lib"
LIBS += "C:/Users/Admin/Desktop/Software/PressureSystem/Camera/uc480.lib"
LIBS += "C:/Users/Admin/Desktop/Software/PressureSystem/Camera/uc480_tools.lib"


SOURCES += main.cpp \
    mainwindow.cpp \
    motorcontroller.cpp \
    arduinocontroller.cpp \
    roirect.cpp \
    cameracontroller.cpp

HEADERS  += mainwindow.h \
    motorcontroller.h \
    FirgelliMotor/mpusbapi.h \
    arduinocontroller.h \
    roirect.h \
    cameracontroller.h


FORMS    += mainwindow.ui

