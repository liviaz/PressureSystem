#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QImage>
#include <QMessageBox>
#include <QByteArray>
#include <QTimer>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDateTime>

#include "motorcontroller.h"
#include "ui_mainwindow.h"
#include "arduinocontroller.h"
#include "cameracontroller.h"
#include "uc480_tools.h"

#define CHANGE_EXPOSURE_TIME 1
#define CHANGE_FRAME_RATE 2
#define CHANGE_PIXEL_CLOCK 3

namespace Ui {
class MainWindow;
}

class MotorController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;  
        int portOpen;
        int motorOpen;
        int cameraOpen;
        int videoOpen;
        int cameraClosable;
        double measurePressureValue;
        double balancePressureValue;
        MotorController *mc;
        ArduinoController *ac;
        CameraController *cc;
        int arduinoStartable;
        int motorStartable;
        int videoStartable;
        QImage *cameraImagePtr;
        QGraphicsScene *scene;
        qint64 baseTime;

        double exposureTimeMin;
        double exposureTimeMax;
        double exposureTimeCurr;
        double exposureTimeIncrement;

        double frameRateMinimum;
        double frameRateMaximum;
        double frameRateCurr;
        double frameRateIncrement;

        int totalFrames;
        int framesDropped;


    signals:
        void setValve(int value);
        void setVent(int value);
        void goToPressure(double desiredPressure, int flag);
        void setMotorPosition(int value); // value is percentage of max stroke
        void startCameraDisplay();
        void stopCameraDisplay();
        void setCameraParams(int param, int value);
        void initCamera();
        void closeCamera();

    public slots:
        void motorClosedSlot();
        void motorOpenSlot();
        void motorReadySlot();
        void arduinoOpenSlot();
        void arduinoClosedSlot();
        void arduinoReadySlot();
        void pressureUpdatedSlot();
        void balanceFinished(int successful, int flag);
        void updateMotorPosition(double value);
        void cameraFinishedInit();
        void cameraFinishedClose();
        void updateFrameRate(double frameRate);
        void cameraFrameReceived(QImage *imgFromCamera);
        void updateCameraParamsInGui(double *paramList);

    private slots:
        void on_stopButton_clicked();
        void on_startButton_clicked();
        void on_valveButton_clicked();
        void on_ventButton_clicked();
        void on_horizontalSlider_sliderReleased();
        void on_initializeButton_clicked();
        void on_balanceButton_clicked();
        void on_measureButton_clicked();
        void on_balancePressureDouble_valueChanged(double arg1);
        void on_measurePressureDouble_valueChanged(double arg1);
        void on_StartVideoButton_clicked();
        void cameraReadySlot();
        void on_InitCameraButton_clicked();
        void on_ExposureTimeSlider_sliderReleased();
        void on_FrameRateSlider_sliderReleased();

        void on_PixelClockSlider_sliderReleased();
};

#endif // MAINWINDOW_H




