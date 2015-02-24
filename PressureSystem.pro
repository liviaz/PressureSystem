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

INCLUDEPATH += C:/Users/Livia/Desktop/OpenCV/v2410/build/include
INCLUDEPATH += "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/FirgelliMotor"
INCLUDEPATH += "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/Camera"

include("C:/Users/Livia/Desktop/IVF/Code/sample code/qextserialport-1.2rc/qextserialport-1.2rc/src/qextserialport.pri")

QMAKE_CXXFLAGS += -Wall

LIBS += -LC:/Users/Livia/Desktop/OpenCV/v2410/build/x86/vc10/lib \
    -lopencv_core2410d \
    -lopencv_highgui2410d \
    -lopencv_imgproc2410d

LIBS += -LC:/Users/Livia/Desktop/OpenCV/v2410/build/x86/vc10/bin \
    -lopencv_core2410d \
    -lopencv_highgui2410d \
    -lopencv_imgproc2410d


LIBS += "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/FirgelliMotor/mpusbapi.lib"
LIBS += "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/Camera/uc480.lib"
LIBS += "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/Camera/uc480_tools.lib"


SOURCES += main.cpp \
    mainwindow.cpp \
    motorcontroller.cpp \
    arduinocontroller.cpp \
    cameracontroller.cpp \
    roirect.cpp

HEADERS  += mainwindow.h \
    motorcontroller.h \
    FirgelliMotor/mpusbapi.h \
    arduinocontroller.h \
    cameracontroller.h \
    roirect.h


FORMS    += mainwindow.ui
