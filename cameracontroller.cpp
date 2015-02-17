#include "cameracontroller.h"
#include <QDebug>
#include <QThread>
#include "QLibrary"
#include <iostream>
#include "Camera/uc480.h"
#include "windows.h"
#include "stdafx.h"



// constructor
CameraController::CameraController(QObject *parent) :
    QObject(parent)
{
    windowName = "ExampleWindow";
    windowOpen = 0;
    videoOn = 0;
    cameraOn = 0;
    cameraImage = NULL;
    imageData = NULL;
    memoryID = 0;

    // camera model is monochrome
    nColorMode = IS_CM_MONO8;
    nBitsPerPixel = 10;

}

// destructor
CameraController::~CameraController(){

}


// stop
void CameraController::closeCamera(){

    // if video is on, stop video
    if (videoOn){
        videoOn = 0;
        nRet = is_StopLiveVideo(*hCam, IS_WAIT);
        nRet = is_ClearSequence(*hCam);
    }

    // if camera is initialized, close it
    if (cameraOn){
        if (imageData!= NULL){
            is_FreeImageMem(*hCam, imageData, memoryID);
        }

        // stop events
        nRet = is_DisableEvent(*hCam, IS_SET_EVENT_FRAME);
        nRet = is_ExitEvent(*hCam, IS_SET_EVENT_FRAME);

        cameraOn = 0;
        imageData = NULL;
        is_ExitCamera(*hCam);
    }

    emit cameraClosed();
}



void CameraController::stopVideo(){

    if (videoOn){
        videoOn = 0;
        if (is_StopLiveVideo(*hCam, IS_WAIT) == IS_SUCCESS){
            qDebug() << "Stopped video successfully";
        } else {
            qDebug() << "Error when stopping video";
        }
    }
}


// initialize camera
void CameraController::initCamera(){


    // first make sure that there is a camera and it can be opened
    INT nNumCam;

    if (is_GetNumberOfCameras(&nNumCam) == IS_SUCCESS && nNumCam == 1) {
        qDebug() << "num cameras: " << nNumCam;

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
        hCam = new HCAM;
        *hCam = pucl->uci[0].dwCameraID;
        INT nRet = is_InitCamera(hCam, NULL);

        // if camera initialized successfully, finish setup and
        // allocate image memory
        if (nRet == IS_SUCCESS){
            initializeCameraParams();
            cameraOn = 1;
            emit cameraInitialized();
            qDebug() << "Camera successfully initialized!";
        } else {
            qDebug() << "Error: can't initialize camera";
        }

        delete [] pucl;

    } else {
        qDebug() << "Error: can't retrieve number of cameras";
    }
}




// point QImage from MainWindow to image.data
void CameraController::startVideo(){

    // is_CaptureVideo
    if (cameraOn){

        nRet = is_InitEvent(*hCam, hEvent, IS_SET_EVENT_FRAME);
        nRet = is_EnableEvent(*hCam, IS_SET_EVENT_FRAME);
        nRet = is_CaptureVideo(*hCam, IS_WAIT);
        BOOL videoStart;

        if (nRet == IS_SUCCESS){
            qDebug() << "Successfully started video";

            while (true){
                is_HasVideoStarted(*hCam, &videoStart);
                if (videoStart == TRUE){
                    break;
                }
            }

            videoOn = 1;
        } else {
            qDebug() << "Error when starting video";
        }

    } else {
        qDebug() << "Error: Camera not initialized yet";
    }

}



// set camera params based on user input
void CameraController::setCameraParams(QString param, int value){


}


// set default camera params after it's initialized
void CameraController::initializeCameraParams(){

    // enable error reporting
    nRet = is_SetErrorReport(*hCam, IS_GET_ERR_REP_MODE);
    nRet = is_SetErrorReport(*hCam, IS_ENABLE_ERR_REP);

    // get sensor info
    sInfo = new SENSORINFO;
    is_GetSensorInfo(*hCam, sInfo);

    nX = sInfo->nMaxWidth;
    nY = sInfo->nMaxHeight;
    qDebug() << "Max image size: " << nX << " by " << nY << " pixels";

    nRet = is_GetColorDepth(*hCam, &nBitsPerPixel, &nColorMode);

    //nBitsPerPixel = 8;
    //nColorMode = 6; // MONO8

    // set color mode and image size
    nRet = is_SetColorMode(*hCam, nColorMode);

    IS_SIZE_2D *initSize = new IS_SIZE_2D;
    initSize->s32Width = nX;
    initSize->s32Height = nY;
    if (is_AOI(*hCam, IS_AOI_IMAGE_SET_SIZE, initSize, sizeof(IS_SIZE_2D)) == IS_SUCCESS){
        qDebug() << "AOI set successfully";
    } else {
        qDebug() << "Error: could not set image size";
    }

    // allocate image memory
    if (is_AllocImageMem(*hCam, nX, nY, nBitsPerPixel, &imageData, &memoryID) != IS_SUCCESS){
        qDebug() << "Error: Image memory allocation failed";
    } else {
        qDebug() << "Memory allocated successfully";
        nRet = is_SetImageMem(*hCam, imageData, memoryID);
        nRet = is_SetDisplayMode(*hCam, IS_SET_DM_DIB);
    }

    // initialize events
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    notifier = new QWinEventNotifier(hEvent);
    connect(notifier, SIGNAL(activated(HANDLE)), this, SLOT(eventSignaled(HANDLE)));
}



// handle windows events : new frame is ready
void CameraController::eventSignaled(HANDLE h) {

    // copy data from camera memory to class variable QImage cameraImage
    if (cameraImage != NULL){
        delete cameraImage;
    }

    cameraImage = new QImage(reinterpret_cast<uchar *>(imageData), nX, nY, QImage::Format_RGB32);
    emit updateImage(cameraImage);
}

