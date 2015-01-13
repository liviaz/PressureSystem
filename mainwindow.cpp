#include "mainwindow.h"
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <iostream>
#include <QDebug>
#include <QtCore/qmath.h>
#include <cmath>
#include <QThread>
#include <QtGlobal>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QImage>

#define MAX_STROKE 50
#define MOTOR_INIT_PCT 70
#define INVALID_PRESSURE -100

// 2 global variables
double pressure;
double motorPosition;

// constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // initialize some variables
    portOpen = 0;
    motorOpen = 0;
    cameraOpen = 0;
    pressure = 0;
    arduinoStartable = 0;
    motorStartable = 0;
    videoStartable = 0;
    balancePressureValue = -0.035;
    measurePressureValue = 0.4;
    motorPosition = MOTOR_INIT_PCT * MAX_STROKE / 100; // units of mm
    cameraImagePtr = NULL;
    scene = NULL;

    // setup UI
    ui->setupUi(this);
    ui->balancePressureDouble->setValue(balancePressureValue);
    ui->measurePressureDouble->setValue(measurePressureValue);
    ui->valveButton->setText("OPEN VALVE");
    ui->BoardStatusLabel->setText("CLOSED");
    ui->ArduinoStatusLabel->setText("CLOSED");
    ui->horizontalSlider->setMaximum(MAX_STROKE);
    ui->actualPressure->setText("+0.000 psi");
    ui->balanceButton->setEnabled(false);
    ui->measureButton->setEnabled(false);
    ui->OpenCameraButton->setEnabled(false);
    ui->horizontalSlider->setValue((int) motorPosition);

    // set up timer to update GUI
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCameraImage()));

    // start motorController, arduinoController, and cameraController
    mc = new MotorController();
    ac = new ArduinoController();
    cc = new CameraController();

    // set up threads
    QThread* thread_ac = new QThread;
    QThread* thread_mc = new QThread;
    QThread* thread_cc = new QThread;
    ac->moveToThread(thread_ac);
    mc->moveToThread(thread_mc);
    cc->moveToThread(thread_cc);

    // set up thread connections
    connect(thread_ac, SIGNAL(started()), this, SLOT(arduinoReadySlot()));
    connect(ac, SIGNAL(signalPortClosed()), this, SLOT(arduinoClosedSlot()));
    connect(ac, SIGNAL(signalPortOpen()), this, SLOT(arduinoOpenSlot()));
    connect(ac, SIGNAL(destroyed()), thread_ac, SLOT(quit()));
    connect(thread_ac, SIGNAL(finished()), thread_ac, SLOT(deleteLater()));

    connect(thread_mc, SIGNAL(started()), this, SLOT(motorReadySlot()));
    connect(mc, SIGNAL(signalPortClosed()), this, SLOT(motorClosedSlot()));
    connect(mc, SIGNAL(signalPortOpen()), this, SLOT(motorOpenSlot()));
    connect(mc, SIGNAL(destroyed()), thread_mc, SLOT(quit()));
    connect(thread_mc, SIGNAL(finished()), thread_mc, SLOT(deleteLater()));

    connect(thread_cc, SIGNAL(started()), this, SLOT(cameraReadySlot()));
    connect(cc, SIGNAL(cameraInitialized()), this, SLOT(cameraFinishedInit()));
    connect(this, SIGNAL(startCameraDisplay(QImage*)), cc, SLOT(displayVideo(QImage*)));
    connect(cc, SIGNAL(cameraFinished()), this, SLOT(cameraFinishedClose()));
    connect(cc, SIGNAL(destroyed()), thread_cc, SLOT(quit()));
    connect(thread_cc, SIGNAL(finished()), thread_cc, SLOT(deleteLater()));

    // set up data connections
    connect(this, SIGNAL(setMotorPosition(int)), mc, SLOT(moveMotor(int)));
    connect(this, SIGNAL(goToPressure(double, int)), mc, SLOT(goToPressure(double, int)));
    connect(mc, SIGNAL(balanceFinished(int, int)), this, SLOT(balanceFinished(int, int)));
    connect(mc, SIGNAL(updateMotorPosition(double)), this, SLOT(updateMotorPosition(double)));
    connect(ui->zeroPressure, SIGNAL(clicked()), ac, SLOT(zeroPressure()));
    connect(ac, SIGNAL(relayPressure()), this, SLOT(pressureUpdatedSlot()));
    connect(this, SIGNAL(setValve(int)), ac, SLOT(setValve(int)));
    connect(this, SIGNAL(setVent(int)), ac, SLOT(setVent(int)));

    // start threads
    thread_ac->start();
    thread_mc->start();
    thread_cc->start();
}


// destructor
MainWindow::~MainWindow()
{
    // close Arduino port
    if (portOpen){
        ac->closePort();
    }

    // close LAC port
    if (motorOpen){
        mc->closePort();
    }

    if (cameraOpen){
        cc->closeCamera();
        videoStartable = 0;
    }

    delete ac;
    delete mc;
    delete cc;
    delete timer;
    delete ui;

}


//on_stopButton_clicked: close serial port
void MainWindow::on_stopButton_clicked()
{
    if (portOpen){
        ac->closePort();
    }

    // close LAC port
    if (motorOpen){
        mc->closePort();
    }

    if (cameraOpen){
        cc->closeCamera();
        videoStartable = 0;
    }

    ui->statusLabel->setText("Stopped");
    ui->OpenCameraButton->setEnabled(false);
}


//on_startButton_clicked: open and set up serial port
void MainWindow::on_startButton_clicked()
{
    // if arduino is startable, start it
    if (arduinoStartable && motorStartable){
        if (!portOpen){
            ac->openPort();
        }

        if (!motorOpen){
            mc->openPort();
        }

        ui->statusLabel->setText("Started");
        ui->OpenCameraButton->setEnabled(true);
    }
}


// slot to signal arduino port closed
void MainWindow::arduinoClosedSlot(){
    ui->ArduinoStatusLabel->setText("CLOSED");
    portOpen = 0;
    pressure = 0;

    ui->stopButton->setEnabled(false);
    ui->ventButton->setEnabled(false);
    ui->valveButton->setEnabled(false);
    ui->initializeButton->setEnabled(false);
    ui->balanceButton->setEnabled(false);
    ui->measureButton->setEnabled(false);
    ui->zeroPressure->setEnabled(false);
    ui->startButton->setEnabled(true);
}


// slot to signal arduino port open
void MainWindow::arduinoOpenSlot(){
    ui->ArduinoStatusLabel->setText("OPEN");
    portOpen = 1;

    ui->stopButton->setEnabled(true);
    ui->ventButton->setEnabled(true);
    ui->valveButton->setEnabled(true);
    ui->initializeButton->setEnabled(true);
    ui->zeroPressure->setEnabled(true);
    ui->startButton->setEnabled(false);
}


void MainWindow::arduinoReadySlot(){
    ui->ArduinoStatusLabel->setText("READY TO OPEN");
    arduinoStartable = 1;
}


void MainWindow::motorReadySlot(){
    ui->BoardStatusLabel->setText("READY TO OPEN");
    motorStartable = 1;
}


// update pressure in GUI
void MainWindow::pressureUpdatedSlot(){

    // update this class's pressure variable
    //pressure = newPressure;

    // print out pressure
    QString data_string;
    data_string.sprintf("%+01.4f", pressure);
    data_string.append(" psi");
    ui->actualPressure->setText(data_string);
}


// redraw camera image in GUI
void MainWindow::updateCameraImage(){
    QGraphicsPixmapItem item(QPixmap::fromImage(*cameraImagePtr));
    scene->addItem(&item);
    ui->cameraImageDisplay->setScene(scene);
}

// on_valveButton_clicked: write high or low value to valve pin
void MainWindow::on_valveButton_clicked()
{
    if (!portOpen){
        std::cout << "Error: port not open yet" << std::endl;
        return;
    }

    int valueToWrite;

    if (ui->valveButton->text() == "OPEN VALVE"){
        valueToWrite = 1;
        ui->valveButton->setText("CLOSE VALVE");
        ui->valveButton->setChecked(1);
    } else {
        valueToWrite = 0;
        ui->valveButton->setText("OPEN VALVE");
        ui->valveButton->setChecked(0);
    }

    emit setValve(valueToWrite);
}


// send voltage to vent valve
void MainWindow::on_ventButton_clicked()
{
    if (!portOpen){
        std::cout << "Error: port not open yet" << std::endl;
        return;
    }

    int valueToWrite;

    if (ui->ventButton->text() == "VENT"){
        valueToWrite = 1;
        ui->ventButton->setText("CLOSE");
        ui->ventButton->setChecked(1);
    } else {
        valueToWrite = 0;
        ui->ventButton->setText("VENT");
        ui->ventButton->setChecked(0);
    }

    emit setVent(valueToWrite);
}


// slot to signify LAC port is closed
void MainWindow::motorClosedSlot(){
    motorOpen = 0;
    ui->BoardStatusLabel->setText("CLOSED");
}


// slot to signify LAC port is open
void MainWindow::motorOpenSlot(){
    motorOpen = 1;
    ui->BoardStatusLabel->setText("OPEN");
}


// take value from horizontal slider and relay to motor
void MainWindow::on_horizontalSlider_sliderReleased()
{
    // motor position goes from 0-50
    // needs to be converted to 0-1024
    double strokePercentage = ((double) ui->horizontalSlider->value())
            / ((double) MAX_STROKE);

    int voltageToWrite = strokePercentage * 1023;
    emit setMotorPosition(voltageToWrite);
    motorPosition = strokePercentage * MAX_STROKE;
}


// update desired balance pressure when box value changes
void MainWindow::on_balancePressureDouble_valueChanged(double arg1)
{
    balancePressureValue = arg1;
}


// update desired measurement pressure when box value changes
void MainWindow::on_measurePressureDouble_valueChanged(double arg1)
{
    measurePressureValue = arg1;
}


// move motor to "initial" position -- 70% of full stroke length
void MainWindow::on_initializeButton_clicked()
{
    // open vent valve first
    ui->ventButton->setText("CLOSE");
    ui->statusLabel->setText("Initializing ... please wait");
    ui->ventButton->setChecked(1);
    QThread::msleep(100);
    emit setVent(1);
    QThread::msleep(100);

    // move motor to initial position
    int voltageToWrite = MOTOR_INIT_PCT * 1023 / 100;
    emit setMotorPosition(voltageToWrite);

    // wait until move is done, then close vent valve
    QThread::sleep(5);
    ui->ventButton->setText("VENT");
    ui->ventButton->setChecked(0);
    emit setVent(0);

    // update motor parameters
    motorPosition = MOTOR_INIT_PCT * MAX_STROKE / 100;
    ui->horizontalSlider->setValue(MOTOR_INIT_PCT * MAX_STROKE / 100);
    ui->balanceButton->setEnabled(true);
    ui->measureButton->setEnabled(true);
    QThread::msleep(500);

    // zero the pressure
    QCoreApplication::processEvents();
    emit ui->zeroPressure->clicked();
    ui->statusLabel->setText(" ... ");
}


/*
 * Balance Pressure:
 * Implements while loop to send signal to motor based on difference
 * between current pressure and desired pressure.
 */
void MainWindow::on_balanceButton_clicked()
{

    ui->balanceButton->setChecked(true);
    ui->initializeButton->setEnabled(false);
    ui->measureButton->setEnabled(false);
    ui->statusLabel->setText("Balancing ... please wait");
    QThread::msleep(100);
    //QCoreApplication::processEvents();

    // call helper function to achieve desired pressure
    emit goToPressure(balancePressureValue, 0);
}

/*
 * This function signifies that desired pressure has been achieved
 * Also carries a flag if there is a next step to be carried out
 * 0: Do nothing next, just re-enable GUI
 * */
void MainWindow::balanceFinished(int successful, int flag){


    // if balance failed, don't do anything else, just
    // re-enable GUI. If flag is 0, just re-enable GUI
    if (!successful || flag == 0){

        if (!successful){
            ui->statusLabel->setText("Balance failed");
            QThread::sleep(2);
        }

        ui->horizontalSlider->setValue((int) motorPosition);
        ui->balanceButton->setChecked(false);
        ui->initializeButton->setEnabled(true);
        ui->measureButton->setEnabled(true);
        ui->balanceButton->setEnabled(true);
        ui->statusLabel->setText(" ... ");

     // if flag is 1, next step is to start recording video
     // and open valve
    } else if (flag == 1){
        // 2. start recording video

        // 3. open valve
        emit setValve(1);

        // 4. wait 3 seconds
        QThread::msleep(3000);

        // 5. stop recording video

        // 6. go back to initial pressure - 0.02 to release embryo
        emit goToPressure(balancePressureValue - 0.02, 2);

     // measurement is done: close valve and re-enable GUI
    } else if (flag == 2){

        // 7. close valve
        emit setValve(0);
        QThread::msleep(100);

        ui->balanceButton->setChecked(true);
        ui->balanceButton->setEnabled(true);
        ui->initializeButton->setEnabled(true);
        ui->measureButton->setEnabled(true);
        ui->statusLabel->setText(" ... ");

    }
}



// update the motor position
void MainWindow::updateMotorPosition(double value){
    motorPosition = value;
}



/*
 * on_measureButton_clicked():
 * Conduct a measurement: achieve desired pressure, open valve until
 * camera is done, then go back to original pressure
 */
void MainWindow::on_measureButton_clicked()
{
    ui->measureButton->setChecked(true);
    ui->initializeButton->setEnabled(false);
    ui->balanceButton->setEnabled(false);
    ui->statusLabel->setText("Measuring ... please wait");

    // 1. achieve desired pressure
    emit goToPressure(measurePressureValue, 1);
}


void MainWindow::cameraFinishedInit(){
    cameraOpen = 1;
    emit startCameraDisplay(cameraImagePtr);
}


void MainWindow::cameraFinishedClose(){
    cameraOpen = 0;
}

void MainWindow::cameraReadySlot(){
    videoStartable = 1;
}

void MainWindow::on_OpenCameraButton_clicked()
{
    if (videoStartable){
        cc->initCamera();
    }
}
