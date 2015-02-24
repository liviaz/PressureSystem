#include "cameracontroller.h"
#include <QDebug>
#include <QThread>
#include "QLibrary"
#include <iostream>
#include "uc480.h"
#include "uc480_tools.h"
#include "windows.h"
#include "stdafx.h"


// constructor
CameraController::CameraController(QObject *parent) :
    QObject(parent)
{
    windowOpen = 0;
    videoOn = 0;
    cameraOn = 0;
    cameraImage = NULL;
    imageData = NULL;
    memoryID = 0;
    maxPixelClock = 40;
    maxFrameRate = 25;

    // camera model is monochrome
    nColorMode = 0; // RGB32
    nBitsPerPixel = 32;
}

// destructor
CameraController::~CameraController(){

}


// stop camera
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



// initialize camera
void CameraController::initCamera(){

    // initialize timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkFrameRate()));

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



// start video
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

            // set pixel clock to 40
            nRet = is_SetPixelClock(*hCam, 40);

            // set frame rate and exposure time params in GUI
            double paramIn = -1;
            double paramIn2 = -1;
            double paramIn3 = -1;
            double *paramsOut = (double *) calloc(7, sizeof(double));

            nRet = is_GetFrameTimeRange(*hCam, &paramIn, &paramIn2, &paramIn3);
            double fps_min = 1 / paramIn2;
            double fps_max = 1 / paramIn;
            memcpy(paramsOut, &fps_min, sizeof(double)); // min fps
            memcpy(paramsOut + 1, &fps_max, sizeof(double)); // max fps

            nRet = is_GetFramesPerSecond(*hCam, &paramIn);
            memcpy(paramsOut + 2, &paramIn, sizeof(double)); // avg fps

            nRet = is_GetExposureRange(*hCam, &paramIn, &paramIn2, &paramIn3);
            memcpy(paramsOut + 3, &paramIn, sizeof(double)); // min time
            memcpy(paramsOut + 4, &paramIn2, sizeof(double)); // max time
            memcpy(paramsOut + 6, &paramIn3, sizeof(double)); // exposure increment

            nRet = is_SetExposureTime(*hCam, paramIn, &paramIn2); // set exposure time to min
            memcpy(paramsOut + 5, &paramIn2, sizeof(double)); // exposure time


            emit updateCameraParamsInGui(paramsOut);

            // start timer
            timer->start(2000);


        } else {
            qDebug() << "Error when starting video";
        }

    } else {
        qDebug() << "Error: Camera not initialized yet";
    }

}



// stop video
void CameraController::stopVideo(){

    if (videoOn){
        videoOn = 0;
        if (is_StopLiveVideo(*hCam, IS_WAIT) == IS_SUCCESS){
            qDebug() << "Stopped video successfully";
        } else {
            qDebug() << "Error when stopping video";
        }
    }

    if (timer->isActive()){
        timer->stop();
    }
}



// set camera params based on user input
void CameraController::setCameraParams(int param, int value){

    double *paramsOut = (double *) calloc(7, sizeof(double));
    double paramIn;
    double paramIn2 = -1;
    double paramIn3 = -1;

    /* first, send new param to camera
     * then, update camera param ranges in GUI
     * paramList is: [frameRateLow, frameRateHigh, frameRateAvg
     * exposureTimeLow, exposureTimeHigh, exposureTimeActual, exposureTimeIncrement]
     * */
    switch (param) {
        case CHANGE_EXPOSURE_TIME:

            // change exposure time
            is_SetExposureTime(*hCam, value, &paramIn);
            memcpy(paramsOut + 5, &paramIn, sizeof(double));

            is_GetFramesPerSecond(*hCam, &paramIn);
            memcpy(paramsOut + 2, &paramIn, sizeof(double));

            break;
        case CHANGE_FRAME_RATE:

            // change frame rate
            is_SetFrameRate(*hCam, value, &paramIn);
            memcpy(paramsOut + 2, &paramIn, sizeof(double));

            break;

        case CHANGE_PIXEL_CLOCK:

            //set pixel clock
            is_SetPixelClock(*hCam, value);

            is_GetFramesPerSecond(*hCam, &paramIn);
            memcpy(paramsOut + 2, &paramIn, sizeof(double));

            break;

    }


    // emit signal to update params in GUI
    // also make a timer to occasionally update frame rate
    is_GetFrameTimeRange(*hCam, &paramIn, &paramIn2, &paramIn3);
    double fps_min = 1 / paramIn2;
    double fps_max = 1 / paramIn;
    memcpy(paramsOut, &fps_min, sizeof(double));
    memcpy(paramsOut + 1, &fps_max, sizeof(double));

    is_GetExposureRange(*hCam, &paramIn, &paramIn2, &paramIn3);
    memcpy(paramsOut + 3, &paramIn, sizeof(double)); // min time
    memcpy(paramsOut + 4, &paramIn2, sizeof(double)); // max time
    memcpy(paramsOut + 6, &paramIn3, sizeof(double)); // exposure increment

    emit updateCameraParamsInGui(paramsOut);
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



// optimize initial pixel clock and frame rate settings
void CameraController::optimizeCameraParams(){

    // first get optimal pixel clock
    //nRet = is_SetOptimalCameraTiming(*hCam, nColorMode, 10000, &maxPixelClock, &maxFrameRate);

    //qDebug() << "max pixel clock: " << maxPixelClock;
    //qDebug() << "max frame rate: " << maxFrameRate;

    // is_GetFrameTimeRange every time ROI is adjusted

    // Use pixel clock = 40

}



// periodically check frame rate
void CameraController::checkFrameRate(){

    double paramIn = -1;
    is_GetFramesPerSecond(*hCam, &paramIn);
    emit updateFrameRate(paramIn);

}






