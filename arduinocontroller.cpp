#include "arduinocontroller.h"
#include "qextserialport.h"
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QMessageBox>
#include <iostream>
#include <QDebug>

#define VALVE_PIN 3
#define VENT_PIN 4
#define START_ZERO_PRESSURE 508

extern double pressure;
extern double motorPosition;

ArduinoController::ArduinoController(QObject *parent) :
    QObject(parent)
{
    pressureOffset = START_ZERO_PRESSURE;
    sensorVoltage = -1;
    serialData.clear();
    port = NULL;
}

// destructor
ArduinoController::~ArduinoController(){
    if (port){
        delete port;
    }
}

// signal that the port was closed successfully
void ArduinoController::closePort(){
    port->close();
    emit signalPortClosed();
}


// signal that the port was opened successfully
void ArduinoController::openPort(){


    // setup serial port
    port = new QextSerialPort("COM8");
    connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    port->open(QIODevice::ReadWrite);

    if (!port->isOpen())
    {
        qDebug() << "Error: couldn't open port";
        return;
    }

    port->setBaudRate(BAUD9600);
    emit signalPortOpen();


}


// send voltage to pressure valve
void ArduinoController::setValve(int value){

    // if button is down, open valve. Otherwise, close it.
    unsigned char valueToWrite;

    if (value > 0){
        valueToWrite = 0xFF;
    } else {
        valueToWrite = 0x0;
    }

    QByteArray valueArray;
    valueArray.append((unsigned char) VALVE_PIN);
    valueArray.append(valueToWrite);

    port->write(valueArray);
    port->flush();
}


// send voltage to vent valve
void ArduinoController::setVent(int value){
    // if button is down, open valve. Otherwise, close it.
    unsigned char valueToWrite;

    if (value > 0){
        valueToWrite = 0xFF;
    } else {
        valueToWrite = 0x0;
    }

    QByteArray valueArray;
    valueArray.append((unsigned char) VENT_PIN);
    valueArray.append(valueToWrite);

    port->write(valueArray);
    port->flush();
}


/*
 * zeroPressure:
 * set zero pressure reference to current pressure
 */
void ArduinoController::zeroPressure(){

    //std::cout << "pressureOffset: " << pressureOffset << std::endl;
    if (sensorVoltage > 0){
        pressureOffset = sensorVoltage;
    }
}

/*
 * onDataAvailable: read data from the serial port
 * If it does not end in appropriate character, put it
 * in serialData variable and keep waiting for more data
 */
void ArduinoController::onDataAvailable()
{
    QByteArray data = port->readAll();

    // check if we've received the whole transmission
    if (data.size() > 0){

        // append the current transmission to what we've received so far
        serialData.append(data);

        // if this represents the full transmission (with ending char)
        // convert to pressure
        if (((unsigned char) data[data.size() - 1]) == 0xFF){

            // take just the last transmission, if many are bunched together
            // returns original QByteArray if there is only one transmission
            int secondLastEnd = serialData.left(serialData.length() - 1).lastIndexOf(0xFF);
            serialData = serialData.right(serialData.length() - secondLastEnd - 1);

            // convert to int
            double dataInt = 0;

            for (int i = 0; i < serialData.length() - 1; i++){
                dataInt = dataInt * 10 + serialData[i] - 48;
            }

            // convert from 10-bit analog value to psi
            sensorVoltage = dataInt;
            pressure =  ((double) sensorVoltage - pressureOffset) / 36.95 * .03614;
            emit relayPressure();

            // clear serialData to make room for next transmission
            serialData.clear();
        }
    }
}





