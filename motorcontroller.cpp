#include "motorcontroller.h"
#include <windows.h>
#include "FirgelliMotor/mpusbapi.h"
#include <iostream>
#include <QDebug>
#include <stdio.h>
#include <QThread>

#define MAX_STROKE 50

extern double pressure;
extern double motorPosition;

// constructor: initialize
MotorController::MotorController(QObject *parent) :
    QObject(parent)
{
    LAC_outpipe = NULL;

    // set the accuracy/resolution

}


// move to position
void MotorController::moveMotor(int value){

    if (value > 1023) value = 1023;
    if (value < 0) value = 0;

    //qDebug() << "motor position: " << value;

    // set data to write
    BYTE bufData[3];
    bufData[0] = 0x20; // set position
    bufData[1] = (unsigned char) value; // data low
    bufData[2] = (unsigned char) (value >> 8); // data high
    DWORD bufProc;

    MPUSBWrite(LAC_outpipe, bufData, sizeof(bufData), &bufProc, 1000);

    //qDebug() << "chars processed: " << bufProc;
}


// close serial port
void MotorController::closePort(){
    if (MPUSBClose(LAC_outpipe)){
        std::cout << "port closed successfully" << std::endl;
        emit signalPortClosed();
    } else {
        std::cout << "port closing unsuccessful" << std::endl;
    }
}


// open serial port
void MotorController::openPort(){

    char vidPid[] = "vid_04d8&pid_fc5f";
    char pipeName[] = "\\MCHP_EP1";

    DWORD numDevices = MPUSBGetDeviceCount(vidPid);
    std::cout << "Num devices: " << numDevices << std::endl;
    deviceNumber = 0;

    // open device
    LAC_outpipe = MPUSBOpen(deviceNumber, vidPid, pipeName, MP_WRITE, 0);
    if (LAC_outpipe != NULL){
        emit signalPortOpen();
    } else {
        std::cout << "port cannot be opened" << std::endl;
    }

    BYTE bufData[3] = {0, 0, 0};
    DWORD bufProc;
    MPUSBRead(LAC_outpipe, bufData, sizeof(bufData), &bufProc, 1000);
}


// control loop to go to a certain pressure
// this is the main function used to achieve a desired pressure
void MotorController::goToPressure(double desiredPressure, int flag){

    int numCycles = 0;
    int maxCycles = 20;
    double accuracy = .0025;
    int P = 15;
    int voltageToWrite = 0;
    double motorToMove = 0;

    // move motor, wait until it's done, move again, etc.
    while (qAbs(desiredPressure - pressure) > accuracy && numCycles < maxCycles){

        // determined how much to move motor
        motorToMove = (desiredPressure - pressure) * P;

        if (motorToMove > 0 && motorToMove < .2){
            motorToMove = .2;
        } else if (motorToMove < 0 && motorToMove > -0.2) {
            motorToMove = -0.2;
        }

        // move motor
        motorPosition -= motorToMove;
        voltageToWrite = motorPosition * 1023 / MAX_STROKE;
        moveMotor(voltageToWrite);

        numCycles++;

        // wait until it's done before moving again
        waitUntilMoveDone(desiredPressure);
    }

    if (numCycles == maxCycles){
        emit balanceFinished(0, flag);
    }

    emit balanceFinished(1, flag);
}



// wait until pressure has stabilized before moving motor again
void MotorController::waitUntilMoveDone(double desiredPressure){

    // update pressure
    //QCoreApplication::processEvents();
    double currPressure = pressure;
    double lastPressure = desiredPressure;
    int numCycles = 0;

    while(qAbs(lastPressure - currPressure) > .002 && numCycles < 10){
        //qDebug() << "lastPressure: " << lastPressure;
        //qDebug() << "pressure: " << pressure;
        //qDebug() << "currPressure: " << currPressure;
        QThread::msleep(200);

        lastPressure = currPressure;
        currPressure = pressure;
        numCycles++;
    }

    if (numCycles == 10){
        //std::cout << "waited too long for pressure to stabilize" << std::endl;
    }
}



