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

class CameraController : public QObject
{
    Q_OBJECT
public:
    explicit CameraController(QObject *parent = 0);
    void setCameraParams(QString param, int value);
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

    // other variables
    int videoOn;
    int cameraOn;
    int windowOpen;
    QString windowName;
    QImage *cameraImage;
    void initializeCameraParams();

signals:
    void cameraInitialized();
    void cameraClosed();
    void updateImage(QImage *cameraImage);

public slots:
    void initCamera();
    void startVideo();
    void stopVideo();
    void closeCamera();
    void eventSignaled(HANDLE h);

private slots:

};

#endif // CAMERACONTROLLER_H
