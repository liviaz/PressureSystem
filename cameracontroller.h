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
    void closeCamera();
    QImage *cameraImage;
    int videoOn;
    cv::Mat image;
    void initCamera();
    ~CameraController();

private:
    int windowOpen;
    QString windowName;
    QTimer *timer;

signals:
    void cameraStarted();
    void cameraInitialized();
    void cameraFinished();

public slots:
    void startVideo(QImage **cameraImgPtr);

private slots:
    void updateVideo();

};

#endif // CAMERACONTROLLER_H
