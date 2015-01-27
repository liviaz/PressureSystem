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
//							Iuc480SensorAWB          - specific uc480 interface
//							Iuc480AutoContrast       - specific uc480 interface
//							Iuc480AutoBacklight      - specific uc480 interface
//							Iuc480AntiFlicker        - specific uc480 interface
//							Iuc480ScenePreset        - specific uc480 interface
//							Iuc480DigitalZoom        - specific uc480 interface
//							Iuc480Saturation         - specific uc480 interface
//							Iuc480Sharpness          - specific uc480 interface
//							Iuc480ColorTemperature   - specific uc480 interface
//							Iuc480TriggerDebounce    - specific uc480 interface
//							Iuc480Photometry         - specific uc480 interface
//							Iuc480AutoFramerate      - specific uc480 interface
//							Iuc480Flash              - specific uc480 interface
//							Iuc480Resample           - specific uc480 interface
//                          Iuc480Trigger            - specific uc480 interface
//							ISpecifyPropertyPages
// The Capture Pin exposes:	IAMCameraControl
//							IKsPropertySet
//							IAMStreamConfig
//							Iuc480CapturePin		 - specific uc480 interface
//                          Iuc480AOI                - specific uc480 interface
//							ISpecifyPropertyPages
// Some functionalities of the cameras are not mentioned in this standards.
// Therefore this file defines some additional interfaces, providing control
// over the missing features.


#ifndef UC480_CAPTURE_INTERFACE
#define UC480_CAPTURE_INTERFACE

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
    /*!
	 *  \brief  Returns the sum (in MHz) of pixelclock.
	 *	\param 	lClock      Receives the overall pixelclock sum.
	 *	\return	HRESULT     0 on success, error code otherwise.
	 *	\see    GetPixelClock
	 */
	STDMETHOD(GetUsedBandwith)(long *lClock) = 0;

    /*!
    *   \brief  Returns the pixelclock for the connected camera.
    *	\param 	plClock     Receives the current pixel clock.
    *	\return	HRESULT     0 on success, error code otherwise.
    *	\see    GetPixelClockRange, SetPixelClock
    */
    STDMETHOD(GetPixelClock)(long *plClock) = 0;

    /*!
	* \brief	Returns the min, max and default value for the pixelclock.
     *	\param 	plMin       Receives the minimum possible pixel clock.
     *	\param 	plMax       Receives the maximum possible pixel clock.
	*\param	plDefault   	Receives the default pixel clock value.
	*\return	HRESULT     0 on success, error code otherwise.
	*\see    	GetPixelClock, SetPixelClock
     */
    STDMETHOD(GetPixelClockRange)(long *plMin, long *plMax, long *plDefault) = 0;

    /*!
	* \brief	Sets the Pixelclock (in MHz) for the connected device.
     *	\param 	lClock      The pixel clock in MHz to set.
     *	\return	HRESULT     0 on success, CO_E_NOT_SUPPORTED if not supported, error code otherwise.
     *	\see    GetPixelClock, GetPixelClockRange
     */
    STDMETHOD(SetPixelClock)(long lClock) = 0;

    /*!
	* \brief	Queries which color mode to use when RGB8 mediatype is selected.
     *	\param 	plMode      Receives the currently selected RGB8 colormode.
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see    SetRGB8ColorMode
     */
    STDMETHOD(GetRGB8ColorMode)(long *plMode) = 0;

    /*!
	* \brief	Determines which color mode to use when RGB8 mediatype is selected.
	*
     *      Specifies whether Y8 or raw bayer pattern is used with RGB8 mode
     *		possible values are 11 for raw bayer pattern (on bayer color
     *      cameras), or 6 for monochrome images.
     *	\param 	lMode       Specifies the color mode used for RGB8 mode.
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see    GetRGB8ColorMode
     */
    STDMETHOD(SetRGB8ColorMode)(long lMode) = 0;

	/*! 
	* \brief	Queries the current possible min, max and interval for exposure time.
	*
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

    /*!
	* \brief	Queries the current exposure time
     *	\param 	plExp       Receives the current exposure time in us.
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see    GetExposureRange, SetExposureTime
     */
    STDMETHOD(GetExposureTime)(long *plExp ) = 0;

    /*!
	* \brief	Sets the exposure time of the camera.
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
  char          SerNo[12];   
  char          ID[20];       
  char          Version[10];  
  char          Date[12];    
  unsigned char Select;      
  unsigned char Type;         
  char          Reserved[8]; 
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
  char          nColorMode;  
  DWORD         nMaxWidth;   
  DWORD         nMaxHeight;   
  BOOL          bMasterGain;     
  BOOL          bRGain;         
  BOOL          bGGain;         
  BOOL          bBGain;          
  BOOL          bGlobShutter;  
  char			Reserved[16];		
} SENSORINFO, *PSENSORINFO;

#   ifdef _SENSORSCALERINFO
#       undef _SENSORSCALERINFO
#   endif
#   ifdef SENSORSCALERINFO
#       undef SENSORSCALERINFO
#   endif
typedef struct _SENSORSCALERINFO
  {
      INT       nCurrMode;
      INT       nNumberOfSteps;
      double    dblFactorIncrement;
      double    dblMinFactor;
      double    dblMaxFactor;
      double    dblCurrFactor;
      INT       nSupportedModes;
      BYTE      bReserved[84];
  } SENSORSCALERINFO;

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
    /*!
	*   \brief 	Returns hardware gain factors in percent
    *	\param 	plRed       Receives the red gain factor
    *	\param 	plGreen     Receives the green gain factor
    *	\param 	plBlue      Receives the blue gain factor
    *	\return	HRESULT     0 on success, error code otherwise.
    */
    STDMETHOD(GetWhiteBalanceMultipliers)(long *plRed, long *plGreen, long *plBlue) = 0;

    /*!
	*   \brief 	Sets hardware gain factors
    *	\param 	lRed        red gain factor to set in percent (457 means a factor of 4.57)
    *	\param 	lGreen      green gain factor to set in percent
    *	\param 	lBlue       blue gain factor to set in percent
    *	\return	HRESULT     0 on success, error code otherwise.
    */
    STDMETHOD(SetWhiteBalanceMultipliers)(long lRed, long lGreen, long lBlue) = 0;

    /*!
	* \brief	OBSOLETE: Queries the number of connected camera devices.
	*/
	STDMETHOD(GetNumberOfCameras)(long *plNr) = 0;

    /*!
	* \brief 	Returns the device info for the connected camera as a pair of CAMERAINFO and SENSORINFO
    *	\param 	psInfo      Receives the SENSORINFO
    *	\param 	pcInfo      Receives the CAMERAINFO
    *	\return	HRESULT     0 on success, error code otherwise.
    */
	STDMETHOD(GetDeviceInfo)(SENSORINFO *psInfo, CAMERAINFO *pcInfo) = 0;

	/*!
	* \brief	Queries the Version of the installed uc480 Driver files
     *	\param 	pVersion    Receives the Version of connected cameras.
     *	\return	HRESULT     0 on success, error code otherwise.
     *
     *  \note   This is not the Version of the uc480 capture device filter but
     *          the Version of the underlying driver files.
	 */
	STDMETHOD(GetDLLVersion)(long *pVersion) = 0;

	/*!
	* \brief 	OBSOLETE: Returns a pair of lists, containing of CAMERAINFO and SENSORINFO structures, which holds
	*		information of the available cameras.
	*/ 
	STDMETHOD(GetListOfCameras)(CAMERAINFO **cInfo, SENSORINFO **sInfo, long *lNr) = 0;

	/*!
	* \brief 	OBSOLETE: Tries to connect the filter to another camera.
	*/
	STDMETHOD(ConnectToCamera)(long lIndex) = 0;

	/*!
	* \brief	Activates or deactivates the hot pixel correction.
     *	\param 	lEnable     Set to 1 to activate or 0 to deactivate correction.
     *	\return	HRESULT     0 on success, error code otherwise.
	 *	\see    GetBadPixelCorrection
	 */
	STDMETHOD(SetBadPixelCorrection)(long lEnable) = 0;

	/*!	 
	* \brief	Queries the current state of the hot pixel correction unit.
     *	\param 	plEnable    Receives 1 if hot pixel correction is enabled.
     *	\return	HRESULT     0 on success, error code otherwise.
	 *	\see    SetBadPixelCorrection
	 */
	STDMETHOD(GetBadPixelCorrection)(long *plEnable) = 0;

	/*!	 
	* \brief	Configures the hot pixel correction of the sensor
     *	\param 	nMode          Selection Mode of Hotpixel
	 *          pParam	       Pointer to function parameter, depends
	 *					       on the selection of nMode
	 *          SizeOfParams   Size of function parameter memory in bytes
	 *
     *	\return	HRESULT     0 on success, error code otherwise.
	 */
	STDMETHOD(HotPixel)(UINT nMode, void *pParam, UINT SizeOfParam) = 0;

    /*!	 
	* \brief	Loads previous stored camera settings.
    *	\return	HRESULT     0 on success, error code otherwise.
    *	\see    SaveSettings
    */
    STDMETHOD(LoadSettings)(void) = 0;
    
    /*!	 
	* \brief	Stores the current set camera settings in the registry.
    *	\return	HRESULT     0 on success, error code otherwise.
    *	\see    LoadSettings
    *
    *   \note Data will be stored individual for each uc480 model
    */
	STDMETHOD(SaveSettings)(void) = 0;

	/*!
	* \brief	Resets the camera parameters to the driver defaults.
     *	\return	HRESULT     0 on success, error code otherwise.
     *	\see     LoadParameters, SaveParameters
     *
     *  \note   You may not be able to reset parameters while the filter is 
     *          connected or running.
	 */
    STDMETHOD(ResetDefaults)(void) = 0;
};
/*!
 * \}
 */	// end of group uc480Capture Interface


// {E179D0EE-E0BB-42d6-BAB9-FFDF2277E887}
DEFINE_GUID(IID_Iuc480CaptureEx, 
            0xe179d0ee, 0xe0bb, 0x42d6, 0xba, 0xb9, 0xff, 0xdf, 0x22, 0x77, 0xe8, 0x87);

interface Iuc480CaptureEx : public Iuc480Capture
{
 	
	/*!
	* \brief	Activates or deactivates the additional gain amplification
    *  \param  lGainBoost  Set to 1 to activate or 0 to deactivate extra amplification.
    *  \return HRESULT     0 on success, error code otherwise.
    *  \see    GetGainBoost
    */
    STDMETHOD(SetGainBoost)(long lGainBoost) = 0;

    /*!
	* \brief	Queries the current state of the extra amplification.
    *  \param  plGainBoost Receives 1 if extra amplification is enabled.
    *  \return HRESULT     0 on success, error code otherwise.
    *  \see    SetGainBoost
    */
    STDMETHOD(GetGainBoost)(long *plGainBoost) = 0;

    /*!
	* \brief	Activates or deactivates the hardware gamma.
    *  \param  lHWGamma    Set to 1 to activate or 0 to deactivate hw gamma.
    *  \return HRESULT     0 on success, error code otherwise.
    *  \see    GetHardwareGamma
    */
    STDMETHOD(SetHardwareGamma)(long lHWGamma) = 0;

    /*!
	* \brief	Queries the current state of hardware gamma.
    *  \param  plHWGamma    Receives 1 if hw gamma is enabled.
    *  \return HRESULT      0 on success, error code otherwise.
    *  \see    SetHardwareGamma
    */
    STDMETHOD(GetHardwareGamma)(long *plHWGamma) = 0;

	/*!	 
	* \brief	Load/save the parameters from/to a file or camera EEPRom userset.
     *	\param 	nMode          Selection Mode of ParameterSet
	 *          pParam	       Pointer to function parameter, depends
	 *					       on the selection of nMode
	 *          SizeOfParams   Size of function parameter memory in bytes
	 *
     *	\return	HRESULT     0 on success, error code otherwise.
	 */
	STDMETHOD(ParametersSet)(UINT nMode, void *pParam, UINT SizeOfParam) = 0;

    /*!
	* \brief	Load the parameters from a file or camera EEPRom userset.
    *	\param 	cszFileName Filename or EEPRom userset to load parameters from.
    *	\return	HRESULT     0 on success, error code otherwise.
    *	\see    SaveParameters, ResetDefaults
    *
    *  \note   You may not be able to load parameters while the filter is 
    *          connected or running.
    */
    STDMETHOD(LoadParameters)(const char* cszFileName) = 0;

    /*!
	* \brief	Stores the current parameters to file or camera EEPRom userset.
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
	/*!
	* \brief	Specifies the brightness reference value which should be achieved by auto gain and auto exposure.
     *  \param  lReference  The reference value the controller should reach.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoBrightnessReference, SetAutoBrightnessMaxExposure, SetAutoBrightnessMaxGain
     */
    STDMETHOD(SetAutoBrightnessReference)(long lReference) = 0;

    /*!
	* \brief 	Queries the actual set reference value for auto brightness control.
     *  \param  plReference Receives the current value for reference.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessReference
     */
    STDMETHOD(GetAutoBrightnessReference)(long* plReference) = 0;

    /*!
	* \brief 	Upper limit of the exposure time when used to control the image brightness automatically.  
     *  \param  lMaxExposure Maximum exposure time (in us Units) the controller is allowed to set.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see GetAutoBrightnessMaxExposure, SetAutoBrightnessMaxGain, SetAutoBrightnessReference
     */
    STDMETHOD(SetAutoBrightnessMaxExposure)(long lMaxExposure) = 0;

    /*!
	* \brief 	Queries the actual set upper limit of automatic controlled exposure time.
     *  \param  plMaxExposure Receives the currently allowed maximum exposure time (us Units)
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessMaxExposure
     */
    STDMETHOD(GetAutoBrightnessMaxExposure)(long* plMaxExposure) = 0;

    /*!
	* \brief 	Upper limit of gain when used to control the image brightness automatically.
     *  \param  lMaxGain    Maximum master gain value the controller is allowed to set.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessMaxExposure, GetAutoBrightnessMaxGain, SetAutoBrightnessReference
     */
    STDMETHOD(SetAutoBrightnessMaxGain)(long lMaxGain) = 0;

    /*!
	* \brief 	Queries the actual set upper limit of automatic controlled master gain amplifier. 
     *  \param  plMaxGain   Receives the currently allowed maximum gain value.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessMaxGain
     */
    STDMETHOD(GetAutoBrightnessMaxGain)(long* plMaxGain) = 0;

    /*!
	* \brief 	Controls the percentage of examined images for the automatic brightness control unit.
     *  \param  lSpeed      The desired speed in a range of 1%(slow) to 100%(fast).
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoBrightnessSpeed, SetAutoBrightnessReference
     */
    STDMETHOD(SetAutoBrightnessSpeed)(long lSpeed) = 0;

    /*!
	* \brief 	Queries the actual set rate at which image brightness is examined. 
     *  \param  plSpeed     Receives the currently set examination speed.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessSpeed
     */
    STDMETHOD(GetAutoBrightnessSpeed)(long* plSpeed) = 0;

    /*!
	* \brief 	Specifies the area of interest within the image in which the brightness should be examined.
     *  \param  lXPos       Left bound of the area of interest.
     *  \param  lYPos       Upper bound of the area of interest.
     *  \param  lWidth      Width of the area of interest.
     *  \param  lHeight     Height of the area of interest.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoBrightnessAOI
     */
    STDMETHOD(SetAutoBrightnessAOI)(long lXPos, long lYPos, long lWidth, long lHeight) = 0;

    /*!
	* \brief 	Queries the actual used area of interest in which the brightness is examined.
     *  \param  plXPos      Receives the left bound.
     *  \param  plYPos      Receives the upper bound.
     *  \param  plWidth     Receives the width.
     *  \param  plHeight    Receives the height.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoBrightnessAOI
     */
    STDMETHOD(GetAutoBrightnessAOI)(long* plXPos, long* plYPos, long* plWidth, long* plHeight) = 0;

    /*!
	* \brief 	Specifies relative offsets between the individual color channels when used by the automatic whitebalance control unit.
     *  \param  lRedOffset  Offset for the red gain channel relative to the green one.
     *  \param  lBlueOffset Offset for the blue gain channel relative to the green one.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoWBGainOffsets, SetAutoWBGainRange
     */
    STDMETHOD(SetAutoWBGainOffsets)(long lRedOffset, long lBlueOffset) = 0;

    /*!
	* \brief 	Queries the actual set color channel offsets for automatic whitebalance.
     *  \param  plRedOffset  Receives the red to green channel offset.
     *  \param  plBlueOffset  Receives the blue to green channel offset.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoWBGainOffsets
     */
    STDMETHOD(GetAutoWBGainOffsets)(long* plRedOffset, long* plBlueOffset) = 0;

    /*!
	* \brief 	Limits the range the automatic whitebalance controller unit is allowed to use when adjusting the RGB gain channels.
     *  \param  lMinRGBGain  Minimum allowed gain value.
     *  \param  lMaxRGBGain  Maximum allowed gain value.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoWBGainRange, SetAutoWBGainOffsets
     */
    STDMETHOD(SetAutoWBGainRange)(long lMinRGBGain, long lMaxRGBGain) = 0;

    /*!
	* \brief 	Queries the actual allowed gain range for the automatic whitebalance controller unit.
     *  \param  plMinRGBGain  Receives the currently allowed minimal gain value.
     *  \param  plMaxRGBGain  Receives the currently allowed maximal gain value.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoWBGainRange
     */
    STDMETHOD(GetAutoWBGainRange)(long* plMinRGBGain, long* plMaxRGBGain) = 0;

    /*!
	* \brief	Controls the percentage of examined images for the automatic whitebalance control unit.
     *  \param  lSpeed      The desired speed in a range of 1%(slow) to 100%(fast).
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoWBGainRange, SetAutoWBGainOffsets
     */
    STDMETHOD(SetAutoWBSpeed)(long lSpeed) = 0;

    /*!
	* \brief 	Queries the actual set rate at which the images whitebalance is examined. 
     *  \param  plSpeed     Receives the currently set examination speed.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    SetAutoWBSpeed
     */
    STDMETHOD(GetAutoWBSpeed)(long* plSpeed) = 0;

    /*!
	* \brief 	Specifies the area of interest within the image in which the whitebalance should be examined.
     *  \param  lXPos       Left bound of the area of interest.
     *  \param  lYPos       Upper bound of the area of interest.
     *  \param  lWidth      Width of the area of interest.
     *  \param  lHeight     Height of the area of interest.
     *  \return HRESULT     0 on success, error code otherwise.
     *  \see    GetAutoWBAOI
     */
    STDMETHOD(SetAutoWBAOI)(long lXPos, long lYPos, long lWidth, long lHeight) = 0;

    /*!
	* \brief 	Queries the actual used area of interest in which the whitebalance is examined.
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


// E122A994-FC4D-445b-B21C-308B674844E0
DEFINE_GUID(IID_Iuc480FaceDetection, 
            0xe122a994, 0xfc4d, 0x445b, 0xb2, 0x1c, 0x30, 0x8b, 0x67, 0x48, 0x44, 0xe0);

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
    INT nFacePosX;              /*!< \brief	Start X position.                                                           	*/
    INT nFacePosY;              /*!< \brief	Start Y position.                                                           	*/
    INT nFaceWidth;             /*!< \brief	Face width.                                                                 	*/
    INT nFaceHeight;            /*!< \brief	Face height.                                                                	*/
    INT nAngle;                 /*!< \brief	Face Angle (0...360° clockwise, 0° at twelve o'clock position. -1: undefined )  */
    UINT nPosture;              /*!< \brief	Face posture.                                                               	*/
    UC480TIME TimestampSystem;  /*!< \brief	System time stamp (device query time) .                                     	*/
    UINT64 nReserved;           /*!< \brief	Reserved for future use.                                                    	*/
    UINT nReserved2[4];         /*!< \brief	Reserved for future use.                                                    	*/
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
    UINT nSizeOfListEntry;      /*!< \brief	Size of one list entry in byte(in).  	*/
    UINT nNumDetectedFaces;     /*!< \brief	Number of detected faces(out).       	*/
    UINT nNumListElements;      /*!< \brief	Number of list elements(in).         	*/ 
    UINT nReserved[4];          /*!< \brief	reserved for future use(out).       	*/ 
    FDT_INFO_EL FaceEntry[1];   /*!< \brief	First face entry.                       */
} FDT_INFO_LIST;
#endif  /* DS_EXPORT */

// ============================================================================
/*! \defgroup Iuc480FaceDetection uc480 Face Detection Interface
*  Proprietary interface for uc480 face detection control exposed by the capture
*  filter. Allows a DirectShow based program to control and query the face detection
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================
interface Iuc480FaceDetection : public IUnknown
{
    /*!
     * \brief Query for support of the face detection feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of the face detection feature.
     * \param pbEnabled output location for result.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_IsEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of the face detection feature.
     * \param bEnable   new 'enabled' status.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_Enable)(bool bEnable) = 0;

    /*!
     * \brief Query the current 'suspended' status of the face detection feature.
     * \param pbSuspended   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_IsSuspended)(bool* pbSuspended) = 0;

    /*!
     * \brief Set the 'suspended' status of the face detection feature.
     * \param bSuspend  new 'suspended' status.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_Suspend)(bool bSuspend) = 0;

    /*!
     * \brief Query the current enabled status of the face detection's 'search angle' subfeature.
     * \param pbEnabled output location for result.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_IsSearchAngleEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of the face detection's 'search angle' feature.
	* \param	bEnable  	new 'search angle enable' status.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_EnableSearchAngle)(bool bEnable) = 0;

    /*!
     * \brief Query the current search angle.
     * \param pulAngle  output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetSearchAngle)(long* pulAngle) = 0;

    /*!
     * \brief Set the new search angle.
     * \param ulAngle   output location for result.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_SetSearchAngle)(long ulAngle) = 0;

    /*!
     * \brief Query the last determined face list.
     * \param pFaceList output location for result: preallocated object of type \see FDT_INFO_LIST.
     * \param ulSize    size of pFaceList in bytes.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetFaceList)(void* pFaceList, unsigned long ulSize) = 0;

    /*!
     * \brief Query the last determined number of faces.
     * \param pulNumFaces   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetNumFaces)(unsigned long* pulNumFaces) = 0;

    /*!
     * \brief Query the maximum number of faces that the feature can detect in an image.
     * \param pulMaxNumFaces    output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetMaxNumFaces)(unsigned long* pulMaxNumFaces) = 0;

    /*!
     * \brief Query the current maximum number of overlay drawings that the feature will show in an image.
     * \param pulMaxNumOvl  output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetMaxNumOvl)(unsigned long* pulMaxNumOvl) = 0;

    /*!
     * \brief Set the new maximum number of overlay drawings that the feature will show in an image.
     * \param ulMaxNumOvl   the new setting.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_SetMaxNumOvl)(unsigned long ulMaxNumOvl) = 0;

    /*!
     * \brief Query the current linewidth for the overlay drawings.
     * \param pulLineWidthOvl   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_GetLineWidthOvl)(unsigned long* pulLineWidthOvl) = 0;

    /*!
     * \brief Set the new linewidth for the overlay drawings.
     * \param ulLineWidthOvl    the new setting.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(FDT_SetLineWidthOvl)(unsigned long ulLineWidthOvl) = 0;

    /*!
     * \brief Query the resolution.
     * \param pulHorzRes    output location for result horizontal resolution.
     * \param pulVertRes    output location for result vertical resolution.
	* \return	HRESULT 	S_OK on success, error code otherwise.
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
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(ImgStab_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of the image stabilization feature.
     * \param pbEnabled output location for result.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(ImgStab_IsEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of the image stabilization feature.
     * \param bEnable   new 'enabled' status.
	* \return	HRESULT 	S_OK on success, error code otherwise.
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

// ============================================================================
/*! \defgroup Iuc480SensorAWB uc480 Sensor Auto White Balance Interface
*  Proprietary interface for uc480 auto white balance feature given by sensor exposed by 
*  the capture filter. Allows a DirectShow based program to control and query the sensor's
*  auto white  balance feature related parameters that are not accessible via direct 
*  show functions.
* \{
*/
// ============================================================================

// {E737FA4C-2160-45a5-95D3-CE6B069D9AB3}
DEFINE_GUID(IID_Iuc480SensorAWB, 
            0xe737fa4c, 0x2160, 0x45a5, 0x95, 0xd3, 0xce, 0x6b, 0x6, 0x9d, 0x9a, 0xb3);

interface Iuc480SensorAWB : public IUnknown
{
    /*!
     * \brief Query for support of sensor's awb feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(SensorAWB_IsSupported)(bool* pbSupported) = 0;

    /*!
	* \brief 	Query the current 'enabled' status of sensor's awb feature.
     * \param pbEnabled output location for result.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(SensorAWB_IsEnabled)(bool* pbEnabled) = 0;

    /*!
	* \brief 	Set the 'enabled' status of sensor's awb feature.
     * \param bEnable   new 'enabled' status.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(SensorAWB_Enable)(bool bEnable) = 0;

    /*!
	* \brief 	Query the current mode of sensor's awb feature.
     * \param pulMode   current mode.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(SensorAWB_GetMode)(unsigned long* pulMode) = 0;

    /*!
	* \brief 	Set the mode of sensor's awb feature.
     * \param ulMode    mode to set.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(SensorAWB_SetMode)(unsigned long ulMode) = 0;

    /*!
	* \brief 	Query the supported modes of sensor's awb feature.
     * \param pulModes  bitmask containing supported modes.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(SensorAWB_GetSupportedModes)(unsigned long* pulModes) = 0;
};
/*!
 * \}
 */	// end of group Iuc480SensorAWB

// ============================================================================
/*! \defgroup Iuc480AutoContrast uc480 Auto Contrast Correction Interface
*  Proprietary interface for uc480 auto contrast correction feature exposed by 
*  the capture filter. Allows a DirectShow based program to control and query the sensor's
*  auto contrast feature related parameters that are not accessible via direct 
*  show functions.
*  \note auto contrast correction can not be used if auto backlight compensation is
*  enabled and vice versa.
* \{
*/
// ============================================================================

// {CC2FCD9E-478A-42d9-9832-A3CC29D05098}
DEFINE_GUID(IID_Iuc480AutoContrast, 
            0xcc2fcd9e, 0x478a, 0x42d9, 0x98, 0x32, 0xa3, 0xcc, 0x29, 0xd0, 0x50, 0x98);

interface Iuc480AutoContrast : public IUnknown
{
    /*!
     * \brief Query for support of auto contrast correction feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoContrast_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of auto contrast correction feature.
     * \param pbEnabled output location for result.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoContrast_IsEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Query the current value of auto contrast correction feature.
     * \param pdblCorrValue   current correction value.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoContrast_GetValue)(double* pdblCorrValue) = 0;

    /*!
     * \brief Set the value of auto contrast correction feature.
     * \param dblCorrValue   value to set.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoContrast_SetValue)(double dblCorrValue) = 0;

    /*!
     * \brief Query the default value of auto contrast correction feature.
     * \param pdblCorrValue   default value.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoContrast_GetDefaultValue)(double* pdblCorrValue) = 0;

    /*!
     * \brief Query the range of auto contrast correction feature.
     * \param pdblMin   minimum value.
     * \param pdblMax   maximum value.
     * \param pdblInc   step width.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoContrast_GetRange)(double* pdblMin, double* pdblMax, double* pdblInc) = 0;
};
/*!
 * \}
 */	// end of group Iuc480AutoContrast

// ============================================================================
/*! \defgroup Iuc480AutoBacklight uc480 Auto Backlight Compensation Interface
*  Proprietary interface for uc480 auto backlight compensation feature exposed by 
*  the capture filter. Allows a DirectShow based program to control and query the sensor's
*  auto backlight compensation feature related parameters that are not accessible via direct 
*  show functions.
*  \note auto backlight compensation can not be used if auto contrast correction is
*  enabled and vice versa.
* \{
*/
// ============================================================================

// {A7CBC666-1A97-4af9-9652-4E34835F77CD}
DEFINE_GUID(IID_Iuc480AutoBacklight, 
            0xa7cbc666, 0x1a97, 0x4af9, 0x96, 0x52, 0x4e, 0x34, 0x83, 0x5f, 0x77, 0xcd);

interface Iuc480AutoBacklight : public IUnknown
{
    /*!
     * \brief Query for support of sensor's auto backlight compensation feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoBacklight_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of sensor's auto backlight compensation feature.
     * \param pbEnabled output location for result.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoBacklight_IsEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of sensor's auto backlight compensation feature.
     * \param bEnable   new 'enabled' status.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoBacklight_Enable)(bool bEnable) = 0;

    /*!
     * \brief Query the current mode of sensor's auto backlight compensation feature.
     * \param pulMode   current mode.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoBacklight_GetMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Set the mode of sensor's auto backlight compensation feature.
     * \param ulMode    mode to set.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoBacklight_SetMode)(unsigned long ulMode) = 0;

    /*!
     * \brief Query the default mode of sensor's auto backlight compensation feature.
     * \param pulMode   default mode.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoBacklight_GetDefaultMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Query the supported modes of sensor's auto backlight compensation feature.
     * \param pulModes  bitmask containing supported modes.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoBacklight_GetSupportedModes)(unsigned long* pulModes) = 0;
};
/*!
 * \}
 */	// end of group Iuc480AutoBacklight

// ============================================================================
/*! \defgroup Iuc480AntiFlicker uc480 Anti Flicker Interface
*  Proprietary interface for uc480 anti flicker feature exposed by the capture filter. 
*  Allows a DirectShow based program to control and query the sensor's anti flicker
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================


// {6B2A1AB6-E324-4d86-9637-2E783F50497A}
DEFINE_GUID(IID_Iuc480AntiFlicker, 
            0x6b2a1ab6, 0xe324, 0x4d86, 0x96, 0x37, 0x2e, 0x78, 0x3f, 0x50, 0x49, 0x7a);

interface Iuc480AntiFlicker : public IUnknown
{
    /*!
     * \brief Query for support of sensor's anti flicker feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(AntiFlicker_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current mode of sensor's anti flicker feature.
     * \param pulMode   current mode.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AntiFlicker_GetMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Set the mode of sensor's anti flicker feature.
     * \param ulMode    mode to set.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AntiFlicker_SetMode)(unsigned long ulMode) = 0;

    /*!
     * \brief Query the default mode of sensor's anti flicker feature.
     * \param pulMode   default mode.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AntiFlicker_GetDefaultMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Query the supported modes of sensor's anti flicker feature.
     * \param pulModes  bitmask containing supported modes.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(AntiFlicker_GetSupportedModes)(unsigned long* pulModes) = 0;
};
/*!
 * \}
 */	// end of group Iuc480AntiFlicker

// ============================================================================
/*! \defgroup Iuc480ScenePreset uc480 Scene Preset Interface
*  Proprietary interface for uc480 scene preset feature exposed by the capture filter. 
*  Allows a DirectShow based program to control and query the sensor's scene preset
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

// {E83A0636-194B-4ad8-BBD2-CD91AE35F136}
DEFINE_GUID(IID_Iuc480ScenePreset, 
            0xe83a0636, 0x194b, 0x4ad8, 0xbb, 0xd2, 0xcd, 0x91, 0xae, 0x35, 0xf1, 0x36);


interface Iuc480ScenePreset : public IUnknown
{
    /*!
     * \brief Query for support of sensor's scene preset feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(ScenePreset_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current mode of sensor's scene preset feature.
     * \param pulMode   current mode.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(ScenePreset_GetMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Set the mode of sensor's scene preset feature.
     * \param ulMode    mode to set.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(ScenePreset_SetMode)(unsigned long ulMode) = 0;

    /*!
     * \brief Query the default mode of sensor's scene preset feature.
     * \param pulMode   default mode.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(ScenePreset_GetDefaultMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Query the supported modes of sensor's scene preset feature.
     * \param pulModes  bitmask containing supported modes.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(ScenePreset_GetSupportedModes)(unsigned long* pulModes) = 0;
};
/*!
 * \}
 */	// end of group Iuc480ScenePreset

// ============================================================================
/*! \defgroup Iuc480DigitalZoom uc480 Digital Zoom Interface
*  Proprietary interface for uc480 digital zoom feature exposed by the capture filter. 
*  Allows a DirectShow based program to control and query the digital zoom
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

// {25F131C3-8F93-484f-8B4A-474313EEDDF5}
DEFINE_GUID(IID_Iuc480DigitalZoom, 
            0x25f131c3, 0x8f93, 0x484f, 0x8b, 0x4a, 0x47, 0x43, 0x13, 0xee, 0xdd, 0xf5);


interface Iuc480DigitalZoom : public IUnknown
{
    /*!
     * \brief Query for support of digital zoom feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(DigitalZoom_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the number of supported zoom factors used by digital zoom feature.
     * \param pulNumFactors number of supported factors
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(DigitalZoom_GetNumZoomFactors)(unsigned long* pulNumFactors) = 0;

    /*!
     * \brief Query the supported zoom factors of digital zoom feature.
     * \param pZFList   output location for result: preallocated object of type double.
     * \param ulSize    size of pZFList in bytes.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(DigitalZoom_GetZoomFactors)(void* pZFList, unsigned long ulSize) = 0;

    /*!
     * \brief Query the current zoom factor of digital zoom feature.
     * \param pdblZoomFactor current zoom factor.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(DigitalZoom_GetZoomFactor)(double* pdblZoomFactor) = 0;

    /*!
     * \brief Set the zoom factor of digital zoom feature.
     * \param dblZoomFactor zoom factor to set.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(DigitalZoom_SetZoomFactor)(double dblZoomFactor) = 0;

};
/*!
 * \}
 */	// end of group Iuc480DigitalZoom

// ============================================================================
/*! \defgroup Iuc480Saturation uc480 Saturation Interface
*  Proprietary interface for uc480 saturation feature exposed by the capture filter. 
*  Allows a DirectShow based program to control and query the sensor's saturation
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

// {EC410EDE-15BC-47b1-9BF7-6CB00F58FF5F}
DEFINE_GUID(IID_Iuc480Saturation, 
            0xec410ede, 0x15bc, 0x47b1, 0x9b, 0xf7, 0x6c, 0xb0, 0xf, 0x58, 0xff, 0x5f);


interface Iuc480Saturation : public IUnknown
{
    /*!
     * \brief Query for support of sensor's saturation feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(Saturation_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current saturation value of sensor's saturation feature.
     * \param plValue   current value.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(Saturation_GetValue)(long* plValue) = 0;

    /*!
     * \brief Set the saturation value of sensor's saturation feature.
     * \param lValue    mode to set.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(Saturation_SetValue)(long lValue) = 0;

    /*!
     * \brief Query the default value of sensor's saturation feature.
     * \param plDefValue default value.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(Saturation_GetDefaultValue)(long* plDefValue) = 0;

    /*!
     * \brief Query the range of sensor's saturation feature.
     * \param plMin     minimum value.
     * \param plMax     maximum value.
     * \param plInc     step width.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(Saturation_GetRange)(long* plMin, long* plMax, long* plInc) = 0;
};
/*!
 * \}
 */	// end of group Iuc480Saturation

// ============================================================================
/*! \defgroup Iuc480Sharpness uc480 Sharpness Interface
*  Proprietary interface for uc480 sharpness feature exposed by the capture filter. 
*  Allows a DirectShow based program to control and query the sensor's sharpness
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

// {1A30F620-78E0-4061-A730-C7B91848C7D0}
DEFINE_GUID(IID_Iuc480Sharpness, 
            0x1a30f620, 0x78e0, 0x4061, 0xa7, 0x30, 0xc7, 0xb9, 0x18, 0x48, 0xc7, 0xd0);

interface Iuc480Sharpness : public IUnknown
{
    /*!
     * \brief Query for support of sensor's sharpness feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(Sharpness_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current sharpness value of sensor's sharpness feature.
     * \param plValue   current value.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Sharpness_GetValue)(long* plValue) = 0;

    /*!
     * \brief Set the sharpness value of sensor's sharpness feature.
     * \param lValue    mode to set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Sharpness_SetValue)(long lValue) = 0;

    /*!
     * \brief Query the default value of sensor's sharpness feature.
     * \param plDefValue default value.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(Sharpness_GetDefaultValue)(long* plDefValue) = 0;

    /*!
     * \brief Query the range of sensor's sharpness feature.
     * \param plMin     minimum value.
     * \param plMax     maximum value.
     * \param plInc     step width.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Sharpness_GetRange)(long* plMin, long* plMax, long* plInc) = 0;
};
/*!
 * \}
 */	// end of group Iuc480Sharpness

// ============================================================================
/*! \defgroup Iuc480ColorTemperature uc480 Color Temperature Interface
*  Proprietary interface for uc480 color temperature feature exposed by the capture filter. 
*  Allows a DirectShow based program to control and query thecolor temperatures
*  feature related parameters that are not accessible via direct show functions.
*  \note changing color temperature values is only possible if rgb model is selected.
* \{
*/
// ============================================================================

// {3311AD49-0D54-4016-8FAA-B26CA351311B}
DEFINE_GUID(IID_Iuc480ColorTemperature, 
            0x3311ad49, 0xd54, 0x4016, 0x8f, 0xaa, 0xb2, 0x6c, 0xa3, 0x51, 0x31, 0x1b);

interface Iuc480ColorTemperature : public IUnknown
{
    /*!
     * \brief Query for support of rgb model feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(RGBModel_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current mode of rgb model feature.
     * \param pulMode   current mode.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(RGBModel_GetMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Set the mode of rgb model feature.
     * \param ulMode    mode to set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(RGBModel_SetMode)(unsigned long ulMode) = 0;

    /*!
     * \brief Query the default mode of rgb model feature.
     * \param pulMode   default mode.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(RGBModel_GetDefaultMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Query the supported modes of rgb model feature.
     * \param pulModes  bitmask containing supported modes.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(RGBModel_GetSupportedModes)(unsigned long* pulModes) = 0;

    /*!
     * \brief Query for support of color temperature feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(ColorTemperature_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current color temperature value of color temperature feature.
     * \param pulValue  current value.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(ColorTemperature_GetValue)(unsigned long* pulValue) = 0;

    /*!
     * \brief Set the color temperature value of color temperature feature.
	* \param	ulValue	mode to set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(ColorTemperature_SetValue)(unsigned long ulValue) = 0;

    /*!
     * \brief Query the default value of color temperature feature.
     * \param pulDefValue default value.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(ColorTemperature_GetDefaultValue)(unsigned long* pulDefValue) = 0;

    /*!
     * \brief Query the range of color temperature feature.
     * \param pulMin    minimum value.
     * \param pulMax    maximum value.
     * \param pulInc    step width.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(ColorTemperature_GetRange)(unsigned long* pulMin, unsigned long* pulMax, unsigned long* pulInc) = 0;
};
/*!
 * \}
 */	// end of group Iuc480ColorTemperature

// ============================================================================
/*! \defgroup Iuc480TriggerDebounce uc480 Trigger Debouncing Interface
*  Proprietary interface for uc480 trigger debounce feature exposed by the capture filter. 
*  Allows a DirectShow based program to control and query the trigger debounces
*  feature related parameters that are not accessible via direct show functions.
*  \note changing trigger debounce values is only possible if any trigger mode is 
*  activated.
*  \note using driver version 3.70 this feature is only supported on GigE cameras.
* \{
*/
// ============================================================================

// {49422CBA-CBD1-48a1-9810-DA3FDDC1FBEA}
DEFINE_GUID(IID_Iuc480TriggerDebounce, 
            0x49422cba, 0xcbd1, 0x48a1, 0x98, 0x10, 0xda, 0x3f, 0xdd, 0xc1, 0xfb, 0xea);

interface Iuc480TriggerDebounce : public IUnknown
{
    /*!
     * \brief Query for support of trigger debounce feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(TriggerDebounce_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current mode of trigger debounce feature.
     * \param pulMode   current mode.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(TriggerDebounce_GetMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Set the mode of trigger debounce feature.
     * \param ulMode    mode to set.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(TriggerDebounce_SetMode)(unsigned long ulMode) = 0;

    /*!
     * \brief Query the default mode of trigger debounce feature.
     * \param pulMode   default mode.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(TriggerDebounce_GetDefaultMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Query the supported modes of trigger debounce feature.
     * \param pulModes  bitmask containing supported modes.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(TriggerDebounce_GetSupportedModes)(unsigned long* pulModes) = 0;

    /*!
     * \brief Query the current delay value of trigger debounce feature.
     * \param pulValue  current value.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(TriggerDebounce_GetDelay)(unsigned long* pulValue) = 0;

    /*!
     * \brief Set the delay value of trigger debounce feature.
	* \param	ulValue	delay to set.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(TriggerDebounce_SetDelay)(unsigned long ulValue) = 0;

    /*!
     * \brief Query the default delay value of trigger debounce feature.
     * \param pulDefValue default value.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(TriggerDebounce_GetDefaultDelay)(unsigned long* pulDefValue) = 0;

    /*!
     * \brief Query the range of trigger debounce feature's delay value.
     * \param pulMin    minimum value.
     * \param pulMax    maximum value.
     * \param pulInc    step width.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(TriggerDebounce_GetDelayRange)(unsigned long* pulMin, unsigned long* pulMax, unsigned long* pulInc) = 0;
};
/*!
 * \}
 */	// end of group Iuc480TriggerDebounce

// ============================================================================
/*! \defgroup Iuc480Trigger uc480 Trigger Interface
*  Proprietary interface for additional uc480 trigger features exposed by the capture filter. 
*  Allows a DirectShow based program to control and query the additional trigger 
*  feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

// {00012E12-4696-4eb7-8CB4-DA7E0B782519}
DEFINE_GUID(IID_Iuc480Trigger, 
0x12e12, 0x4696, 0x4eb7, 0x8c, 0xb4, 0xda, 0x7e, 0xb, 0x78, 0x25, 0x19);

interface Iuc480Trigger : public IUnknown
{
    /*!
     * \brief Query if falling edge trigger mode is supported
     * \param pbSupported  output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(Trigger_IsFallingEdgeSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query if rising edge trigger mode is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Trigger_IsRisingEdgeSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query if software trigger mode is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Trigger_IsSoftwareTriggerSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Set trigger mode to be used
     * \param nMode  trigger mode
     * \return HRESULT: S_OK on success, error code otherwise.
     * \note    only the trigger mode to be used will be set. To
     *          activate triggering use SetMode().
     */
    STDMETHOD(Trigger_SetTriggerMode)(long nMode) = 0;

    /*!
     * \brief Get the current trigger mode
     * \param pnMode  output location for result.
     * \return HRESULT: S_OK on success, error code otherwise.
     */
    STDMETHOD(Trigger_GetTriggerMode)(long* pnMode) = 0;
};
/*!
 * \}
 */	// end of group Iuc480Trigger

// ============================================================================
/*! \defgroup Iuc480Photometry uc480 Photometry Interface
*  Proprietary interface for uc480 sensor's auto shutter and auto gain feature exposed 
*  by the capture filter. 
*  Allows a DirectShow based program to control and query the sensor's auto shutter
*  and auto gain feature related parameters that are not accessible via direct show functions.
*  \note on uc480 XS cameras modes for auto shutter and auto gain have to be set to the same value.
* \{
*/
// ============================================================================

// {5B200824-C3AD-4bcf-B6D7-F4991C7B5BF4}
DEFINE_GUID(IID_Iuc480Photometry, 
            0x5b200824, 0xc3ad, 0x4bcf, 0xb6, 0xd7, 0xf4, 0x99, 0x1c, 0x7b, 0x5b, 0xf4);

interface Iuc480Photometry : public IUnknown
{
    /*!
     * \brief Query for support of sensor's auto gain feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_IsAutoGainSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of sensor's auto gain feature.
     * \param pbEnabled output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_IsAutoGainEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of sensor's auto gain feature.
     * \param bEnable   new 'enabled' status.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_AutoGainEnable)(bool bEnable) = 0;

    /*!
     * \brief Query the current mode of sensor's auto gain feature.
     * \param pulMode   current mode.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_GetAutoGainMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Set the mode of sensor's auto gain feature.
     * \param ulMode    mode to set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_SetAutoGainMode)(unsigned long ulMode) = 0;

    /*!
     * \brief Query the default mode of sensor's auto gain feature.
     * \param pulMode   default mode.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_GetDefaultAutoGainMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Query the supported modes of sensor's auto gain feature.
     * \param pulModes  bitmask containing supported modes.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_GetSupportedAutoGainModes)(unsigned long* pulModes) = 0;

    /*!
     * \brief Query for support of sensor's auto shutter feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_IsAutoShutterSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of sensor's auto shutter feature.
     * \param pbEnabled output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_IsAutoShutterEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of sensor's auto shutter feature.
     * \param bEnable   new 'enabled' status.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_AutoShutterEnable)(bool bEnable) = 0;

    /*!
     * \brief Query the current mode of sensor's auto shutter feature.
     * \param pulMode   current mode.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_GetAutoShutterMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Set the mode of sensor's auto shutter feature.
     * \param ulMode    mode to set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_SetAutoShutterMode)(unsigned long ulMode) = 0;

    /*!
     * \brief Query the default mode of sensor's auto shutter feature.
     * \param pulMode   default mode.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_GetDefaultAutoShutterMode)(unsigned long* pulMode) = 0;

    /*!
     * \brief Query the supported modes of sensor's auto shutter feature.
     * \param pulModes  bitmask containing supported modes.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Photometry_GetSupportedAutoShutterModes)(unsigned long* pulModes) = 0;
};
/*!
 * \}
 */	// end of group Iuc480Photometry

// ============================================================================
/*! \defgroup Iuc480AutoFramerate uc480 Auto Framerate Interface
*  Proprietary interface for uc480 auto framerate feature exposed by the capture filter. 
*  Allows a DirectShow based program to control and query the sensor's or driver's
*  auto framerate feature related parameters that are not accessible via direct show functions.
*  \note you can use either sensor's or driver's auto framerate functionality, not both.
* \{
*/
// ============================================================================

// {92931A38-35C1-4923-97CC-0BCEE403EAFA}
DEFINE_GUID(IID_Iuc480AutoFramerate, 
            0x92931a38, 0x35c1, 0x4923, 0x97, 0xcc, 0xb, 0xce, 0xe4, 0x3, 0xea, 0xfa);

interface Iuc480AutoFramerate : public IUnknown
{
    /*!
     * \brief Query for support of sensor's auto framerate feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoFramerateSensor_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of sensor's auto framerate feature.
     * \param pbEnabled output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoFramerateSensor_IsEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of sensor's auto framerate feature.
     * \param bEnable   new 'enabled' status.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoFramerateSensor_Enable)(bool bEnable) = 0;
    /*!
     * \brief Query for support of driver's auto framerate feature.
     * \param pbSupported   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoFramerateDriver_IsSupported)(bool* pbSupported) = 0;

    /*!
     * \brief Query the current 'enabled' status of driver's auto framerate feature.
     * \param pbEnabled output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoFramerateDriver_IsEnabled)(bool* pbEnabled) = 0;

    /*!
     * \brief Set the 'enabled' status of driver's auto framerate feature.
     * \param bEnable   new 'enabled' status.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoFramerateDriver_Enable)(bool bEnable) = 0;
    /*!
     * \brief Get the actual framerate of the camera.
     * \param dblFramerate   current framerate
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(AutoFramerate_GetFramerate)(double* dblFramerate) = 0;
};
/*!
 * \}
 */	// end of group Iuc480AutoFramerate

// ============================================================================
/*! \defgroup Iuc480Flash uc480 Flash Interface
*  Proprietary interface for uc480 flash feature exposed by the capture filter. 
*  Allows a DirectShow based program to control and query the flash feature related
*  parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

// {051D5440-05F8-498f-BDAD-19E3ABB48ED9}
DEFINE_GUID(IID_Iuc480Flash, 0x51d5440, 0x5f8, 0x498f, 0xbd, 0xad, 0x19, 0xe3, 0xab, 0xb4, 0x8e, 0xd9);


interface Iuc480Flash : public IUnknown
{
    /*!
     * \brief Set the current flash strobe mode.
     * \param lMode     flash strobe mode to set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_SetStrobeMode)(long lMode) = 0;

    /*!
     * \brief Query the current flash strobe mode.
     * \param plMode    output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_GetStrobeMode)(long* plMode) = 0;

    /*!
     * \brief Set flash duration
	* \param	pulDuration	flash duration to set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_GetDuration)(unsigned long* pulDuration) = 0;

    /*!
     * \brief Query the range of flash duration
     * \param pulMin    minimum value
     * \param pulMax    maximum value
     * \param pulInc    step width
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_GetDurationRange)(unsigned long* pulMin, unsigned long* pulMax, unsigned long* pulInc ) = 0;

    /*!
     * \brief Set flash delay.
     * \param ulDelay   flash delay to be set.
	* \param	ulDuration	flash duration to be set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_SetDelayDuration)(unsigned long ulDelay, unsigned long ulDuration) = 0;

    /*!
     * \brief Query current flash delay.
     * \param pulDelay   output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_GetDelay)(unsigned long* pulDelay) = 0;

    /*!
     * \brief Query the range of flash delay
     * \param pulMin    minimum value
     * \param pulMax    maximum value
     * \param pulInc    step width
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_GetDelayRange)(unsigned long* pulMin, unsigned long* pulMax, unsigned long* pulInc ) = 0;

    /*!
     * \brief Query global exposure window to simulate global shutter
     * \param pulDelay      delay used for simulating global shutter
     * \param pulDuration   duration used for simulating global shutter
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_GetGlobalExposureWindow)(unsigned long* pulDelay, unsigned long* pulDuration ) = 0;

    /*!
     * \brief Query supported gpio ports available for flash output
     * \param pulPorts  bitmask containing supported gpio ports
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_GetSupportedGPIOPorts)(unsigned long* pulPorts ) = 0;

    /*!
     * \brief enable flash output on spcified gpio port
     * \param ulPort    port to be used for flash purposes
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Flash_EnableGPIOPort)(unsigned long ulPort ) = 0;
};

/*!
 * \}
 */	// end of group Iuc480Flash

// ============================================================================
/*! \defgroup Iuc480Resample uc480 Subsampling and Binning Interface
*  Proprietary interface for uc480 subsampling and binning features exposed by 
*  the capture filter. 
*  Allows a DirectShow based program to control and query the subsampling and
*  binning feature related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================

// {7C0098F5-20BE-47f7-83FF-E7CC12246547}
DEFINE_GUID(IID_Iuc480Resample, 0x7c0098f5, 0x20be, 0x47f7, 0x83, 0xff, 0xe7, 0xcc, 0x12, 0x24, 0x65, 0x47);
        

interface Iuc480Resample : public IUnknown
{
    /*!
     * \brief Set the current subsampling mode.
     * \param lMode     flash strobe mode to set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_SetMode)(long lMode) = 0;

    /*!
     * \brief Query the current subsampling mode.
     * \param plMode    output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_GetMode)(long* plMode) = 0;

    /*!
     * \brief Query current vertical resolution.
     * \param pulResolution   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_GetVerticalResolution)(unsigned long* pulResolution) = 0;

    /*!
     * \brief Query current horizontal resolution.
     * \param pulResolution   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_GetHorizontalResolution)(unsigned long* pulResolution) = 0;

    /*!
     * \brief Query if 2x vertical subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is2xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 2x horizontal subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is2xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 3x vertical subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is3xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 3x horizontal subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is3xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 4x vertical subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is4xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 4x horizontal subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is4xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 5x vertical subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is5xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 5x horizontal subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is5xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 6x vertical subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is6xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 6x horizontal subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is6xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 8x vertical subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is8xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 8x horizontal subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is8xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 16x vertical subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is16xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 16x horizontal subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_Is16xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if colorful subsampling is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Subsampling_IsColorSubsamplingSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Set the current binning mode.
     * \param lMode     binning mode to set.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_SetMode)(long lMode) = 0;

    /*!
     * \brief Query the current binning mode.
     * \param plMode    output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_GetMode)(long* plMode) = 0;

    /*!
     * \brief Query current vertical resolution.
     * \param pulResolution   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_GetVerticalResolution)(unsigned long* pulResolution) = 0;

    /*!
     * \brief Query current horizontal resolution.
     * \param pulResolution   output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_GetHorizontalResolution)(unsigned long* pulResolution) = 0;

    /*!
     * \brief Query current image width.
     * \param pnWidth   output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_GetImageWidth)(int* pnWidth) = 0;

    /*!
     * \brief Query current image height.
     * \param pnHeight   output location for result.
	* \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_GetImageHeight)(int* pnHeight) = 0;

    /*!
     * \brief Query if 2x vertical binning is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_Is2xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 2x horizontal binning is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_Is2xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 3x vertical binning is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_Is3xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 3x horizontal binning is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_Is3xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 4x vertical binning is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_Is4xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 4x horizontal binning is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_Is4xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 6x vertical binning is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_Is6xVertSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if 6x horizontal binning is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_Is6xHorSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if colorful binning is supported
     * \param pbSupported  output location for result.
	* \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Binning_IsColorBinningSupported)(bool* pbSupported ) = 0;
};
/*!
 * \}
 */	// end of group Iuc480Resample

// ============================================================================
/*! \defgroup Iuc480AOI uc480 Area of Interest Interface
*  Proprietary interface for controlling uc480 AOI features exposed by 
*  the capture filter. 
*  Allows a DirectShow based program to control and query the AOI feature 
*  related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================
// {8201BA4C-6E10-4258-9E4E-F8A70DFF4FAF}
DEFINE_GUID(IID_Iuc480AOI, 0x8201ba4c, 0x6e10, 0x4258, 0x9e, 0x4e, 0xf8, 0xa7, 0xd, 0xff, 0x4f, 0xaf);

interface Iuc480AOI : public IUnknown
{
    /*!
     * \brief   Query if setting an image aoi is supported
     * \param   pbSupported  output location for result.
	 * \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_IsImageAOISupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Get the current image area of interest in absolute uc480 coordinates.
     * \param   prcAOI   output location containing aoi information (left, top, right, bottom).
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetImageAOI)(RECT *prcAOI) = 0;

    /*!
     * \brief Set the current image area of interest in absolute uc480 coordinates.
     * \param   rcAOI   variable of type RECT containing the new AOI coordinates.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_SetImageAOI)(RECT rcAOI) = 0;

    /*!
     * \brief Get the current area of interest used by auto exposure feature (if not set the actual image AOI is used).
     * \param   prcAOI   output location containing aoi information (left, top, right, bottom).
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetAutoBrightnessAOI)(RECT *prcAOI) = 0;

    /*!
     * \brief Set the current area of interest used by auto exposure feature.
     * \param   rcAOI   variable of type RECT containing the new AOI coordinates.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_SetAutoBrightnessAOI)(RECT rcAOI) = 0;

    /*!
     * \brief Get the current area of interest used by auto white balance feature (if not set the actual image AOI is used).
     * \param   prcAOI   output location containing aoi information (left, top, right, bottom).
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetAutoWBAOI)(RECT *prcAOI) = 0;

    /*!
     * \brief Set the current area of interest used by auto white balance feature.
     * \param   rcAOI   variable of type RECT containing the new AOI coordinates.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_SetAutoWBAOI)(RECT rcAOI) = 0;

    /*!
     * \brief Get the increment to change horizontal position of the AOI.
     * \param   pnInc   variable containing the increment value.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetIncPosX)(INT* pnInc) = 0;

    /*!
     * \brief Get the increment to change vertical position of the AOI.
     * \param   pnInc   variable containing the increment value.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetIncPosY)(INT* pnInc) = 0;

    /*!
     * \brief Get the increment to change the width of the AOI.
     * \param   pnInc   variable containing the increment value.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetIncSizeX)(INT* pnInc) = 0;

    /*!
     * \brief Get the increment to change the height of the AOI.
     * \param   pnInc   variable containing the increment value.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetIncSizeY)(INT* pnInc) = 0;

    /*!
     * \brief Get the minimum and maximum value of the horizontal position of the AOI.
     * \param   pnMin   variable containing the smallest possible horizontal position.
     * \param   pnMax   variable containing the largest possible horizontal position.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetMinMaxPosX)(INT* pnMin, INT* pnMax) = 0;


    /*!
     * \brief Get the minimum and maximum value of the vertical position of the AOI.
     * \param   pnMin   variable containing the smallest possible horizontal position.
     * \param   pnMax   variable containing the largest possible horizontal position.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetMinMaxPosY)(INT* pnMin, INT* pnMax) = 0;

    /*!
     * \brief Get the minimum and maximum value of the width of the AOI.
     * \param   pnMin   variable containing the smallest possible width.
     * \param   pnMax   variable containing the largest possible width.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetMinMaxSizeX)(INT* pnMin, INT* pnMax) = 0;


    /*!
     * \brief Get the minimum and maximum value of the height of the AOI.
     * \param   pnMin   variable containing the smallest possible height.
     * \param   pnMax   variable containing the largest possible height.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(AOI_GetMinMaxSizeY)(INT* pnMin, INT* pnMax) = 0;

    /*!
     * \brief Generic access to the AOI interface. Use this command to access to more AOI functionality. (see uc480 SDK documentation)
     * \param   ulCommand   specifies which aoi command has to be used.
     * \param   pParam      void pointer containing function parameters.
     * \param   ulSize      size of pParam.
     * \return  HRESULT	S_OK on success, error code otherwise.
     * \note the generic access function is provided for future use.
     */
    STDMETHOD(AOI_Generic)(unsigned long ulCommand, void* pParam, unsigned long ulSize) = 0;
};
/*!
 * \}
 */	// end of group Iuc480AOI

// ============================================================================
/*! \defgroup Iuc480Gain uc480 Hardware Gain Interface
*  Proprietary interface for controlling uc480 hardware gain features exposed by 
*  the capture filter. 
*  Allows a DirectShow based program to control and query the hardware gain feature 
*  related parameters that are not accessible via direct show functions.
* \{
*/
// ============================================================================
// {D750DAE4-9C88-48cd-B966-79ACE3E5147C}
DEFINE_GUID(IID_Iuc480Gain, 0xd750dae4, 0x9c88, 0x48cd, 0xb9, 0x66, 0x79, 0xac, 0xe3, 0xe5, 0x14, 0x7c);

interface Iuc480Gain : public IUnknown
{
    /*!
     * \brief Query if master gain is supported
     * \param pbSupported  output location for result.
	 * \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_IsMasterSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief Query if rgb gains are supported
     * \param pbSupported  output location for result.
	 * \return	HRESULT		S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_IsRGBSupported)(bool* pbSupported ) = 0;

    /*!
     * \brief   Get the current hardware gain value.
     * \param   nWhich  specifies which gain to query (0 = master gain, 1 = red gain, 
     *                  2 = green gain, 3 = blue gain).
     * \param   pnValue output location containing gain value.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_GetHwGain)(INT nWhich, INT *pnValue) = 0;

    /*!
     * \brief   Set the specified hardware gain value.
     * \param   nWhich  specifies which gain to set (0 = master gain, 1 = red gain, 
     *                  2 = green gain, 3 = blue gain).
     * \param   nValue  gain value to set.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_SetHwGain)(INT nWhich, INT nValue) = 0;

    /*!
     * \brief   Get the hardware gain default values.
     * \param   pnMaster    output location containing the master gain default value.
     * \param   pnRed       output location containing the red gain default value (-1 if not available).
     * \param   pnGreen     output location containing the green gain default value (-1 if not available).
     * \param   pnBlue      output location containing the blue gain default value (-1 if not available).
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_GetHwGainDefaults)(INT *pnMaster, INT *pnRed, INT *pnGreen, INT *pnBlue) = 0;

    /*!
     * \brief   Get the hardware gain value range.
     * \param   nWhich  specifies which gain to query (0 = master gain, 1 = red gain, 
     *                  2 = green gain, 3 = blue gain).
     * \param   pnMin output location containing minimum gain value.
     * \param   pnMin output location containing maximum gain value.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_GetHwGainRange)(INT nWhich, INT *pnMin, INT *pnMax) = 0;

    /*!
     * \brief   Get the current gain factor value.
     * \param   nWhich      specifies which gain factor to query (0 = master gain, 1 = red gain, 
     *                      2 = green gain, 3 = blue gain).
     * \param   pnFactor    output location containg the queried gain factor value.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_GetHwGainFactor)(INT nWhich, INT* pnFactor) = 0;

    /*!
     * \brief   Set the specified hardware gain factor value.
     * \param   nWhich  specifies which gain to set (0 = master gain, 1 = red gain, 
     *                  2 = green gain, 3 = blue gain).
     * \param   nValue  gain factor value to set.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_SetHwGainFactor)(INT nWhich, INT nFactor) = 0;

    /*!
     * \brief   Get the hardware gain factor default values.
     * \param   pnMasterFactor    output location containing the master gain factor default value.
     * \param   pnRedFactor       output location containing the red gain factor default value (-1 if not available).
     * \param   pnGreenFactor     output location containing the green gain factor default value (-1 if not available).
     * \param   pnBlueFactor      output location containing the blue gain factor default value (-1 if not available).
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_GetHwGainFactorDefaults)(INT *pnMasterFactor, INT *pnRedFactor, INT *pnGreenFactor, INT *pnBlueFactor) = 0;

    /*!
     * \brief   Get the current gain factor value for the given gain index.
     * \param   nWhich      specifies which gain factor to query (0 = master gain, 1 = red gain, 
     *                      2 = green gain, 3 = blue gain).
     * \param   nGain       gain value to query factor for.
     * \param   pnFactor    output location containing the queried gain factor value.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_InquireHwGainFactor)(INT nWhich, INT nGain, INT* pnFactor) = 0;

    /*!
     * \brief   Get the gain factor value range for the given gain.
     * \param   nWhich      specifies which gain factor to query (0 = master gain, 1 = red gain, 
     *                      2 = green gain, 3 = blue gain).
     * \param   pnMin       output location containing the minimum gain factor value.
     * \param   pnMax       output location containing the maximum gain factor value.
	 * \return	HRESULT	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_GetHwGainFactorRange)(INT nWhich, INT* pnMin, INT* pnMax) = 0;

    /*!
     * \brief   Query for support of sensor's gain boost feature.
     * \param   pbSupported     output location for result.
	 * \return	HRESULT 		S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_IsGainBoostSupported)(bool* pbSupported) = 0;

    /*!
     * \brief   Query the current value of sensor's gain boost feature.
     * \param   plValue     current value.
	* \return	HRESULT     S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_GetGainBoostValue)(long* plValue) = 0;

    /*!
     * \brief   Set the saturation value of sensor's saturation feature.
     * \param   lValue      mode to set.
	* \return	HRESULT 	S_OK on success, error code otherwise.
     */
    STDMETHOD(Gain_SetGainBoostValue)(long lValue) = 0;

};
/*!
 * \}
 */	// end of group Iuc480Gain


// ============================================================================
/*! \defgroup Iuc480Scaler uc480 Scaler Interface
*
*
*
* \{
*/
// ============================================================================

// {720C5C49-5282-4b6e-9FED-98FE8A8A6063}
DEFINE_GUID(IID_Iuc480Scaler, 0x720c5c49, 0x5282, 0x4b6e, 0x9f, 0xed, 0x98, 0xfe, 0x8a, 0x8a, 0x60, 0x63);

interface Iuc480Scaler : public IUnknown
{
	/*!	 
	* \brief	Activated in some sensors the internal image scaling
     *	\param 	nMode		   Function mode
	 *          dblFactor	   scaling factor
	 *
     *	\return	HRESULT     0 on success, error code otherwise.
	 */
	STDMETHOD(SetSensorScaler)(UINT nMode, double dblFactor) = 0;

	/*!	 
	* \brief	Obtained from some sensors information about the internal image scaling
     *	\param 	pSensorScalerInfo		Pointer to a structure of type SENSORSCALERINFO, 
	 *									where should be written in the information
	 *          nSensorScalerInfoSize   Size of the structure of type SENSORSCALERINFO
	 *
     *	\return	HRESULT     0 on success, error code otherwise.
	 */
	STDMETHOD(GetSensorScalerInfo)(SENSORSCALERINFO *pSensorScalerInfo, INT nSensorScalerInfoSize) = 0;

	STDMETHOD(GetScalerImageWidth)		(int *pnWidth) = 0;
	STDMETHOD(GetScalerImageHeight)		(int *pnHeight) = 0;
	STDMETHOD(SetImageSize)				(int nWidth, int nHeight) = 0;
};
/*!
 * \}
 */	// end of group Iuc480Scaler


// ============================================================================
/*! \defgroup Iuc480Event uc480 Event Interface
*
*
*
* \{
*/
// ============================================================================

// {C539FCE7-05F1-4ddd-8FD4-FADDC199403C}
DEFINE_GUID(IID_Iuc480Event, 0xc539fce7, 0x5f1, 0x4ddd, 0x8f, 0xd4, 0xfa, 0xdd, 0xc1, 0x99, 0x40, 0x3c);

interface Iuc480Event : public IUnknown
{

	/*!	 
	 *  \brief	Initialising Event
	 *
     *	\return	HRESULT     0 on success, error code otherwise.
	 */
	STDMETHOD(InitEvent)(HANDLE hEv, INT nWhich) = 0;

	/*!	 
	 *  \brief	Enable Event
	 *
     *	\return	HRESULT     0 on success, error code otherwise.
	 */
	STDMETHOD(EnableEvent)(INT nWhich) = 0;

	/*!	 
	 *  \brief	Disable Event
	 *
     *	\return	HRESULT     0 on success, error code otherwise.
	 */
	STDMETHOD(DisableEvent)(INT nWhich) = 0;

    /*!	 
	 *  \brief	Exit Event
	 *
     *	\return	HRESULT     0 on success, error code otherwise.
	 */
	STDMETHOD(ExitEvent)(INT ExitEvent) = 0;
};
/*!
 * \}
 */	// end of group Iuc480Event



#endif  // #ifndef UC480_CAPTURE_INTERFACE
