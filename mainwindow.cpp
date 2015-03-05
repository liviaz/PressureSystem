#include "mainwindow.h"
#include <QMainWindow>
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
#include <QImage>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include "roirect.h"

using namespace cv;

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
    videoOpen = 0;
    pressure = 0;
    arduinoStartable = 0;
    motorStartable = 0;
    videoStartable = 0;
    balancePressureValue = -0.035;
    measurePressureValue = 0.4;
    motorPosition = MOTOR_INIT_PCT * MAX_STROKE / 100; // units of mm
    cameraImagePtr = NULL;
    cameraClosable = 0;
    scene = NULL;
    baseTime = QDateTime::currentMSecsSinceEpoch();
    frameRateMinimum = 0;
    frameRateMaximum = 100;
    frameRateCurr = 10;
    frameRateIncrement = 0.01;
    exposureTimeMin = 0;
    exposureTimeMax = 100;
    exposureTimeCurr = 10;
    exposureTimeIncrement = 1;
    videoWriter = NULL;
    recordingVideo = 0;
    valveOpened = 0;
    frameMsec = 50;

    // timers related to measurement
    videoTimer = new QTimer(this);
    connect(videoTimer, SIGNAL(timeout()), this, SLOT(stopRecording()));
    cropTimer = new QTimer(this);
    connect(cropTimer, SIGNAL(timeout()), this, SLOT(startRecording()));
    uncropTimer = new QTimer(this);
    connect(uncropTimer, SIGNAL(timeout()), this, SLOT(finishMeasurement()));
    currWidth = 0;
    currHeight = 0;

    // ROI related
    currPixmapItem = NULL;
    rectAdded = FALSE;
    ROIselected = FALSE;
    pixmapSet = false;
    imageRoi = NULL; // initialize ROI bounding box;
    scene = new QGraphicsScene(this);
    saveX = 50;
    saveY = 50;
    saveW = 1180;
    saveH = 924;

    // setup UI
    ui->setupUi(this);
    ui->balancePressureDouble->setValue(balancePressureValue);
    ui->measurePressureDouble->setValue(measurePressureValue);
    ui->valveButton->setText("OPEN VALVE");
    ui->horizontalSlider->setMaximum(MAX_STROKE);
    ui->actualPressure->setText("+0.000 psi");
    ui->balanceButton->setEnabled(false);
    ui->measureButton->setEnabled(false);
    ui->horizontalSlider->setValue((int) motorPosition);
    ui->cameraImageDisplay->setScene(scene);
    ui->FrameRateSlider->setValue(10);
    ui->ExposureTimeSlider->setValue(10);
    ui->PixelClockIndicator->setText("40");
    ui->PixelClockSlider->setValue(40);
    ui->ZoomToSelectionButton->setEnabled(false);
    ui->ClearSelectionButton->setEnabled(false);

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

    // thread connections for camera
    connect(thread_cc, SIGNAL(started()), this, SLOT(cameraReadySlot()));
    connect(this, SIGNAL(initCamera()), cc, SLOT(initCamera())); // tell camera to initialize
    connect(cc, SIGNAL(cameraInitialized()), this, SLOT(cameraFinishedInit())); // wait until camera has finished init
    connect(this, SIGNAL(startCameraDisplay()), cc, SLOT(startVideo()));
    connect(this, SIGNAL(stopCameraDisplay()), cc, SLOT(stopVideo()));
    connect(cc, SIGNAL(videoStarted()), this, SLOT(videoStarted()));
    connect(cc, SIGNAL(updateImage(char *, int, int)), this, SLOT(cameraFrameReceived(char *, int, int))); // update GUI when new frame available
    connect(this, SIGNAL(setCameraParams(int,int)), cc, SLOT(setCameraParams(int,int)));
    connect(cc, SIGNAL(updateFrameRate(double)), this, SLOT(updateFrameRate(double)));
    connect(this, SIGNAL(changeCameraROI(QRectF)), cc, SLOT(changeCameraROI(QRectF)));
    connect(cc, SIGNAL(updateCameraParamsInGui(double *)), this, SLOT(updateCameraParamsInGui(double *)));
    connect(this, SIGNAL(closeCamera()), cc, SLOT(closeCamera())); // tell camera to close
    connect(cc, SIGNAL(cameraClosed()), this, SLOT(cameraFinishedClose())); // wait until camera is finished closing
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

    if (imageRoi != NULL){
        delete imageRoi;
        imageRoi = NULL;
    }

    if (cameraOpen){
        videoOpen = 0;
        scene->clear();
        imageRoi = NULL;
        currPixmapItem = NULL;
        rectAdded = false;
        pixmapSet = false;

        emit stopCameraDisplay();
        ui->ZoomToSelectionButton->setEnabled(false);
        ui->ClearSelectionButton->setEnabled(false);
    }

    emit closeCamera();

    // wait for CameraController to be cleaned up before deleting it
    while (true){
       QCoreApplication::processEvents();
       if (cameraClosable){
           break;
       }
       QThread::msleep(100);
    }

    delete ac;
    delete mc;
    delete cc;
    delete scene;
    delete ui;



}


/* **************************************
 *                                      *
 *  Basic ui and variable stuff         *
 *                                      *
 * ************************************ */


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

        videoOpen = 0;
        scene->clear();
        imageRoi = NULL;
        currPixmapItem = NULL;
        rectAdded = false;
        pixmapSet = false;

        emit stopCameraDisplay();
        ui->ZoomToSelectionButton->setEnabled(false);
        ui->ClearSelectionButton->setEnabled(false);

        emit closeCamera();
    }

    ui->statusLabel->setText("Stopped");
}



//on_startButton_clicked: open and set up serial port
void MainWindow::on_startButton_clicked()
{
    // if arduino is startable, start it
    if (arduinoStartable && motorStartable && videoStartable){
        if (!portOpen){
            ac->openPort();
        }

        if (!motorOpen){
            mc->openPort();
        }

        emit initCamera();
        cameraClosable = 0;

        ui->statusLabel->setText("Started");
    }

}



// slot to signal arduino port closed
void MainWindow::arduinoClosedSlot(){
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
    portOpen = 1;

    ui->stopButton->setEnabled(true);
    ui->ventButton->setEnabled(true);
    ui->valveButton->setEnabled(true);
    ui->initializeButton->setEnabled(true);
    ui->zeroPressure->setEnabled(true);
    ui->startButton->setEnabled(false);
}


void MainWindow::arduinoReadySlot(){
    arduinoStartable = 1;
}


void MainWindow::motorReadySlot(){
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



/* **************************************
 *                                      *
 *  Measure functions                   *
 *                                      *
 * ************************************ */


// slot to signify LAC port is closed
void MainWindow::motorClosedSlot(){
    motorOpen = 0;
}


// slot to signify LAC port is open
void MainWindow::motorOpenSlot(){
    motorOpen = 1;
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


// update the motor position
void MainWindow::updateMotorPosition(double value){
    motorPosition = value;
}


/* **************************************
 *                                      *
 *  Measure functions                   *
 *                                      *
 * ************************************ */


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
    QThread::sleep(1);
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
            QThread::sleep(1);
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

        // 2. Crop ROI
        QRectF boundingROI = imageRoi->boundingRect();
        saveX = imageRoi->xTopLeft;
        saveY = imageRoi->yTopLeft;
        saveW = imageRoi->width;
        saveH = imageRoi->height;

        // stop video
        videoOpen = 0;
        scene->clear();
        imageRoi = NULL;
        currPixmapItem = NULL;
        rectAdded = true;
        pixmapSet = false;
        emit stopCameraDisplay();

        // resize ROI
        emit changeCameraROI(boundingROI);

        // restart video with no bounding rect
        scene->clear();
        emit startCameraDisplay();
        videoOpen = 1;

        QString currResolution;
        currResolution.sprintf("%d x %d", saveW, saveH);
        ui->ResolutionIndicator->setText(currResolution);

        // now wait a bit for frames to start coming in
        cropTimer->start(1000);


     // measurement is done: close valve and re-enable GUI
    } else if (flag == 2){

        // 7. close valve
        emit setValve(0);

        ui->balanceButton->setChecked(true);
        ui->balanceButton->setEnabled(true);
        ui->initializeButton->setEnabled(true);
        ui->measureButton->setEnabled(true);
        ui->statusLabel->setText(" ... ");

    }
}


// start recording video
void MainWindow::startRecording()
{

    cropTimer->stop();

    // init video
    fileName = QFileDialog::getSaveFileName(this,
         tr("Open Image"), "C:/Users/Admin/Desktop");
    fileName.append(".avi");

    // 4. start grabbing frames for the next 3 seconds
    recordingVideo = 1;
    valveOpened = 0;
    videoTimer->start(3000);

}


// stop recording video and release embryo
void MainWindow::stopRecording()
{

    // 6. stop video
    recordingVideo = 0;
    valveOpened = 0;
    videoTimer->stop();
    videoWriter->release();
    videoWriter = NULL;
    uncropTimer->start(1000);

}

// un-crop ROI after everything is done.
void MainWindow::finishMeasurement(){

    uncropTimer->stop();

    // 7. Un-crop ROI
    QRectF boundingROI = QRectF(0, 0, 1280, 1024);

    // stop video
    videoOpen = 0;
    scene->clear();
    imageRoi = NULL;
    currPixmapItem = NULL;
    rectAdded = false;
    pixmapSet = false;
    emit stopCameraDisplay();

    // resize ROI
    emit changeCameraROI(boundingROI);

    // restart video
    imageRoi = new RoiRect(saveX, saveY, saveW, saveH);
    scene->clear();
    emit startCameraDisplay();
    videoOpen = 1;

    ui->ResolutionIndicator->setText("1280 x 1024");

    // 8. go back to initial pressure - 0.02 to release embryo
    emit goToPressure(balancePressureValue - 0.02, 2);
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



/* **************************************
 *                                      *
 *  Camera stuff                        *
 *                                      *
 * ************************************ */


// update cameraImagePtr when a new frame is received from the camera
void MainWindow::cameraFrameReceived(char* imgFromCamera, int width, int height){

    cameraImagePtr = new QImage(reinterpret_cast<uchar *>(imgFromCamera),
                                width, height, QImage::Format_RGB32);

    //cameraImagePtr = imgFromCamera;

    if (cameraOpen && videoOpen){

        if (pixmapSet == false){
            pixmapSet = true;
            currPixmapItem = scene->addPixmap(QPixmap::fromImage(*cameraImagePtr));
        } else if (frameMsec < 500 && frameMsec > 5){
            currPixmapItem->setPixmap(QPixmap::fromImage(*cameraImagePtr));
        }

        // add imageRoi just the first time
        if (!rectAdded){
            rectAdded = TRUE;
            scene->addItem(imageRoi);
            imageRoi->setZValue(1);
            imageRoi->setVisible(true);
        }

        currWidth = cameraImagePtr->width();
        currHeight = cameraImagePtr->height();

        if (recordingVideo){

            // write frame to video
            if (videoWriter == NULL) {

                cv::Size frameSize(currWidth, currHeight);
                videoWriter = new VideoWriter(fileName.toStdString(), CV_FOURCC('D', 'I', 'B', ' '),
                                              frameRateCurr, frameSize, true);


            } else if (videoWriter->isOpened()) {

                Mat tempMat = Mat(currHeight, currWidth, CV_8UC4,
                                      const_cast<uchar*>(cameraImagePtr->bits()),
                                      cameraImagePtr->bytesPerLine());
                videoWriter->write(tempMat);

                qDebug() << "wrote frame to videowriter";
            }

            // 5. if valve not open yet, open it and wait 3 seconds
            if (valveOpened == 0){
                valveOpened = 1;
                emit setValve(1);
                qDebug() << "valve set";
            }

        }

        scene->setSceneRect(cameraImagePtr->rect());
        ui->cameraImageDisplay->setScene(scene);
        ui->cameraImageDisplay->fitInView(scene->itemsBoundingRect(),
                                          Qt::KeepAspectRatio);

        qint64 currTime = QDateTime::currentMSecsSinceEpoch();
        frameMsec = currTime - baseTime;
        qDebug() << "frame msecs: " << frameMsec;
        baseTime = currTime;

    }
}



// signifies that camera is done initializing
// automatically start the video
void MainWindow::cameraFinishedInit(){
    cameraOpen = 1;
    cameraClosable = 0;

    // either start or stop video depending on state at time of click
    if (videoStartable && cameraOpen && !videoOpen){

        imageRoi = new RoiRect(50, 50, 1180, 924);
        scene->clear();

        emit startCameraDisplay();
        videoOpen = 1;
        ui->ResolutionIndicator->setText("1280 x 1024");
        ui->ZoomToSelectionButton->setEnabled(true);
        ui->ClearSelectionButton->setEnabled(true);

    }
}


// signifies that camera is done closing and mainwindow can be deleted
void MainWindow::cameraFinishedClose(){
    cameraClosable = 1;
    cameraOpen = 0;
}


// wait for camera thread to start
void MainWindow::cameraReadySlot(){
    videoStartable = 1;
}



void MainWindow::videoStarted(){
    videoOpen = 1;
}






/* change exposure time when slider released */
void MainWindow::on_ExposureTimeSlider_sliderReleased()
{
    double valuePct =  ui->ExposureTimeSlider->value();
    double valueOut = exposureTimeMin + valuePct * exposureTimeIncrement;
    emit setCameraParams(CHANGE_EXPOSURE_TIME, valueOut);
}




/* change frame rate when slider released
 * frameRateIncrement maps to 1 on the slider increment
 *
 */
void MainWindow::on_FrameRateSlider_sliderReleased()
{
    double valuePct =  ui->FrameRateSlider->value();
    double valueOut = frameRateMinimum + valuePct * frameRateIncrement;
    emit setCameraParams(CHANGE_FRAME_RATE, valueOut);
}





// change pixel clock
void MainWindow::on_PixelClockSlider_sliderReleased()
{
    QString pixelClockActual;
    pixelClockActual.sprintf("%d",ui->PixelClockSlider->value());
    ui->PixelClockIndicator->setText(pixelClockActual);
    emit setCameraParams(CHANGE_PIXEL_CLOCK, ui->PixelClockSlider->value());
}




/* update camera param ranges in GUI
 * paramList is: [frameRateLow, frameRateHigh, frameRateCurr,
 * exposureTimeLow, exposureTimeHigh, exposureTimeCurr,
 * exposureTimeIncrement]
 * */
void MainWindow::updateCameraParamsInGui(double *paramList){

    if (paramList == NULL){
        return;
    }

    for (int i = 0; i < 7; i++){

        // only update parameters with values > 0
        if (paramList[i] > 0){            
            if (i == 0){
                 qDebug() << "update frameRateMinimum";
                frameRateMinimum = paramList[i];
                QString frameRateLowS;
                frameRateLowS.sprintf("%1.2f", paramList[i]);
                frameRateLowS.append(" fps");
                ui->FrameRateLow->setText(frameRateLowS);
                ui->FrameRateSlider->setMinimum(0);
            } else if (i == 1){
                 qDebug() << "update frameRateMaximum";
                frameRateMaximum = paramList[i];
                QString frameRateHighS;
                frameRateHighS.sprintf("%1.2f", paramList[i]);
                frameRateHighS.append(" fps");
                ui->FrameRateHigh->setText(frameRateHighS);
                ui->FrameRateSlider->setMaximum((int) ((frameRateMaximum - frameRateMinimum) /
                                                   frameRateIncrement) + 1);
                qDebug() << "frameRateSliderMax: " << ui->FrameRateSlider->maximum();
            } else if (i == 2){
                 qDebug() << "update frameRateCurr";
                frameRateCurr = paramList[i];
                ui->FrameRateSlider->setValue((int) ((frameRateCurr - frameRateMinimum) /
                                                     frameRateIncrement));
                QString frameRateCurrS;
                frameRateCurrS.sprintf("%1.2f", frameRateCurr);
                frameRateCurrS.append(" fps");
                ui->FrameRateIndicator->setText(frameRateCurrS);
                qDebug() << "frameRateSliderValue: " << ui->FrameRateSlider->value();
            } else if (i == 3){
                 qDebug() << "update exposureTimeMin";
                exposureTimeMin = paramList[i];
                QString exposureTimeLowS;
                exposureTimeLowS.sprintf("%1.2f", paramList[i]);
                exposureTimeLowS.append(" msec");
                ui->ExposureTimeLow->setText(exposureTimeLowS);
            } else if (i == 4){
                 qDebug() << "update exposureTimeMax";
                exposureTimeMax = paramList[i];
                ui->ExposureTimeSlider->setMaximum((int) ((exposureTimeMax - exposureTimeMin) /
                                                   exposureTimeIncrement) + 1);
                QString exposureTimeHighS;
                exposureTimeHighS.sprintf("%1.2f", paramList[i]);
                exposureTimeHighS.append(" msec");
                ui->ExposureTimeHigh->setText(exposureTimeHighS);
            } else if (i == 5){
                qDebug() << "update exposureTimeCurr";
                exposureTimeCurr = paramList[i];
                QString exposureTimeCurrS;
                exposureTimeCurrS.sprintf("%1.2f", exposureTimeCurr);
                exposureTimeCurrS.append(" msec");
                ui->ExposureTimeIndicator->setText(exposureTimeCurrS);
                ui->ExposureTimeSlider->setValue((int) ((exposureTimeCurr - exposureTimeMin) /
                                                        exposureTimeIncrement));
            } else if (i == 6) {
                qDebug() << "update exposureTimeIncrement";
                exposureTimeIncrement = paramList[i];
                ui->ExposureTimeSlider->setMinimum(0);
                ui->ExposureTimeSlider->setMaximum((int) ((exposureTimeMax - exposureTimeMin) /
                                                   exposureTimeIncrement) + 1);
                ui->ExposureTimeSlider->setValue((int) ((exposureTimeCurr - exposureTimeMin) /
                                                        exposureTimeIncrement));
            }

        }

    }

    delete [] paramList;
}




// updateFrameRate in GUI only
void MainWindow::updateFrameRate(double frameRate){

    frameRateCurr = frameRate;
    QString frameRateCurrS;
    frameRateCurrS.sprintf("%1.2f", frameRateCurr);
    frameRateCurrS.append(" fps");
    ui->FrameRateIndicator->setText(frameRateCurrS);
    ui->FrameRateSlider->setValue((int) ((frameRateCurr - frameRateMinimum) /
                                         frameRateIncrement));
}




// crop ROI
void MainWindow::on_ZoomToSelectionButton_clicked()
{

    QRectF boundingROI = imageRoi->boundingRect();
    saveX = imageRoi->xTopLeft;
    saveY = imageRoi->yTopLeft;
    saveW = imageRoi->width;
    saveH = imageRoi->height;

    // stop video
    videoOpen = 0;
    scene->clear();
    imageRoi = NULL;
    currPixmapItem = NULL;
    rectAdded = true;
    pixmapSet = false;
    emit stopCameraDisplay();

    // resize ROI
    emit changeCameraROI(boundingROI);

    // restart video with no bounding rect
    scene->clear();
    emit startCameraDisplay();
    videoOpen = 1;

    QString currResolution;
    currResolution.sprintf("%d x %d", saveW, saveH);
    ui->ResolutionIndicator->setText(currResolution);

}



// un-crop ROI
void MainWindow::on_ClearSelectionButton_clicked()
{

    QRectF boundingROI = QRectF(0, 0, 1280, 1024);

    // stop video
    videoOpen = 0;
    scene->clear();
    imageRoi = NULL;
    currPixmapItem = NULL;
    rectAdded = false;
    pixmapSet = false;
    emit stopCameraDisplay();

    // resize ROI
    emit changeCameraROI(boundingROI);

    // restart video
    imageRoi = new RoiRect(saveX, saveY, saveW, saveH);
    scene->clear();
    emit startCameraDisplay();
    videoOpen = 1;

    ui->ResolutionIndicator->setText("1280 x 1024");
}



