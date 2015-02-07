#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <QImage>
#include <QString>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QTimer>

class CameraController : public QObject
{
    Q_OBJECT
public:
    explicit CameraController(QObject *parent = 0);
    QImage *cameraImage;
    int videoOn;
    void setCameraParams(QString param, int value);
    ~CameraController();

private:
    int windowOpen;
    QString windowName;
    QTimer *timer;
    void initializeCameraParams();

signals:
    void cameraInitialized();
    void cameraClosed();

public slots:
    void initCamera();
    void startVideo(QImage **cameraImgPtr);
    void stopVideo();
    void closeCamera();

private slots:
    void updateVideo();

};

#endif // CAMERACONTROLLER_H
