#include "cameracontroller.h"
#include <QDebug>
#include <QThread>
#include "QLibrary"
#include <iostream>
#include "Camera/uc480.h"

using namespace cv;

// constructor
CameraController::CameraController(QObject *parent) :
    QObject(parent)
{
    qDebug() << "beginning of camera constructor";
    windowName = "ExampleWindow";
    windowOpen = 0;
    videoOn = 0;
    cameraImage = NULL;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateVideo()));
    connect(this, SIGNAL(cameraFinished()), timer, SLOT(deleteLater()));
    timer->start(2000);

    qDebug() << "camera object constructed!";

}

// destructor
CameraController::~CameraController(){

    videoOn = 0;
    if (cvGetWindowHandle(windowName.toStdString().c_str()) != NULL){
        cvDestroyWindow(windowName.toStdString().c_str());
    }

    //timer->stop();
    //delete timer;
    emit cameraFinished();
}

void CameraController::closeCamera(){

    videoOn = 0;
    windowOpen = 0;
    if (cvGetWindowHandle(windowName.toStdString().c_str()) != NULL){
        cvDestroyWindow(windowName.toStdString().c_str());

    }

    //timer->stop();
    //timer->deleteLater();
    emit cameraFinished();

}


void CameraController::initCamera(){

    qDebug() << "about to initialize camera!";
    QString filePath = "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/crocoduck.jpg";
    image = imread(filePath.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);

    //if (image == NULL){
    if (!(image.data)){
        qDebug() << "could not open image";
    } else {
        cvNamedWindow(windowName.toStdString().c_str(), CV_WINDOW_AUTOSIZE);
        //moveWindow(windowName.toStdString().c_str(), 50, 50);
        imshow(windowName.toStdString().c_str(), image);
        windowOpen = 1;
        cameraImage = new QImage(image.data, image.cols, image.rows, QImage::Format_RGB888);
    }



    emit cameraInitialized();
}



void CameraController::initVideoCamera(int camera_id){

    QString videoPath = "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/E1.avi";

    INT nNumCam;
    if (is_GetNumberOfCameras(&nNumCam) == IS_SUCCESS) {
        qDebug() << "num cameras: " << nNumCam;
    }




//        VideoCapture *cap = new VideoCapture;
//        cap->open(i);// = new VideoCapture(camera_id);

//        if (cap->isOpened()){
//            qDebug() << "Video opened!";
//        } else {
//            qDebug() << "Could not open video";
//        }


}



// point QImage from MainWindow to image.data
void CameraController::startVideo(QImage **cameraImgPtrPtr){

    videoOn = 1;
    *cameraImgPtrPtr = cameraImage;
}

void CameraController::updateVideo(){

    qDebug() << "updating camera image";

    if (videoOn){
        //memcpy(cameraImage->bits(), image.data, sizeof(uchar) * image.rows * image.cols);
        //cameraImage->fromData(image.data, image.rows * image.cols);
    }
}



