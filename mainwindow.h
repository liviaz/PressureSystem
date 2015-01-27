#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QImage>
#include <QMessageBox>
#include <QByteArray>
#include <QTimer>

#include "motorcontroller.h"
#include "ui_mainwindow.h"
#include "arduinocontroller.h"
#include "cameracontroller.h"

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
        double measurePressureValue;
        double balancePressureValue;
        MotorController *mc;
        ArduinoController *ac;
        CameraController *cc;
        int arduinoStartable;
        int motorStartable;
        int videoStartable;
        QImage *cameraImagePtr;
        //QTimer *timer;
        //QGraphicsScene *scene;
        //QGraphicsPixmapItem *item;


    signals:
        void setValve(int value);
        void setVent(int value);
        void goToPressure(double desiredPressure, int flag);
        void setMotorPosition(int value); // value is percentage of max stroke
        void startCameraDisplay(QImage **imgPtr);

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
        //void updateCameraImage();
        void on_OpenCameraButton_clicked();
        void cameraReadySlot();
        void on_videoCameraOpenButton_clicked();
};

#endif // MAINWINDOW_H




