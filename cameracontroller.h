#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <QImage>
#include <QString>
#include "opencv/cv.h"
#include "opencv/highgui.h"

class CameraController : public QObject
{
    Q_OBJECT
public:
    explicit CameraController(QObject *parent = 0);
    void closeCamera();
    QImage *cameraImage;
    int videoOn;
    void initCamera();
    ~CameraController();

private:
    IplImage *image;
    int windowOpen;
    QString windowName;


signals:
    void cameraStarted();
    void cameraInitialized();
    void cameraFinished();

public slots:
    void displayVideo(QImage *cameraImgPtr);

};

#endif // CAMERACONTROLLER_H
