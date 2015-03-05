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

#include "cameracontroller.h"
#include "motorcontroller.h"
#include "ui_mainwindow.h"
#include "arduinocontroller.h"
#include "uc480_tools.h"
#include "roirect.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"


#define CHANGE_EXPOSURE_TIME 1
#define CHANGE_FRAME_RATE 2
#define CHANGE_PIXEL_CLOCK 3

namespace Ui {
class MainWindow;
}

class MotorController;
class RoiRect;

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
        QGraphicsPixmapItem *currPixmapItem;
        RoiRect *imageRoi;
        BOOL rectAdded;
        BOOL ROIselected;
        cv::VideoWriter *videoWriter;
        int recordingVideo;
        QTimer *videoTimer;
        QTimer *cropTimer;
        QTimer *uncropTimer;
        int valveOpened;
        QString fileName;

        qint64 baseTime;
        qint64 frameMsec;
        double exposureTimeMin;
        double exposureTimeMax;
        double exposureTimeCurr;
        double exposureTimeIncrement;
        double frameRateMinimum;
        double frameRateMaximum;
        double frameRateCurr;
        double frameRateIncrement;
        bool pixmapSet;
        int saveX;
        int saveY;
        int saveW;
        int saveH;
        int currWidth;
        int currHeight;

    signals:
        void setValve(int value);
        void setVent(int value);
        void goToPressure(double desiredPressure, int flag);
        void setMotorPosition(int value); // value is percentage of max stroke
        void startCameraDisplay();
        void stopCameraDisplay();
        void setCameraParams(int param, int value);
        void changeCameraROI(QRectF boundingROI);
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
        void videoStarted();
        void startRecording();
        void stopRecording();
        void finishMeasurement();

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
        void cameraReadySlot();
        void on_ExposureTimeSlider_sliderReleased();
        void on_FrameRateSlider_sliderReleased();
        void on_PixelClockSlider_sliderReleased();
        void on_ZoomToSelectionButton_clicked();
        void on_ClearSelectionButton_clicked();
};

#endif // MAINWINDOW_H




