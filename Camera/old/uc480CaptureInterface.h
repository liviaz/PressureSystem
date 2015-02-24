// The uc480 Capture Device Filter supports a range of standard interfaces.
// These interfaces are:
// The Filter exposes:		IAMVideoProcAmp
//							IAMVideoControl
//							IAMDroppedFrames
//							IAMFilterMiscFlags
//							IKsPropertySet
//							Iuc480Capture		    - specific uc480 interface
//							Iuc480CaptureEx          - specific uc480 interface
//                          Iuc480AutoFeatures	    - specific uc480 interface
//							Iuc480FaceDetection      - specific uc480 interface
//							Iuc480ImageStabilization - specific uc480 interface
//							ISpecifyPropertyPages
// The Capture Pin exposes:	IAMCameraControl
//							IKsPropertySet
//							IAMStreamConfig
//							Iuc480CapturePin		    - specific uc480 interface
//							ISpecifyPropertyPages
// Some functionalities of the cameras are not mentioned in this standards.
// Therefore this file defines some additional interfaces, providing control
// over the missing features.

#ifndef _UC480_CAPTURE_INTERFACE_
#define _UC480_CAPTURE_INTERFACE_

#include <initguid.h>

const char uc480CaptureInterfaceVersion[] = "3.0.0";


// ============================================================================
/*! \defgroup Iuc480CapturePin uc480 Capture Pin Interface
 *  Proprietary interface for extra functionality exposed by the capture pin.
 *  It controls mainly the pixelclock settings for sensor read in
 * \{
 */
// ============================================================================

// {67030826-2EE0-44e7-BE1A-6A3BDB5B47FE}
DEFINE_GUID(IID_Iuc480CapturePin, 
            0x67030826, 0x2ee0, 0x44e7, 0xbe, 0x1a, 0x6a, 0x3b, 0xdb, 0x5b, 0x47, 0xfe);

interface Iuc480CapturePin : public IUnknown
{
	//! Returns the sum (in MHz) of pixelclock.
	/*!
	 *	\param 	lClock      Receives the overall pixelclock sum.
	 *	\return	HRESULT     0 on success, error code otherwise.
	 *	\see    GetPixelClock
	 */
	STDMETHOD(GetUsedBandwith)(long *lClock) = 0;

    //! Returns the pixelclock for the connected camera.
    /*!
    *	\param 	plClock     Receives the current pixel clock.
    *	\return	HRESULT     0 on success, error code otherwise.
    *	\see    GetPixelClockRange, SetPixelClock
    */
    STDMETHOD(GetPixelClock)(long *plClock) = 0;

    //! Returns the min, max and default value for the pixelclock.
    /*!
     *	\param 	plMin       Receives the minimum possible pixel clock.
     *	\param 	plMax       Receives the maximum possible pixel clock.
     *	\param 	plDefault   Receives the default pixel clock value.
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see    GetPixelClock, SetPixelClock
     */
    STDMETHOD(GetPixelClockRange)(long *plMin, long *plMax, long *plDefault) = 0;

    //! Sets the Pixelclock (in MHz) for the connected device.
    /*!
     *	\param 	lClock      The pixel clock in MHz to set.
     *	\return	HRESULT     0 on success, CO_E_NOT_SUPPORTED if not supported, error code otherwise.
     *	\see    GetPixelClock, GetPixelClockRange
     */
    STDMETHOD(SetPixelClock)(long lClock) = 0;

    //! Queries which color mode to use when RGB8 mediatype is selected.
    /*!
     *	\param 	plMode      Receives the currently selected RGB8 colormode.
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see    SetRGB8ColorMode
     */
    STDMETHOD(GetRGB8ColorMode)(long *plMode) = 0;

    //! Determines which color mode to use when RGB8 mediatype is selected.
    /*!
     *      Specifies whether Y8 or raw bayer pattern is used with RGB8 mode
     *		possible values are 11 for raw bayer pattern (on bayer color
     *      cameras), or 6 for monochrome images.
     *	\param 	lMode       Specifies the color mode used for RGB8 mode.
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see    GetRGB8ColorMode
     */
    STDMETHOD(SetRGB8ColorMode)(long lMode) = 0;

	//! Queries the current possible min, max and interval for exposure time.
	/*! 
     *      Gets the actual min, max and interval values for exposure time.
     *      Values are given in us unit.
	 *	\param 	plMinExp    Receives the minimum possible exposure time.
	 *	\param 	plMaxExp    Receives the maximum possible exposure time.
     *	\param 	plInterval  Receives the current possible step width.
     *	\return	HRESULT     0 on success, error code otherwise.
	 *	\see    GetExposureTime, SetExposureTime, SetPixelClock
     *
     *  \note   This range may change depending on framerate and pixelclock
     *          settings.
	 */
	STDMETHOD(GetExposureRange)(long *plMinExp, long *plMaxExp, long *plInterval) = 0;

    //! Queries the current exposure time
    /*!
     *	\param 	plExp       Receives the current exposure time in us.
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see    GetExposureRange, SetExposureTime
     */
    STDMETHOD(GetExposureTime)(long *plExp ) = 0;

	//Sets the Exposure Time, more finer than the IAMCameraControl Interface function can do (2^n vs )
	// the parameter is divided by 1000.
    //! Sets the exposure time of the camera.
    /*!
     *		This function sets the exposure time in units of Microseconds and
     *      thus allows a finer exposure time granularity than the function of
     *      the IAMCameraControl Interface does. (2^n seconds vs. x us).
     *	\param 	lExp        Specifies the exposure time to use (in us).
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see    GetExposureTime, GetExposureRange
     */
    STDMETHOD(SetExposureTime)(long lExp) = 0;
};

/*!
 * \}
 */	// end of group uc480CaptureInterface

//structs needed by some functions
#ifndef DS_EXPORT
#   define DS_EXPORT
#   ifdef CAMERAINFO
#       undef CAMERAINFO
#   endif
typedef struct
{
  char          SerNo[12];    // e.g. "12345-1234"  (11 char)
  char          ID[20];       // e.g. "OEM"
  char          Version[10];  // e.g. "V1.00"  (9 char)
  char          Date[12];     // e.g. "11.11.1999" (11 char)
  unsigned char Select;       // 0 (contains board select number for multi board support)
  unsigned char Type;         // 1 (contains board type)
  char          Reserved[8];  // (7 char)
} CAMERAINFO, *PCAMERAINFO;

#   ifdef _SENSORINFO
#       undef _SENSORINFO
#   endif
#   ifdef SENSORINFO
#       undef SENSORINFO
#   endif
typedef struct _SENSORINFO
{
  WORD          SensorID;           
  char          strSensorName[32]; 
  char          nColorMode;         // e.g. IS_COLORMODE_BAYER
  DWORD         nMaxWidth;          // e.g. 1280
  DWORD         nMaxHeight;         // e.g. 1024
  BOOL          bMasterGain;        // e.g. FALSE
  BOOL          bRGain;             // e.g. TRUE
  BOOL          bGGain;             // e.g. TRUE
  BOOL          bBGain;             // e.g. TRUE
  BOOL          bGlobShutter;       // e.g. TRUE
  char			Reserved[16];		// not used
} SENSORINFO, *PSENSORINFO;
#endif  // DS_EXPORT

// ============================================================================
/*! \defgroup Iuc480Capture uc480 Capture Interface
 *  Proprietary interfaces for extra functionality exposed by the capture filter
 *  It adds functions for hot pixel and whitebalance user control as well as
 *  parameter persistence.
 * \{
 */
// ============================================================================

// {7BDFA675-E6BF-449e-8349-5F62AE9E0023}
DEFINE_GUID(IID_Iuc480Capture, 
            0x7bdfa675, 0xe6bf, 0x449e, 0x83, 0x49, 0x5f, 0x62, 0xae, 0x9e, 0x0, 0x23);

interface Iuc480Capture : public IUnknown
{
    // OBSOLETE: function will return CO_E_NOT_SUPPORTED and do nothing.
    STDMETHOD(GetWhiteBalanceMultipliers)(long *plRed, long *plGreen, long *plBlue) = 0;

    // OBSOLETE: function will return CO_E_NOT_SUPPORTED and do nothing.
    STDMETHOD(SetWhiteBalanceMultipliers)(long lRed, long lGreen, long lBlue) = 0;

    // Queries the number of connected camera devices.
    // OBSOLETE
	STDMETHOD(GetNumberOfCameras)(long *plNr) = 0;

    //! Returns the device info for the connected camera as a pair of CAMERAINFO and SENSORINFO
    /*!
    *	\param 	psInfo      Receives the SENSORINFO
    *	\param 	pcInfo      Receives the CAMERAINFO
    *	\return	HRESULT     0 on success, error code otherwise.
    */
	STDMETHOD(GetDeviceInfo)(SENSORINFO *psInfo, CAMERAINFO *pcInfo) = 0;

	//! Queries the Version of the installed uc480 Driver files
	/*!
     *	\param 	pVersion    Receives the Version of connected cameras.
     *	\return	HRESULT     0 on success, error code otherwise.
     *
     *  \note   This is not the Version of the uc480 capture device filter but
     *          the Version of the underlying driver files.
	 */
	STDMETHOD(GetDLLVersion)(long *pVersion) = 0;

	// Returns a pair of lists, containing of CAMERAINFO and SENSORINFO structures, which holds
	// information of the available cameras.
	// OBSOLETE
	STDMETHOD(GetListOfCameras)(CAMERAINFO **cInfo, SENSORINFO **sInfo, long *lNr) = 0;

	// Tries to connect the filter to another camera.
	// OBSOLETE
	STDMETHOD(ConnectToCamera)(long lIndex) = 0;

	//! Activates or deactivates the hot pixel correction.
	/*!
     *	\param 	lEnable     Set to 1 to activate or 0 to deactivate correction.
     *	\return	HRESULT     0 on success, error code otherwise.
	 *	\see    GetBadPixelCorrection
	 */
	STDMETHOD(SetBadPixelCorrection)(long lEnable) = 0;

	//! Queries the current state of the hot pixel correction unit.
	/*!	 
     *	\param 	plEnable    Receives 1 if hot pixel correction is enabled.
     *	\return	HRESULT     0 on success, error code otherwise.
	 *	\see    SetBadPixelCorrection
	 */
	STDMETHOD(GetBadPixelCorrection)(long *plEnable) = 0;

    //! Loads previous stored camera settings.
    /*!	 
    *	\return	HRESULT     0 on success, error code otherwise.
    *	\see    SaveSettings
    */
    STDMETHOD(LoadSettings)(void) = 0;
    
    //! Stores the current set camera settings in the registry.
    /*!	 
    *	\return	HRESULT     0 on success, error code otherwise.
    *	\see    LoadSettings
    *
    *   \note Data will be stored individual for each uc480 model.
    */
	STDMETHOD(SaveSettings)(void) = 0;

	//! Resets the camera parameters to the driver defaults.
	/*!
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see     LoadParameters, SaveParameters
     *
     *  \note   You may not be able to reset parameters while the filter is 
     *          connected or running.
	 */
    STDMETHOD(ResetDefaults)(void) = 0;
};


// {E179D0EE-E0BB-42d6-BAB9-FFDF2277E887}
DEFINE_GUID(IID_Iuc480CaptureEx, 
            0xe179d0ee, 0xe0bb, 0x42d6, 0xba, 0xb9, 0xff, 0xdf, 0x22, 0x77, 0xe8, 0x87);

interface Iuc480CaptureEx : public Iuc480Capture
{
    //! Activates or deactivates the additional gain amplification
    /*!
    *  \param  lGainBoost  Set to 1 to activate or 0 to deactivate extra amplification.
    *  \return HRESULT     0 on success, error code otherwise.
    *  \see    GetGainBoost
    */
    STDMETHOD(SetGainBoost)(long lGainBoost) = 0;

    //! Queries the current state of the extra amplification.
    /*!
    *  \param  plGainBoost Receives 1 if extra amplification is enabled.
    *  \return HRESULT     0 on success, error code otherwise.
    *  \see    SetGainBoost
    */
    STDMETHOD(GetGainBoost)(long *plGainBoost) = 0;

    //! Activates or deactivates the hardware gamma.
    /*!
    *  \param  lHWGamma    Set to 1 to activate or 0 to deactivate hw gamma.
    *  \return HRESULT     0 on success, error code otherwise.
    *  \see    GetHardwareGamma
    */
    STDMETHOD(SetHardwareGamma)(long lHWGamma) = 0;

    //! Queries the current state of hardware gamma.
    /*!
    *  \param  plHWGamma    Receives 1 if hw gamma is enabled.
    *  \return HRESULT      0 on success, error code otherwise.
    *  \see    SetHardwareGamma
    */
    STDMETHOD(GetHardwareGamma)(long *plHWGamma) = 0;

    //! Load the parameters from a file or camera EEPRom userset.
    /*!
    *		Detailed description missing
    *	\param 	cszFileName Filename or EEPRom userset to load parameters from.
    *	\return	HRESULT     0 on success, error code otherwise.
    *	\see    SaveParameters, ResetDefaults
    *
    *  \note   You may not be able to load parameters while the filter is 
    *          connected or running.
    */
    STDMETHOD(LoadParameters)(const char* cszFileName) = 0;

    //! Stores the current parameters to file or camera EEPRom userset.
    /*!
    *	\param 	cszFileName Filename or EEPRom userset to store parameters to.
    *	\return	HRESULT     0 on success, error code otherwise.
    *	\see    LoadParameters, ResetDefaults
    */
    STDMETHOD(SaveParameters)(const char* cszFileName) = 0;
};


/*!
* \}
*/	// end of group uc480CaptureInterface


// ============================================================================
/*! \defgroup Iuc480AutoFeatures uc480 Auto Feature Interface
*  Proprietary interface for uc480 auto feature control exposed by the capture
*  filter. Allows a DirectShow based program to control and query all auto
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

// {666A7ED1-C64F-47e8-A8D2-E381FD353315}
DEFINE_GUID(IID_Iuc480AutoFeatures, 
            0x666a7ed1, 0xc64f, 0x47e8, 0xa8, 0xd2, 0xe3, 0x81, 0xfd, 0x35, 0x33, 0x15);

interface Iuc480AutoFeatures : public IUnknown
{
    //! Specifies the brightness reference value which should be achieved by auto gain and auto exposure.
    /*!
     *  \param  lReference  The reference value the controller should reach.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoBrightnessReference, SetAutoBrightnessMaxExposure, SetAutoBrightnessMaxGain
     */
    STDMETHOD(SetAutoBrightnessReference)(long lReference) = 0;

    //! Queries the actual set reference value for auto brightness control.
    /*!
     *  \param  plReference Receives the current value for reference.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessReference
     */
    STDMETHOD(GetAutoBrightnessReference)(long* plReference) = 0;

    //! Upper limit of the exposure time when used to control the image brightness automatically.  
    /*!
     *  \param  lMaxExposure Maximum exposure time (in us Units) the controller is allowed to set.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see GetAutoBrightnessMaxExposure, SetAutoBrightnessMaxGain, SetAutoBrightnessReference
     */
    STDMETHOD(SetAutoBrightnessMaxExposure)(long lMaxExposure) = 0;

    //! Queries the actual set upper limit of automatic controlled exposure time.
    /*!
     *  \param  plMaxExposure Receives the currently allowed maximum exposure time (us Units)
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessMaxExposure
     */
    STDMETHOD(GetAutoBrightnessMaxExposure)(long* plMaxExposure) = 0;

    //! Upper limit of gain when used to control the image brightness automatically.
    /*!
     *  \param  lMaxGain    Maximum master gain value the controller is allowed to set.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessMaxExposure, GetAutoBrightnessMaxGain, SetAutoBrightnessReference
     */
    STDMETHOD(SetAutoBrightnessMaxGain)(long lMaxGain) = 0;

    //! Queries the actual set upper limit of automatic controlled master gain amplifier. 
    /*!
     *  \param  plMaxGain   Receives the currently allowed maximum gain value.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessMaxGain
     */
    STDMETHOD(GetAutoBrightnessMaxGain)(long* plMaxGain) = 0;

    //! Controls the percentage of examined images for the automatic brightness control unit.
    /*!
     *  \param  lSpeed      The desired speed in a range of 1%(slow) to 100%(fast).
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoBrightnessSpeed, SetAutoBrightnessReference
     */
    STDMETHOD(SetAutoBrightnessSpeed)(long lSpeed) = 0;

    //! Queries the actual set rate at which image brightness is examined. 
    /*!
     *  \param  plSpeed     Receives the currently set examination speed.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessSpeed
     */
    STDMETHOD(GetAutoBrightnessSpeed)(long* plSpeed) = 0;

    //! Specifies the area of interest within the image in which the brightness should be examined.
    /*!
     *  \param  lXPos       Left bound of the area of interest.
     *  \param  lYPos       Upper bound of the area of interest.
     *  \param  lWidth      Width of the area of interest.
     *  \param  lHeight     Height of the area of interest.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoBrightnessAOI
     */
    STDMETHOD(SetAutoBrightnessAOI)(long lXPos, long lYPos, long lWidth, long lHeight) = 0;

    //! Queries the actual used area of interest in which the brightness is examined.
    /*!
     *  \param  plXPos      Receives the left bound.
     *  \param  plYPos      Receives the upper bound.
     *  \param  plWidth     Receives the width.
     *  \param  plHeight    Receives the height.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessAOI
     */
    STDMETHOD(GetAutoBrightnessAOI)(long* plXPos, long* plYPos, long* plWidth, long* plHeight) = 0;

    //! Specifies relative offsets between the individual color channels when used by the automatic whitebalance control unit.
    /*!
     *  \param  lRedOffset  Offset for the red gain channel relative to the green one.
     *  \param  lBlueOffset Offset for the blue gain channel relative to the green one.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoWBGainOffsets, SetAutoWBGainRange
     */
    STDMETHOD(SetAutoWBGainOffsets)(long lRedOffset, long lBlueOffset) = 0;

    //! Queries the actual set color channel offsets for automatic whitebalance.
    /*!
     *  \param  plRedOffset  Receives the red to green channel offset.
     *  \param  plBlueOffset  Receives the blue to green channel offset.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoWBGainOffsets
     */
    STDMETHOD(GetAutoWBGainOffsets)(long* plRedOffset, long* plBlueOffset) = 0;

    //! Limits the range the automatic whitebalance controller unit is allowed to use when adjusting the RGB gain channels.
    /*!
     *  \param  lMinRGBGain  Minimum allowed gain value.
     *  \param  lMaxRGBGain  Maximum allowed gain value.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoWBGainRange, SetAutoWBGainOffsets
     */
    STDMETHOD(SetAutoWBGainRange)(long lMinRGBGain, long lMaxRGBGain) = 0;

    //! Queries the actual allowed gain range for the automatic whitebalance controller unit.
    /*!
     *  \param  plMinRGBGain  Receives the currently allowed minimal gain value.
     *  \param  plMaxRGBGain  Receives the currently allowed maximal gain value.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoWBGainRange
     */
    STDMETHOD(GetAutoWBGainRange)(long* plMinRGBGain, long* plMaxRGBGain) = 0;

    //! Controls the percentage of examined images for the automatic whitebalance control unit.
    /*!
     *  \param  lSpeed      The desired speed in a range of 1%(slow) to 100%(fast).
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoWBGainRange, SetAutoWBGainOffsets
     */
    STDMETHOD(SetAutoWBSpeed)(long lSpeed) = 0;

    //! Queries the actual set rate at which the images whitebalance is examined. 
    /*!
     *  \param  plSpeed     Receives the currently set examination speed.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoWBSpeed
     */
    STDMETHOD(GetAutoWBSpeed)(long* plSpeed) = 0;

    //! Specifies the area of interest within the image in which the whitebalance should be examined.
    /*!
     *  \param  lXPos       Left bound of the area of interest.
     *  \param  lYPos       Upper bound of the area of interest.
     *  \param  lWidth      Width of the area of interest.
     *  \param  lHeight     Height of the area of interest.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoWBAOI
     */
    STDMETHOD(SetAutoWBAOI)(long lXPos, long lYPos, long lWidth, long lHeight) = 0;

    //! Queries the actual used area of interest in which the whitebalance is examined.
    /*!
     *  \param  plXPos      Receives the left bound.
     *  \param  plYPos      Receives the upper bound.
     *  \param  plWidth     Receives the width.
     *  \param  plHeight    Receives the height.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoWBAOI
     */
    STDMETHOD(GetAutoWBAOI)(long* plXPos, long* plYPos, long* plWidth, long* plHeight) = 0;
};
/*!
 * \}
 */	// end of group Iuc480AutoFeatures


// ============================================================================
/*! \defgroup Iuc480FaceDetection uc480 Face Detection Interface
*  Proprietary interface for uc480 face detection control exposed by the capture
*  filter. Allows a DirectShow based program to control and query the face detection
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

#ifndef DS_EXPORT
#   define DS_EXPORT
#   ifdef _UC480TIME
#       undef _UC480TIME
#   endif
#   ifdef UC480TIME
#       undef UC480TIME
#   endif
/*!
 * \brief uc480 time data type.
 * Used in \see FDT_INFO_EL.
 */
typedef struct _UC480TIME
{
    WORD      wYear;
    WORD      wMonth;
    WORD      wDay;
    WORD      wHour;
    WORD      wMinute;
    WORD      wSecond;
    WORD      wMilliseconds;
    BYTE      byReserved[10];
} UC480TIME;
#endif  /* DS_EXPORT */

#ifndef DS_EXPORT
#   define DS_EXPORT
#   ifdef S_FDT_INFO_EL
#       undef S_FDT_INFO_EL
#   endif
#   ifdef FDT_INFO_EL
#       undef FDT_INFO_EL
#   endif
/*!
 * \brief uc480 face detection info element data type.
 * Info on a single detected face as listed by \see FDT_INFO_LIST.
 */
typedef struct S_FDT_INFO_EL
{
    INT nFacePosX;              /*< Start X position.                                                                */
    INT nFacePosY;              /*< Start Y position.                                                                */
    INT nFaceWidth;             /*< Face width.                                                                      */
    INT nFaceHeight;            /*< Face height.                                                                     */
    INT nAngle;                 /*< Face Angle (0...360° clockwise, 0° at twelve o'clock position. -1: undefined ).  */
    UINT nPosture;              /*< Face posture.                                                                    */
    UC480TIME TimestampSystem;  /*< System time stamp (device query time) .                                          */
    UINT64 nReserved;           /*< Reserved for future use.                                                         */
    UINT nReserved2[4];         /*< Reserved for future use.                                                         */
} FDT_INFO_EL;
#endif  /* DS_EXPORT */

#ifndef DS_EXPORT
#   define DS_EXPORT
#   ifdef S_FDT_INFO_LIST
#       undef S_FDT_INFO_LIST
#   endif
#   ifdef FDT_INFO_LIST
#       undef FDT_INFO_LIST
#   endif
/*!
 * \brief uc480 face detection info list data type.
 * List of detected faces, lists \see FDT_INFO_EL objects.
 */
typedef struct S_FDT_INFO_LIST
{
    UINT nSizeOfListEntry;      /*< Size of one list entry in byte(in).  */
    UINT nNumDetectedFaces;     /*< Number of detected faces(out).       */
    UINT nNumListElements;      /*< Number of list elements(in).         */ 
    UINT nReserved[4];          /*< reserved for future use(out).        */ 
    FDT_INFO_EL FaceEntry[1];   /*< First face entry.                    */
} FDT_INFO_LIST;
#endif  /* DS_EXPORT */

// {E122A994-FC4D-445b-B21C-308B674844E0}
DEFINE_GUID(IID_Iuc480FaceDetection, 
            0xe122a994, 0xfc4d, 0x445b, 0xb2, 0x1c, 0x30, 0x8b, 0x67, 0x48, 0x44, 0xe0);

interface Iuc480FaceDetection : public IUnknown
{
    /*!
     * \brief Query for support of the face detection feature.
     * \param pbSupported   output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of the face detection feature.
     * \param pbEnabled output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_IsEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of the face detection feature.
     * \param bEnable   new 'enabled' status.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_Enable)(bool bEnable) = 0;

    /*!
     * \brief Query the current 'suspended' status of the face detection feature.
     * \param pbSuspended   output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_IsSuspended)(bool* pbSuspended) = 0;

    /*!
     * \brief Set the 'suspended' status of the face detection feature.
     * \param bSuspend  new 'suspended' status.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_Suspend)(bool bSuspend) = 0;

    /*!
     * \brief Query the current enabled status of the face detection's 'search angle' subfeature.
     * \param pbEnabled output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_IsSearchAngleEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of the face detection's 'search angle' feature.
     * \param bSuspend  new 'suspended' status.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_EnableSearchAngle)(bool bEnable) = 0;

    /*!
     * \brief Query the current search angle.
     * \param pulAngle  output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetSearchAngle)(long* pulAngle) = 0;

    /*!
     * \brief Set the new search angle.
     * \param ulAngle   output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_SetSearchAngle)(long ulAngle) = 0;

    /*!
     * \brief Query the last determined face list.
     * \param pFaceList output location for result: preallocated object of type \see FDT_INFO_LIST.
     * \param ulSize    size of pFaceList in bytes.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetFaceList)(void* pFaceList, unsigned long ulSize) = 0;

    /*!
     * \brief Query the last determined number of faces.
     * \param pulNumFaces   output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetNumFaces)(unsigned long* pulNumFaces) = 0;

    /*!
     * \brief Query the maximum number of faces that the feature can detect in an image.
     * \param pulMaxNumFaces    output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetMaxNumFaces)(unsigned long* pulMaxNumFaces) = 0;

    /*!
     * \brief Query the current maximum number of overlay drawings that the feature will show in an image.
     * \param pulMaxNumOvl  output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetMaxNumOvl)(unsigned long* pulMaxNumOvl) = 0;

    /*!
     * \brief Set the new maximum number of overlay drawings that the feature will show in an image.
     * \param ulMaxNumOvl   the new setting.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_SetMaxNumOvl)(unsigned long ulMaxNumOvl) = 0;

    /*!
     * \brief Query the current linewidth for the overlay drawings.
     * \param pulLineWidthOvl   output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetLineWidthOvl)(unsigned long* pulLineWidthOvl) = 0;

    /*!
     * \brief Set the new linewidth for the overlay drawings.
     * \param ulLineWidthOvl    the new setting.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_SetLineWidthOvl)(unsigned long ulLineWidthOvl) = 0;

    /*!
     * \brief Query the resolution.
     * \param pulHorzRes    output location for result horizontal resolution.
     * \param pulVertRes    output location for result vertical resolution.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetResolution)(unsigned long* pulHorzRes, unsigned long* pulVertRes) = 0;

    /*!
     * \brief Generic access to the face detection feature.
     * \return E_NOTIMPL
     * \note the generic access interface is provided for future use.
     */
    STDMETHOD(FDT_GenericAccess)(unsigned long ulCommand, void* pParam, unsigned long ulSize) = 0;
};
/*!
 * \}
 */	// end of group Iuc480FaceDetection


// ============================================================================
/*! \defgroup Iuc480ImageStabilization uc480 Image Stabilization Interface
*  Proprietary interface for uc480 image stabilization control exposed by the capture
*  filter. Allows a DirectShow based program to control and query the image stabilization
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

// {D24BA219-7426-45b9-960A-126246ED0897}
DEFINE_GUID(IID_Iuc480ImageStabilization, 
            0xd24ba219, 0x7426, 0x45b9, 0x96, 0xa, 0x12, 0x62, 0x46, 0xed, 0x8, 0x97);

interface Iuc480ImageStabilization : public IUnknown
{
    /*!
     * \brief Query for support of the image stabilization feature.
     * \param pbSupported   output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(ImgStab_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of the image stabilization feature.
     * \param pbEnabled output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(ImgStab_IsEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of the image stabilization feature.
     * \param bEnable   new 'enabled' status.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(ImgStab_Enable)(bool bEnable) = 0;

    /*!
     * \brief Generic access to the image stabilization feature.
     * \return E_NOTIMPL
     * \note the generic access interface is provided for future use.
     */
    STDMETHOD(ImgStab_GenericAccess)(unsigned long ulCommand, void* pParam, unsigned long ulSize) = 0;
};
/*!
 * \}
 */	// end of group Iuc480ImageStabilization

#endif  // #ifndef _UC480_CAPTURE_INTERFACE_
