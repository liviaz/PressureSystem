#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <QImage>
#include <QString>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QTimer>
#include "windows.h"
#include "Camera/uc480.h"
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#  include <QtCore/QWinEventNotifier>
#else
#  include <QtCore/private/qwineventnotifier_p.h>
#endif

#define MAX_STROKE 50
#define MOTOR_INIT_PCT 70
#define INVALID_PRESSURE -100
#define CHANGE_EXPOSURE_TIME 1
#define CHANGE_FRAME_RATE 2
#define CHANGE_PIXEL_CLOCK 3


class CameraController : public QObject
{
    Q_OBJECT
public:
    explicit CameraController(QObject *parent = 0);
    int resX;
    int resY;
    ~CameraController();

private:

    // camera variables
    char *imageData; // pointer to image memory buffer
    INT memoryID; // image memory buffer ID
    INT nX; // width of video
    INT nY; // height of video
    HCAM *hCam; // camera handle
    SENSORINFO *sInfo; // sensor info struct
    INT nColorMode; // Y8
    INT nBitsPerPixel; // number of bits per pixel
    INT nRet; // return value for camera SDK functions
    HANDLE hEvent;
    QWinEventNotifier *notifier;
    INT maxPixelClock;
    double maxFrameRate;

    // other variables
    int videoOn;
    int cameraOn;
    int windowOpen;
    QImage *cameraImage;
    QTimer *timer;
    void initializeCameraParams();

signals:
    void cameraInitialized();
    void cameraClosed();
    void updateImage(QImage *cameraImage);
    void updateFrameRate(double frameRate);
    void updateCameraParamsInGui(double *paramList);

public slots:
    void initCamera();
    void startVideo();
    void stopVideo();
    void closeCamera();
    void eventSignaled(HANDLE h);
    void optimizeCameraParams();
    void setCameraParams(int param, int value);

private slots:
    void checkFrameRate();

};

#endif // CAMERACONTROLLER_H
