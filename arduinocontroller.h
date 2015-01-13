#ifndef ARDUINOCONTROLLER_H
#define ARDUINOCONTROLLER_H

#include <QObject>
#include "qextserialport.h"
#include <QByteArray>

class ArduinoController : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoController(QObject *parent = 0);
    ~ArduinoController();
    void closePort();
    void openPort();

private:
    QextSerialPort *port;
    QByteArray serialData;
    int sensorVoltage;
    double pressureOffset;

signals:
    void signalPortClosed();
    void signalPortOpen();
    void relayPressure();

public slots:
    void setValve(int value);
    void setVent(int value);
    void zeroPressure();

private slots:
    void onDataAvailable();

};

#endif // ARDUINOCONTROLLER_H
