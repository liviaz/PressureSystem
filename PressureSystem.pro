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

INCLUDEPATH += "C:/opencv/install/include/"
INCLUDEPATH += "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/FirgelliMotor"
INCLUDEPATH += "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/Camera"

include("C:/Users/Livia/Desktop/IVF/Code/sample code/qextserialport-1.2rc/qextserialport-1.2rc/src/qextserialport.pri")

QMAKE_CXXFLAGS += -Wall

LIBS += -LC:/opencv/bin \
    -llibopencv_core2410 \
    -llibopencv_highgui2410

LIBS += -L"C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/FirgelliMotor" \
    -lmpusbapi

#LIBS += -L"C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/Camera" \
#    -luc480


SOURCES += main.cpp \
    mainwindow.cpp \
    motorcontroller.cpp \
    arduinocontroller.cpp \
    cameracontroller.cpp

HEADERS  += mainwindow.h \
    motorcontroller.h \
    FirgelliMotor/mpusbapi.h \
    arduinocontroller.h \
    cameracontroller.h


FORMS    += mainwindow.ui
