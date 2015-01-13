#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <QObject>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <windows.h>


class MotorController : public QObject
{
    Q_OBJECT

public:
    explicit MotorController(QObject *parent = 0);
    void closePort();
    void openPort();

private:
    int deviceNumber;
    HANDLE LAC_outpipe;
    void waitUntilMoveDone(double desiredPressure);

signals:
    int signalPortClosed();
    int signalPortOpen();
    void balanceFinished(int successful, int flag);
    void updateMotorPosition(double value);

public slots:
    void moveMotor(int value);
    void goToPressure(double desiredPressure, int flag);

};

#endif // MOTORCONTROLLER_H
