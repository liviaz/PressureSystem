#include "cameracontroller.h"
#include <QDebug>
#include <QThread>
#include "QLibrary"
#include <iostream>
#include "Camera/uc480.h"
#include "windows.h"


using namespace cv;

// constructor
CameraController::CameraController(QObject *parent) :
    QObject(parent)
{
    windowName = "ExampleWindow";
    windowOpen = 0;
    videoOn = 0;
    cameraImage = NULL;

}

// destructor
CameraController::~CameraController(){

}


// stop timer
void CameraController::closeCamera(){

    videoOn = 0;
    timer->stop();
    delete timer;
    emit cameraClosed();
}


void CameraController::stopVideo(){
    videoOn = 0;
}


// initialize camera
void CameraController::initCamera(){

    // start timer!
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateVideo()));
    timer->start(2000);

    // first make sure that there is a camera and it can be opened
    INT nNumCam;

    if (is_GetNumberOfCameras(&nNumCam) == IS_SUCCESS) {
        qDebug() << "num cameras: " << nNumCam;

        if (nNumCam == 1){
            // create new list
            UC480_CAMERA_LIST *pucl;
            pucl = (UC480_CAMERA_LIST*) new BYTE [sizeof(DWORD) + nNumCam * sizeof(UC480_CAMERA_INFO)];
            pucl->dwCount = nNumCam;

            // retrieve camera info
            if (is_GetCameraList(pucl) == IS_SUCCESS){
                for (int i = 0; i < (int) pucl->dwCount; i++){
                    // Test output of camera info on screen
                    qDebug() << "Camera " << i << " Id: " << pucl->uci[i].dwCameraID;
                }
            }

            // now initialize camera
            HCAM *hCam = new HCAM;
            *hCam = pucl->uci[0].dwCameraID;
            INT nRet = is_InitCamera(hCam, NULL);

            if (nRet == IS_SUCCESS){
                initializeCameraParams();
                emit cameraInitialized();
                qDebug() << "Camera successfully initialized!";
            } else {
                qDebug() << "Error: can't initialize camera";
            }

            delete [] pucl;

        } else {
            qDebug() << "Error: " << nNumCam << " cameras found";
        }
    } else {
        qDebug() << "Error: can't retrieve number of cameras";
    }
}



// point QImage from MainWindow to image.data
void CameraController::startVideo(QImage **cameraImgPtrPtr){

    // allocate image memory
    // is_AllocImageMem
    // is_SetImageMem
    // is_GetImageMem


    // is_CaptureVideo

    videoOn = 1;
    *cameraImgPtrPtr = cameraImage;
}



void CameraController::updateVideo(){

    // grab frame
    if (videoOn){

         qDebug() << "updating camera image";

        //memcpy(cameraImage->bits(), image.data, sizeof(uchar) * image.rows * image.cols);
        //cameraImage->fromData(image.data, image.rows * image.cols);
    }
}



void CameraController::setCameraParams(QString param, int value){


}


// set default camera params after it's initialized
void CameraController::initializeCameraParams(){

}



