#include "cameracontroller.h"
#include <QDebug>
#include "QLibrary"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>

using namespace cv;

// constructor
CameraController::CameraController(QObject *parent) :
    QObject(parent)
{
    qDebug() << "camera object constructed!";
    windowName = "ExampleWindow";
    windowOpen = 0;
}

// destructor
CameraController::~CameraController(){
    if (windowOpen){
        cvReleaseImage(&image);
        cvDestroyWindow(windowName.toStdString().c_str());
        windowOpen = 0;
    }

    emit cameraFinished();
}

void CameraController::closeCamera(){
    if (windowOpen){
        cvReleaseImage(&image);
        cvDestroyWindow(windowName.toStdString().c_str());
        windowOpen = 0;
    }

    emit cameraFinished();

}


void CameraController::initCamera(){

    qDebug() << "about to initialize camera!";
    QString filePath = "C:/Users/Livia/Desktop/IVF/Code/sample code/PressureSystem/PressureSystem/crocoduck.jpg";

    image = cvLoadImage(filePath.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);
    qDebug() << "image read in";

    if (image == NULL){
        qDebug() << "could not open image";
    } else {
        qDebug() << "displaying image: " ;
        cvNamedWindow(windowName.toStdString().c_str(), CV_WINDOW_AUTOSIZE );
        cvShowImage(windowName.toStdString().c_str(), image);
        windowOpen = 1;
    }

    emit cameraInitialized();
}


void CameraController::displayVideo(QImage *cameraImgPtr){

    qDebug() << "camera is displaying video!";
}



