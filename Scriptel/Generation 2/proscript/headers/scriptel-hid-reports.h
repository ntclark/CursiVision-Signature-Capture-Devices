/*******************************************************************************
 * scriptel-hid-reports.h - Copyright 2014 - Scriptel Corporation
 * ----
 * This header contains information about how to interact with ScripTouch
 * devices. Data structures and constants used by the devices are stored here.
 ******************************************************************************/

#ifndef _SCRIPTEL_HID_REPORTS_H_
#define _SCRIPTEL_HID_REPORTS_H_

/**
 * Defines the "base" region size, before factoring in caption length.
 */
#define SCRIPTEL_REGION_BASE_SIZE 28

/**
  * @file scriptel-hid-reports.h
  * This file contains the headers required for lower level communication
  * with ScripTouch devices.
  */

/**
 * This enumeration contains all of the valid reports identifiers for
 * reports that the ScripTouch firmware can respond to.
 */
typedef enum {
	/**
	 * The report identifier of the run mode feature report.
	 */
	SCRIPTEL_FEATURE_RUNMODE = 10,
	/**
	 * The report identifier of the capabilities feature report.
	 */
	SCRIPTEL_FEATURE_CAPABILITIES = 11,
	/**
	 * The report identifier of the device status and error clearer.
	 */
	SCRIPTEL_FEATURE_DEVICE_STATUS = 13,
	/**
	 * The report identifier of the device configuration feature report.
	 */
	SCRIPTEL_FEATURE_DEVICECONFIGURE = 15,
	/**
	 * The report identifier of the contact count maximum feature report.
	 */
	SCRIPTEL_FEATURE_CONTACTCOUNTMAXIMUM = 16,
	/**
	 * The report identifier of the frequency amplitude feature report.
	 */
	SCRIPTEL_FEATURE_FREQUENCYAMPLITUDE = 20,
	/**
	 * The report identifier of the power state feature report.
	 */
	SCRIPTEL_FEATURE_POWERSTATE = 21,
	/**
	 * The report identifier of the version information feature report.
	 */
	SCRIPTEL_FEATURE_VERSIONINFO = 22,
	/**
	 * The report identifier of the coordinate range feature report.
	 */
	SCRIPTEL_FEATURE_COORDINATERANGE = 23,
	/**
	 * The report identifier of the firmware download feature report.
	 */
	SCRIPTEL_FEATURE_FIRMWAREDOWNLOAD = 25,
	/**
	 * The report identifier of the error correction feature report.
	 */
	SCRIPTEL_FEATURE_ERRORCORRECTION = 26,
	/**
	 * The report identifier of the serial number feature report.
	 */
	SCRIPTEL_FEATURE_SERIALNUMBER = 27,
	/**
	 * The report identifier of the save configuration feature report.
	 */
	SCRIPTEL_FEATURE_SAVECONFIGURATION = 28,
	/**
	 * The report identifier of the download command status feature report.
	 */
	SCRIPTEL_FEATURE_DOWNLOADCOMMANDSTATUS = 29,
	/**
	 * The report identifier of the operating mode feature report.
	 */
	SCRIPTEL_FEATURE_OPERATINGMODE = 40,
	/**
	 * The report identifier of the general parameters feature report.
	 */
	SCRIPTEL_FEATURE_GENERALPARAMETERS = 41,
	/**
	 * The report identifier of the ASIC reset feature report.
	 */
	SCRIPTEL_FEATURE_ASICRESET = 42,
	/**
	 * The report identifier of the register get feature report.
	 */
	SCRIPTEL_FEATURE_REGISTERGET = 43,
	/**
	 * The report identifier of the register set feature report.
	 */
	SCRIPTEL_FEATURE_REGISTERSET = 44,
	/**
	 * The report identifier of the pen parameters feature report.
	 */
	SCRIPTEL_FEATURE_PENPARAMETERS = 50,
	/**
	 * The report identifier of the touch parameters feature report.
	 */
	SCRIPTEL_FEATURE_TOUCHPARAMETERS = 60,
	/**
     * The report identifier of the output configuration feature report.
     */
    SCRIPTEL_FEATURE_OUTPUTCONFIGURATION = 91,
	/**
	 * The report identifier of the peripheral configuration feature report.
	 */
	SCRIPTEL_FEATURE_PERIPHERAL_CONFIGURATION = 92,
	/**
	 * The report identifier of the calibration data report.
	 */
	SCRIPTEL_FEATURE_CALIBRATION_DATA = 131,
	/**
	 * The report identifier of the display information feature report.
	 */
	SCRIPTEL_FEATURE_DISPLAYINFO = 100,
	/**
	 * The report identifier of the display content feature report.
	 */
	SCRIPTEL_FEATURE_DISPLAYCONTENT = 101,
	/**
	 * The report identifier of the display settings feature report.
	 */
	SCRIPTEL_FEATURE_DISPLAYSETTINGS = 102,
	/**
	 * The report identifier of the update display chunk feature report.
	 */
	SCRIPTEL_FEATURE_UPDATEDISPLAYCHUNK = 103,
	/**
	 * The report identifier of the region get feature report.
	 */
	SCRIPTEL_FEATURE_REGIONGET = 110,
	/**
	 * The report identifier of the region default feature report.
	 */
	SCRIPTEL_FEATURE_REGIONGETDEFAULT = 111,
	/**
	 * The report identifier of the region set feature report.
	 */
	SCRIPTEL_FEATURE_REGIONSET = 112,
	/**
	 * The report identifier of the region update feature report.
	 */
	SCRIPTEL_FEATURE_REGIONUPDATE = 113,
	/**
	 * The report identifier of the region delete feature report.
	 */
	SCRIPTEL_FEATURE_REGIONDELETE = 114,
	/**
	 * The report identifier of the region edit complete feature report.
	 */
	SCRIPTEL_FEATURE_REGIONEDITCOMPLETE = 115,
	/**
	 * The report identifier of the calibrate feature report.
	 */
	SCRIPTEL_FEATURE_CALIBRATE = 130,
	/**
	 * The report identifier of the resource index feature report. 
	 */
    SCRIPTEL_FEATURE_RESOURCE_IDX = 140,
    /**
    * The report identifier of the resource info feature report.
    */
    SCRIPTEL_FEATURE_RESOURCE_INFO = 141,
    /**
    * The report identifier of the resource feature report.
    */
    SCRIPTEL_FEATURE_RESOURCE = 142,
    /**
    * The report identifier of the delete resource feature report.
    */
    SCRIPTEL_FEATURE_DELETE_RESOURCE = 143,
    /**
    * The report identifier of the resource availability feature report.
    */
    SCRIPTEL_FEATURE_RESOURCE_AVAILABILITY = 144,
    /**
    * The report identifier of the region index feature report.
    */
    SCRIPTEL_FEATURE_REGION_IDX = 150,
    /**
    * The report identifier of the region feature report.
    */
    SCRIPTEL_FEATURE_REGION = 152,
    /**
    * The report identifier of the delete region feature report.
    */
    SCRIPTEL_FEATURE_DELETE_REGION = 153,
    /**
    * The report identifier of the screen index feature report.
    */
    SCRIPTEL_FEATURE_SCREEN_IDX = 160,
    /**
    * The report identifier of the screen feature report.
    */
    SCRIPTEL_FEATURE_SCREEN = 162,
    /**
    * The report identifier of the delete screen feature report.
    */
    SCRIPTEL_FEATURE_DELETE_SCREEN = 163,
    /**
    * The report identifier of the modify screen feature report.
    */
    SCRIPTEL_FEATURE_MODIFY_SCREEN = 164,
    /**
    * The report identifier of the screen region info feature report.
    */
    SCRIPTEL_FEATURE_SCREEN_REGION_INFO = 170,
    /**
    * The report identifier of the active screen feature report.
    */
    SCRIPTEL_FEATURE_ACTIVE_SCREEN = 171,
    /**
    * The report identifier of the screen region save reload report.
    */
    SCRIPTEL_FEATURE_SCREEN_REGION_SAVE_RELOAD = 172,
    /**
     * The report identifier of the coordinate input report.
     */
	SCRIPTEL_INPUT_COORDINATE = 1,
	/**
	 * The report identifier of the extended coordinate input report.
	 */
	SCRIPTEL_INPUT_EXTENDEDCOORDINATE = 2,
	/**
	 * The report identifier of the adc values input report.
	 */
	SCRIPTEL_INPUT_ADCVALUES = 3,
	/**
	 * The report identifier of the mouse coordinate input report.
	 */
	SCRIPTEL_INPUT_MOUSECOORDINATE = 4,
	/**
	 * The report identifier of the digitizer pen coordinate input report.
	 */
	SCRIPTEL_INPUT_DIGITIZERPENCOORDINATE = 5,
	/**
	 * The report identifier of the digitizer touch coordinate input report.
	 */
	SCRIPTEL_INPUT_DIGITIZERTOUCHCOORDINATE = 6,
	/**
	 * The report identifier of the keyboard input report.
	 */
	SCRIPTEL_INPUT_KEYBOARD = 7,
	/**
	 * The report identifier of the multi-touch coordinate input report.
	 */
	SCRIPTEL_INPUT_MULTITOUCHCOORDINATE = 8,
	/**
	 * The report identifier of the magnetic card data input report.
	 */
	SCRIPTEL_INPUT_MAGCARDDATA = 12,
	/**
	 * The report identifier of the debug coordinate input report.
	 */
	SCRIPTEL_INPUT_DEBUGCOORDINATE = 254,
	/**
	 * The report identifier of Engineering Use report
	 */
	SCRIPTEL_FEATURE_ENGINEERING_TEST = 253,
	/**
	 * The report identifier of Get/Set Picel Retrieval Location
	 */
	SCRIPTEL_FEATURE_PIXEL_RETRIEVAL_LOCATION = 104,
	/**
	 * The report itdentifier of Get/SET Pixels
	 */
	SCRIPTEL_FEATURE_PIXELS = 105,

	/** The following reports are fake input reports generated in the library **/

	/**
	 * The report identifier of the button down input report.
	 */
	SCRIPTEL_INPUT_BUTTONDOWN = 250,
	/**
	 * The report identifier of the button move input report.
	 */
	SCRIPTEL_INPUT_BUTTONMOVE = 251,
	/**
	 * The report identifier of the button up input report.
	 */
	SCRIPTEL_INPUT_BUTTONUP = 252,
	/**
	 * The report identifier of the button press input report.
	 */
	SCRIPTEL_INPUT_BUTTONPRESS = 253	// note this is overloaded with a real feature report
} scriptel_hid_report_id;

/**
 * This enumeration contains the output report flags.
 */
 typedef enum {
	 /**
	  * Used by Scriptel, does not apply to signature pads
	  */
	 SCRIPTEL_OUTPUT_CONFIGURATION_OEM = 0x00000001,
	 /**
	  *	ProScript mode
	  */
	 SCRIPTEL_OUTPUT_CONFIGURATION_OEM_EXTENDED = 0x00000002,
	 /**
	  * USB device type, not used on signature pads.
	  */
	 SCRIPTEL_OUTPUT_CONFIGURATION_MOUSE = 0x00000100,
	 /**
	  * USB device type, not used on signature pads.
	  */
	 SCRIPTEL_OUTPUT_CONFIGURATION_PEN_DIGITIZER = 0x00000200,
	 /**
	 * USB device type, not used on signature pads.
	 */
	 SCRIPTEL_OUTPUT_CONFIGURATION_TOUCH = 0x00000400,
	 /**
	  * EasyScript mode
	  */
	 SCRIPTEL_OUTPUT_CONFIGURATION_KEYBOARD = 0x00010000,
	 /**
	  * Used for testing
	  */
	 SCRIPTEL_OUTPUT_CONFIGURATION_RAW_DATA = 0x40000000,
	 /**
	  * Used for testing
	  */
	 SCRIPTEL_OUTPUT_CONFIGURATION_DEBUG_DATA = 0x80000000
 } scriptel_ouput_configuration_flags;

/**
 * This enumeration contains the sizes in bytes of all of the static sized
 * reports. This size includes the report identifier.
 */
typedef enum {
	/**
	 * Size in bytes of the run mode feature report.
	 */
	SCRIPTEL_FEATURE_RUNMODE_SIZE = 2,
	/**
	 * Size in bytes of the capabilities feature report.
	 */
	SCRIPTEL_FEATURE_CAPABILITIES_SIZE = 65,
	/**
	 * Size in bytes of the device configure feature report.
	 */
	SCRIPTEL_FEATURE_DEVICECONFIGURE_SIZE = 3,
	/**
	 * Size in bytes of the contact count maximum feature report.
	 */
	SCRIPTEL_FEATURE_CONTACTCOUNTMAXIMUM_SIZE = 2,
	/**
	 * Size in bytes of the frequency amplitude feature report.
	 */
	SCRIPTEL_FEATURE_FREQUENCYAMPLITUDE_SIZE = 4,
	/**
	 * Size in bytes of the power state feature report.
	 */
	SCRIPTEL_FEATURE_POWERSTATE_SIZE = 2,
	/**
	 * Size in bytes of the version info feature report.
	 */
	SCRIPTEL_FEATURE_VERSIONINFO_SIZE = 9,
	/**
	 * Size in bytes of the coordinate range feature report.
	 */
	SCRIPTEL_FEATURE_COORDINATERANGE_SIZE = 9,
	/**
	 * Size in bytes of the firmware download feature report.
	 */
	SCRIPTEL_FEATURE_FIRMWAREDOWNLOAD_SIZE = 134,
	/**
	 * Size in bytes of the error correction feature report.
	 */
	SCRIPTEL_FEATURE_ERRORCORRECTION_SIZE = 2049,
	/**
	 * Size in bytes of the serial number feature report.
	 */
	SCRIPTEL_FEATURE_SERIALNUMBER_SIZE = 35,
	/**
	 * Size in bytes of the save configuration feature report.
	 */
	SCRIPTEL_FEATURE_SAVECONFIGURATION_SIZE = 2,
	/**
	 * Size in bytes of the download command status feature report.
	 */
	SCRIPTEL_FEATURE_DOWNLOADCOMMANDSTATUS_SIZE = 3,
	/**
	 * Size in bytes of the operating mode feature report.
	 */
	SCRIPTEL_FEATURE_OPERATINGMODE_SIZE = 5,
	/**
	 * Size in bytes of the general parameters feature report.
	 */
	SCRIPTEL_FEATURE_GENERALPARAMETERS_SIZE = 6,
	/**
	 * Size in bytes of the ASIC reset feature report.
	 */
	SCRIPTEL_FEATURE_ASICRESET_SIZE = 2,
	/**
	 * Size in bytes of the register get feature report.
	 */
	SCRIPTEL_FEATURE_REGISTERGET_SIZE = 9,
	/**
	 * Size in bytes of the register set feature report.
	 */
	SCRIPTEL_FEATURE_REGISTERSET_SIZE = 3,
	/**
	 * Size in bytes of the pen parameters feature report.
	 */
	SCRIPTEL_FEATURE_PENPARAMETERS_SIZE = 18,
	/**
	 * Size in bytes of the touch parameters feature report.
	 */
	SCRIPTEL_FEATURE_TOUCHPARAMETERS_SIZE = 22,
	/**
	* Size in bytes of the output configuration report
	*/
	SCRIPTEL_FEATURE_OUTPUTCONFIGURATION_SIZE = 10,
	/**
	 * Size in bytes of the peripheral configuration report
	 */
	SCRIPTEL_FEATURE_PERIPHERAL_CONFIGURATION_SIZE = 17,
	/**
	* Size in bytes of the Device Status report
	*/
	SCRIPTEL_FEATURE_DEVICE_STATUS_SIZE= 73,
	/**
	 * Size in bytes of the calibration data report
	 */
	 SCRIPTEL_FEATURE_CALIBRATION_DATA_SIZE = 49,
	/**
	 * Size in bytes of the display info feature report.
	 */
	SCRIPTEL_FEATURE_DISPLAYINFO_SIZE = 10,
	/**
	 * Size in bytes of the display content feature report.
	 */
	SCRIPTEL_FEATURE_DISPLAYCONTENT_SIZE = 2,
	/**
	 * Size in bytes of the display settings feature report.
	 */
	SCRIPTEL_FEATURE_DISPLAYSETTINGS_SIZE = 7,
	/**
	 * Size in bytes of the update display chunk feature report.
	 */
    SCRIPTEL_FEATURE_UPDATEDISPLAYCHUNK_SIZE = 2049,
    /**
     * Size in bytes of the region update feature report.
     */
    SCRIPTEL_FEATURE_REGIONUPDATE_SIZE = 12,
    /**
     * Size in bytes of the region delete feature report.
     */
	SCRIPTEL_FEATURE_REGIONDELETE_SIZE = 2,
	/**
	 * Size in bytes of the region edit complete feature report.
	 */
	SCRIPTEL_FEATURE_REGIONEDITCOMPLETE_SIZE = 2,
	/**
	* Size in bytes of the resource  index feature report.
	*/
	SCRIPTEL_FEATURE_RESOURCE_IDX_SIZE = 2,
	/**
	* Size in bytes of the resource info feature report.
	*/
	SCRIPTEL_FEATURE_RESOURCE_INFO_SIZE = 20,
	/**
	* Size in bytes of the resource availability feature report.
	*/
	SCRIPTEL_FEATURE_RESOURCE_AVAILABILITY_SIZE = 18,
	/**
	* Size in bytes of the resource feature report.
	*/
	SCRIPTEL_FEATURE_RESOURCE_SIZE = 1045,
	/**
	* Size in bytes of the delete resource feature report.
	*/
	SCRIPTEL_FEATURE_DELETE_RESOURCE_SIZE = 2,
	/**
	* Size in bytes of the region index feature report.
	*/
	SCRIPTEL_FEATURE_REGION_IDX_SIZE = 2,
	/**
	* Size in bytes of the screen index feature report.
	*/
	SCRIPTEL_FEATURE_SCREEN_IDX_SIZE = 2,
	/**
	* Size in bytes of the delete region feature report.
	*/
	SCRIPTEL_FEATURE_DELETE_REGION_SIZE = 2,
	/**
	* Size in bytes of the screen feature report.
	*/
	SCRIPTEL_FEATURE_SCREEN_SIZE = 80,
	/**
	* Size in bytes of the delete screen feature report.
	*/
	SCRIPTEL_FEATURE_DELETE_SCREEN_SIZE = 2,
	/**
	* Size in bytes of the modify screen feature report.
	*/
	SCRIPTEL_FEATURE_MODIFY_SCREEN_SIZE = 13,
	/**
	* Size in bytes of the screen region info feature report.
	*/
	SCRIPTEL_FEATURE_SCREEN_REGION_INFO_SIZE = 25,
	/**
	* Size in bytes of the active screen feature report.
	*/
	SCRIPTEL_FEATURE_ACTIVE_SCREEN_SIZE = 2,
	/**
	* Size in bytes of the screen region commit feature report.
	*/
	SCRIPTEL_FEATURE_SCREEN_REGION_SAVE_RELOAD_SIZE = 2,
	/**
	 * Size in bytes of the calibrate feature report.
	 */
	SCRIPTEL_FEATURE_CALIBRATE_SIZE = 2,
	/**
	 * Size in bytes of the coordinate size input report.
	 */
	SCRIPTEL_INPUT_COORDINATE_SIZE = 6,
	/**
	 * Size in bytes of the extended coordinate input report.
	 */
	SCRIPTEL_INPUT_EXTENDEDCOORDINATE_SIZE = 7,
	/**
	 * Size in bytes of the ADC values input report.
	 */
	SCRIPTEL_INPUT_ADCVALUES_SIZE = 11,
	/**
	 * Size in bytes of the magnetic card data input report.
	 */
	SCRIPTEL_INPUT_MAGCARDDATA_SIZE = 231,
	/**
	 * Size in bytes of the debug coordinate input report.
	 */
	SCRIPTEL_INPUT_DEBUGCOORDINATE_SIZE = 22,
	/**
	 * Size in bytes of the get/set Pixel Retrieval Location feature report
	 */
	SCRIPTEL_FEATURE_PIXEL_RETRIEVAL_LOCATION_SIZE = 5,
	/**
	  * Size in bytes of the get/set pixels feature report
	  */
	SCRIPTEL_FEATURE_PIXELS_SIZE = 1031,
	/**
	  * Size in bytes of the engineering test feature report
	  */
	SCRIPTEL_FEATURE_ENGINEERING_TEST_SIZE = 67
} scriptel_hid_report_size;

/**
 * This enumeration contains all of the valid run modes for ScripTouch firmware.
 */
typedef enum {
	/**
	 * Command mode, turns off the ASIC and accepts commands.
	 */
	SCRIPTEL_RUNMODE_COMMAND = 0,
	/**
	 * Touch mode, configures the digitizer to accept touch input. This mode
	 * may not be supported in all devices.
	 */
	SCRIPTEL_RUNMODE_TOUCH = 1,
	/**
	 * Pen mode, configures the digitizer to accept pen input.
	 */
	SCRIPTEL_RUNMODE_PEN = 2,
	/**
	 * Auto Touch mode, alternates quickly between pen and touch mode to detect
	 * either. This mode may not be supported in all devices.
	 */
	SCRIPTEL_RUNMODE_AUTOTOUCH = 4,
	/**
	 * Defined but unused, reserved for later use.
	 */
	SCRIPTEL_RUNMODE_TOUCHSINGLE = 8,
	/**
	 * Defined but unused, reserved for later use.
	 */
	SCRIPTEL_RUNMODE_TOUCHSTREAM = 10,
	/**
	 * Defined but unused, reserved for later use.
	 */
	SCRIPTEL_RUNMODE_PENSINGLE = 16,
	/**
	 * Defined but unused, reserved for later use.
	 */
	SCRIPTEL_RUNMODE_PENSTREAM = 18,
	/**
	 * Defined but unused, reserved for later use.
	 */
	SCRIPTEL_RUNMODE_PENCONTINUOUS = 20
} scriptel_device_run_mode;

/**
 * This enumeration contains all supported acquisition modes.
 */
typedef enum {
	/**
	 * Device is in the touch acquisition mode.
	 */
	SCRIPTEL_ACQMODE_TOUCH = 0,
	/**
	 * Device is in the pen acquisition mode.
	 */
	SCRIPTEL_ACQMODE_PEN = 1,
	/**
	 * Device is in the auto-touch acquisition mode.
	 */
	SCRIPTEL_ACQMODE_AUTOTOUCH = 2,
	/**
	 * Device is in command mode.
	 */
	SCRIPTEL_ACQMODE_COMMAND = 3
} scriptel_device_acquisition_mode;

/**
 * This enumeration contains all supported pen modes.
 */
typedef enum {
	/**
	 * Pen is in single mode.
	 */
	SCRIPTEL_PTMODE_SINGLE = 0,
	/**
	 * Pen is in single liftoff mode.
	 */
	SCRIPTEL_PTMODE_SINGLE_LIFTOFF = 1,
	/**
	 * Pen is in stream mode.
	 */
	SCRIPTEL_PTMODE_STREAM = 2,
	/**
	 * Pen is in stream liftoff mode.
	 */
	SCRIPTEL_PTMODE_STREAM_LIFTOFF = 3,
	/**
	 * Pen is in continuous mode.
	 */
	SCRIPTEL_PTMODE_CONTINUOUS = 4
} scriptel_device_pentouch_mode;

/**
 * This enumeration contains all valid region types and their numeric identifiers.
 */
typedef enum {
	/**
	 * Type for containers.
	 */
	SCRIPTEL_REGION_TYPE_CONTAINER = 0,
	/**
	 * Type for buttons.
	 */
	SCRIPTEL_REGION_TYPE_BUTTON = 1,
	/**
	 * Type for lines.
	 */
	SCRIPTEL_REGION_TYPE_LINE = 2,
	/**
	 * Type for graphics.
	 */
	SCRIPTEL_REGION_TYPE_GRAPHIC = 3,
	/**
	* Type for text.
	*/
	SCRIPTEL_REGION_TYPE_TEXT = 4,
	/**
	 * Type for null regions.
	 */
	SCRIPTEL_REGION_TYPE_NULL = 0xFF
} scriptel_region_type;

/**
 * This enumeration contains output modes; whether the output is to be held off, or sent immediatly.
 */
typedef enum {
	/**
	 * Send coordinates as soon as possible
	 */
	REAL_TIME = 0,
	/**
	 * Don't send coordinates until the OK button is pressed
	 */
	TRANSMIT_ON_CONFIRM = 1,
	/**
	 * Don't send coordinates until the host requests them
	 */
	TRANSMIT_ON_HOST_COMMAND = 2
} scriptel_output_mode;

/**
 * This enumeration contains valid compression modes.
 */
typedef enum {
    /**
     * No compression, low resolution output
     */
    SCRIPTEL_OUTPUT_UNCOMPRESSED_LORES = 0,
    /**
     * Compressed, low resolution output  (currently unsupported in firmware)
     */
    SCRIPTEL_OUTPUT_COMPRESSED_LORES = 1,
    /**
     * Uncompressed, high resolution output (currently unsupported in firmware)
     */
    SCRIPTEL_OUTPUT_UNCOMPRESSED_HIRES = 2,
    /**
     * Compressed, high resolution output
     */
    SCRIPTEL_OUTPUT_COMPRESSED_HIRES = 3
} scriptel_compression_mode;

typedef enum {
	/**
	 * Peripheral outputs data in OEM Format
	 */
	SCRIPTEL_MAG_CARD_OEM_FORMAT = 0x01,
	/**
	* Peripheral outputs data in Keyboard Format.
	*/
	SCRIPTEL_MAG_CARD_KEYBOARD = 0x04,
	/**
	* Peripheral outputs data in Keyboard Format with Preamble, SCRIPTEL_MAG_CARD_KEYBOARD must be active.
	*/
	SCRIPTEL_MAG_CARD_KEYBOARD_PREAMBLE = 0x08
} scriptel_mag_card_output;

/**
 * This enumeration contains valid flags for the command byte
 * field in the set display settings report.
 */
typedef enum scriptel_displaysettings_command {
    /**
     * The display settings will only persist during runtime.
     * Resetting the device will cause the values to revert.
     */
    SCRIPTEL_COMMAND_RUNTIME = 0,
    /**
     * This display settings will be commited to flash. A power
     * reset will not change the values.
     */
    SCRIPTEL_COMMAND_COMMIT = 1,
    /**
     * Ignore the values in the report, restore the default values.
     * The default values are not written to flash.
     */
    SCRIPTEL_COMMAND_RESTORE_DEFAULTS = 128
} scriptel_displaysettings_command;

/**
 * This structure contains information about a particular sample
 * that is common to both pen and touch samples.
 */
typedef struct {
	/**
	 * This will be true if the pen (or finger) is currently against the
	 * screen.
	 */
	unsigned char down;
	/**
	 * Reserved for later use.
	 */
	unsigned char reserved1;
	/**
	 * Data source of the sample.
	 */
	unsigned char data_source;
	/**
	 * Reserved for later use.
	 */
	unsigned char reserved2;
} scriptel_sample_info_common;

/**
 * This structure contains information about a pen sample.
 */
typedef struct {
	/**
	 * This will be true if the pen is currently against the screen.
	 */
	unsigned char tip_switch;
	/**
	 * Reserved for later use.
	 */
	unsigned char reserved1;
	/**
	 * Data source of the sample.
	 */
	unsigned char data_source;
	/**
	 * Reserved for later use.
	 */
	unsigned char reserved2;
} scriptel_sample_info_pen;

/**
 * This structure contains information about a touch sample.
 */
typedef struct {
	/**
	 * This will be true if skin is currently contacting the glass.
	 */
	unsigned char contact;
	/**
	 * For multi-touch devices this will contain the identifier of the
	 * contact.
	 */
	unsigned char contact_id;
	/**
	 * Reserved for later use.
	 */
	unsigned char reserved1;
	/**
	 * Data source of the touch event.
	 */
	unsigned char data_source;
	/**
	 * Reserved for later use.
	 */
	unsigned char reserved2;
} scriptel_sample_info_touch;

/**
 * This union represents all types of sample information that can be associated
 * with a particular sample.
 */
typedef union {
	/**
	 * Sample information relevant to both pen and touch.
	 */
	scriptel_sample_info_common common;
	/**
	 * Sample information relevant to only pen input.
	 */
	scriptel_sample_info_pen pen;
	/**
	 * Sample information relevant to only touch input.
	 */
	scriptel_sample_info_touch touch;
} scriptel_sample_info;

/**
 * This structure represents a basic coordinate without any region information.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_INPUT_COORDINATE.
	 */
	unsigned char report_id;
	/**
	 * Information about the sample that produced this coordinate.
	 */
	scriptel_sample_info sample_info;
	/**
	 * Horizontal component of the sample.
	 */
	unsigned short x;
	/**
	 * Vertical component of the sample.
	 */
	unsigned short y;
} scriptel_hid_input_coordinate;

/**
 * This structure represents an extended coordinate that does contain region information.
 */
typedef struct {
	/**
	 * Report ID, should always be set to #SCRIPTEL_INPUT_EXTENDEDCOORDINATE.
	 */
	unsigned char report_id;
	/**
	 * Information about the sample that produced this coordinate.
	 */
	scriptel_sample_info sample_info;
	/**
	 * Horizontal component of the sample.
	 */
	unsigned short x;
	/**
	 * Vertical component of the sample.
	 */
	unsigned short y;
	/**
	 * The region number associated with the most recent stroke (since the last pen down event).
	 */
	unsigned char region;
} scriptel_hid_input_extended_coordinate;

/**
 * This enumeration contains all valid types of ADC data.
 */
typedef enum {
	/**
	 * Identifier for a scan sample.
	 */
	SCRIPTEL_ADC_DATA_TYPE_SCAN = 2,
	/**
	 * Identifier for a channel sample.
	 */
	SCRIPTEL_ADC_DATA_TYPE_CHANNEL = 3
} scriptel_adc_data_type;

/**
 * This structure represents a sample from the ADC.
 */
typedef struct {
	/**
	 * Report ID, should always be set to #SCRIPTEL_INPUT_ADCVALUES.
	 */
	unsigned char report_id;
	/**
	 * The data type of this sample.
	 */
	scriptel_adc_data_type data_type;
	/**
	 * This union will either contain scan values or channel values from the ADC.
	 */
	union {
		/**
		 * This structure contains information about scan values.
		 */
		struct {
			/**
			 * Sample information associated with this scan.
			 */
			scriptel_sample_info sample_info;
			/**
			 * Maximum horizontal scan.
			 */
			unsigned short x_max;
			/**
			 * Minimum horizontal scan.
			 */
			unsigned short x_min;
			/**
			 * Maximum vertical scan.
			 */
			unsigned short y_max;
			/**
			 * Minimum vertical scan.
			 */
			unsigned short y_min;
		} scan_values;
		/**
		 * This structure contains information about channel values.
		 */
		struct {
			/**
			 * The channel number of this sample.
			 */
			unsigned char channel_number;
			/**
			 * The first sample.
			 */
			unsigned short sample1;
			/**
			 * The second sample.
			 */
			unsigned short sample2;
			/**
			 * The third sample.
			 */
			unsigned short sample3;
			/**
			 * The forth sample.
			 */
			unsigned short sample4;
		} channel_values;
	} adc_data;
} scriptel_hid_input_adc_values;

/**
 * This enumeration contains all of the valid magnetic card types.
 */
typedef enum {
	/**
	 * Identifier for financial (credit) cards.
	 */
	SCRIPTEL_CARDSWIPE_TYPE_FINANCIAL = 0x01,
	/**
	 * Identifier for identification (state identification) cards.
	 */
	SCRIPTEL_CARDSWIPE_TYPE_IDENTIFICATION = 0x02,
	/**
	 * Identifier for unknown.
	 */
	SCRIPTEL_CARDSWIPE_TYPE_UNKNOWN = 0xFF
} scriptel_cardswipe_type;

/**
 * This enumeration contains error codes that can be passed back when
 * reading magnetic card tracks.
 */
typedef enum {
	/**
	 * The track read is valid and can be used.
	 */
	SCRIPTEL_CARDSWIPE_STATUS_VALID = 0x00,
	/**
	 * A parity error was found.
	 */
	SCRIPTEL_CARDSWIPE_STATUS_PARITY_ERROR = 0x01,
	/**
	 * The magnetic card is in an unknown format.
	 */
	SCRIPTEL_CARDSWIPE_STATUS_UNKNOWN_FORMAT = 0x10,
	/**
	 * The checksum of the card was incorrect.
	 */
	SCRIPTEL_CARDSWIPE_STATUS_CORRUPT = 0x11
} scriptel_cardswipe_track_status;

/**
 * This enumeration contains all of the valid power states for the ASIC.
 */
typedef enum {
	/**
	 * The ASIC is on and running.
	 */
	SCRIPTEL_ASIC_POWER_STATE_NORMAL = 0,
	/**
	 * The ASIC is off.
	 */
	SCRIPTEL_ASIC_POWER_STATE_POWERDOWN = 1,
	/**
	 * The ASIC is in standby mode.
	 */
	SCRIPTEL_ASIC_POWER_STATE_STANDBY = 2
} scriptel_asic_power_state;

/**
 * This enumeration contains all of the valid calibration points.
 */
typedef enum {
	/**
	 * Calibration point is in the center of the screen.
	 */
	SCRIPTEL_CALIBRATION_POINT_CENTER = 0,
	/**
	 * Calibration point is in the upper left of the screen.
	 */
	SCRIPTEL_CALIBRATION_POINT_UPPERLEFT = 2,
	/**
	 * Calibration point is in the lower left of the screen.
	 */
	SCRIPTEL_CALIBRATION_POINT_LOWERLEFT = 4,
	/**
	 * Calibration point is in the lower right of the screen.
	 */
	SCRIPTEL_CALIBRATION_POINT_LOWERRIGHT = 6,
	/**
	 * Calibration point is in the upper right of the screen.
	 */
	SCRIPTEL_CALIBRATION_POINT_UPPERRIGHT = 8
} scriptel_calibration_point;

/**
 * This structure carries information regarding a magnetic card
 * swipe.
 */
typedef struct {
	/**
	 * Report ID, should always be set to #SCRIPTEL_INPUT_MAGCARDDATA.
	 */
	unsigned char report_id;
	/**
	 * The format of the card.
	 */
	scriptel_cardswipe_type card_format;
	/**
	 * The status of track one.
	 */
	scriptel_cardswipe_track_status track1_status;
	/**
	 * The status of track two.
	 */
	scriptel_cardswipe_track_status track2_status;
	/**
	 * The status of track three.
	 */
	scriptel_cardswipe_track_status track3_status;
	/**
	 * Track one data, valid only if track1_status indicates success.
	 */
	unsigned char track1[79];
	/**
	 * Track two data, valid only if track2_status indicates success.
	 */
	unsigned char track2[40];
	/**
	 * Track three data, valid only if track3_status indicates success.
	 */
	unsigned char track3[107];
} scriptel_hid_input_card_swipe;

/**
 * This structure contains information about a debug coordinate from a ScripTouch
 * device with debug firmware.
 */
typedef struct {
	/**
	 * Report ID, should always be set to #SCRIPTEL_INPUT_DEBUGCOORDINATE.
	 */
	unsigned char report_id;
	/**
	 * Information about the sample.
	 */
	scriptel_sample_info sample_info;
	/**
	 * Offset of the coordinate.
	 */
	unsigned short offset;
	/**
	 * Maximum horizontal sample
	 */
	unsigned short x_max;
	/**
	 * Minimum horizontal sample
	 */
	unsigned short x_min;
	/**
	 * Maximum vertical sample
	 */
	unsigned short y_max;
	/**
	 * Minimum vertical sample
	 */
	unsigned short y_min;
	/**
	 * The horizontal coordinate after error correction.
	 */
	unsigned short x_errcor;
	/**
	 * The vertical coordinate after error correction.
	 */
	unsigned short y_errcor;
	/**
	 * The horizontal coordinate after being mapped.
	 */
	unsigned short x_mapped;
	/**
	 * The vertical coordinate after being mapped.
	 */
	unsigned short y_mapped;
	/**
	 * The region the last pen down even occurred in.
	 */
	unsigned char region;
} scriptel_hid_input_debug_coordinate;

/**
 * This report is capable of both sending and receiving the current
 * device run mode. Run modes are defined in the scriptel_device_run_mode
 * enumeration.
 */
typedef struct {
	/**
	 * Report ID, should always be set to #SCRIPTEL_FEATURE_RUNMODE.
	 */
	unsigned char report_id;
	/**
	 * Run mode the device is currently in or should be set to.
	 */
	scriptel_device_run_mode run_mode;
} scriptel_hid_feature_run_mode;

/**
 * This enumeration contains values for the capabilities bit-mask
 * in the compatibility report. Values indicate that a device supports
 * a particular type of input report.
 */
typedef enum {
	/**
	 * The device supports vendor-specific (Scriptel) HID input reports.
	 */
	SCRIPTEL_SUPPORTED_REPORTS_HID = 1,
	/**
	 * The device supports HID mouse input reports.
	 */
	SCRIPTEL_SUPPORTED_REPORTS_MOUSE = 2,
	/**
	 * The device supports HID pen input reports.
	 */
	SCRIPTEL_SUPPORTED_REPORTS_PEN = 4,
	/**
	 * The device supports HID touch input reports.
	 */
	SCRIPTEL_SUPPORTED_REPORTS_TOUCH = 8,
	/**
	 * The device supports HID keyboard input reports.
	 */
	SCRIPTEL_SUPPORTED_REPORTS_KEYBOARD = 16
} scriptel_supported_reports;

/**
 * This enumeration contains values for the coordinate_modes capabilities bit-mask.
 */
typedef enum {
	/**
	 * The device supports streaming output.
	 */
	SCRIPTEL_MODE_STREAMING = 0x01,
	/**
	 * The device supports coordinate buffering (batch) output.
	 */
	SCRIPTEL_MODE_BUFFERING = 0x02,
	/**
	 * The device supports coordinate compression.
	 */
	SCRIPTEL_MODE_COMPRESSION = 0x04,
	/**
	 * The device supports coordinate throttling.
	 */
	SCRIPTEL_MODE_THROTTLING = 0x08
} scriptel_supported_transfer_modes;

/**
 * This enumeration contains the different types of screens that
 * can be embedded in ScripTouch products.
 */
typedef enum {
	/**
	 * The device has no display equipped.
	 */
	SCRIPTEL_DISPLAY_TYPE_NONE = 0,
	/**
	 * The device has a 5x1 inch 240x64 pixel black and white screen equipped.
	 */
	SCRIPTEL_DISPLAY_TYPE_240X64_1X5_BLACKWHITE = 1,
	/**
	 * The device has a 5x1 inch 240x64 pixel gray-scale screen equipped.
	 */
	SCRIPTEL_DISPLAY_TYPE_240X64_1X5_GRAYSCALE = 2
} scriptel_display_type;

/**
 * This enumeration contains the different types of card readers that
 * can be embedded into a ScripTouch product.
 */
typedef enum {
	/**
	 * No card reader equipped.
	 */
	SCRIPTEL_CARDREADER_NONE = 0,
	/**
	 * Vendor-specific HID card reader equipped.
	 */
	SCRIPTEL_CARDREADER_HID = 1,
	/**
	 * Keyboard card reader equipped - no preamble.
	 */
	SCRIPTEL_CARDREADER_KEYBOARD_BASIC = 2,
	/**
	 * Keyboard card reader equipped - Scriptel preamble.
	 */
	SCRIPTEL_CARDREADER_KEYBOARD_PREAMBLE = 3,
	/**
	 * Card read can switch between HID and Keyboard.
	 */
	SCRIPTEL_CARDREADER_INTERCHANGEABLE = 131,
	/**
	 * Vendor-specific HID card reader equipped.
	 */
	SCRIPTEL_MAG_CARDREADER_HID = 0x01,
	/**
	 * Keyboard card reader equipped.
	 */
	SCRIPTEL_MAG_CARDREADER_KEYBOARD = 0x02,
	/**
	 * Reader capable of switching output modes.
	 */
	SCRIPTEL_MAG_CARDREADER_CHANGEABLE = 0x80
} scriptel_card_reader;

typedef enum {
	/**
	 *	Set pen coordinates in screen units
	 */
	SCRIPTEL_ENGINEERING_TEST_DISPLAY = 1,	// Mike Bodnar calls this 'Local'
	/**
	 * Set pen coordinates in the native output unit; e.g. 0-499 for EasyScript uncompressed, 0-2999 for EasyScript compressed, 0-65535 for ProScript
	 */
	SCRIPTEL_ENGINEERING_TEST_NATIVE = 2	// Mike Bodnar calls this 'Host'
} scriptel_engineering_test;


/**
 * This structure describes the capabilities report of a ScripTouch
 * device. This report details what capabilities the connected device has.
 */
typedef struct {
	/**
	 * Report ID, if successful should be set to #SCRIPTEL_FEATURE_CAPABILITIES.
	 */
	unsigned char report_id;
	/**
	 * A bit-mask of values contained in #scriptel_supported_reports. Determines
	 * what types of input reports can be returned by this device.
	 */
	unsigned short supported_reports;
	/**
	 * Bitmask indicating which coordinate output features are available.
	 */
	unsigned char coordinate_modes;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved1;
	/**
	 * The type of display the unit is equipped with.
	 */
	scriptel_display_type display_id;
	/**
	 * The number of back-light brightness levels the unit supports.
	 */
	unsigned char brightness_levels;
	/**
	 * The number of contrast levels the unit supports.
	 */
	unsigned char contrast_levels;
	/**
	 * The color depth in bits of the unit's screen (if equipped).
	 */
	unsigned char color_depth;
	/**
	 * The width of the display in pixels.
	 */
	unsigned short display_width;
	/**
	 * The height of the display in pixels.
	 */
	unsigned short display_height;
	/**
	 * The width of the physical digitizer surface in millimeters.
	 */
	unsigned short digitizer_width;
	/**
	 * The width of the physical digitizer surface in millimeters.
	 */
	unsigned short digitizer_height;
	/**
	 * The minimum coordinate that can be returned in the horizontal direction.
	 */
	unsigned short coordinate_xmin;
	/**
	 * The maximum coordinate that can be returned in the horizontal direction.
	 */
	unsigned short coordinate_xmax;
	/**
	 * The minimum coordinate that can be returned in the vertical direction.
	 */
	unsigned short coordinate_ymin;
	/**
	 * The maximum coordinate that can be returned in the vertical direction.
	 */
	unsigned short coordinate_ymax;
	/**
	 * Reserved for future use.
	 */
	unsigned short calibration_xmin;
	/**
	 * Reserved for future use.
	 */
	unsigned short calibration_xmax;
	/**
	 * Reserved for future use.
	 */
	unsigned short calibration_ymin;
	/**
	 * Reserved for future use.
	 */
	unsigned short calibration_ymax;
	/** 
	 * Reserved for future use.
	 */
	unsigned int reserved_4;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_5;
	/**
	 * The type of card reader the unit is equipped with.
	 */
	scriptel_card_reader mag_reader_id;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_6;
	/**
	 * Reserved for future use.
	 */
	unsigned short reserved_7;
	/**
	 * Defines additional features this device can use.
	 * '1' allows use of new Region, Screen, and Resource commands.
	 */
	unsigned char supports_screens_resources;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_9;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_10;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_11;
	/**
	* Reserved for future use.
	*/
	unsigned int reserved_12;
	/**
	* Reserved for future use.
	*/
	unsigned int reserved_13;
	/**
	* Reserved for future use.
	*/
	unsigned int reserved_14;
} scriptel_hid_capabilities;
/**
 * This report is capable of getting the device configuration.
 */
typedef struct {
	/**
	 * Report ID, should always be set to #SCRIPTEL_FEATURE_DEVICECONFIGURE.
	 */
	unsigned char report_id;
	/**
	 * Device Mode - Will be 1 if the device is configured to output pen digitizer reports, 0 otherwise.
	 */
	unsigned char device_mode;
	/**
	 * Device ID - Presently unused.
	 */
	unsigned char device_id;
} scriptel_hid_feature_device_configure;

/**
 * This report is capable of retrieving the maximum number of track-able contacts
 * in a multi-touch digitizer. Only usable if touch input is supported on the device.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_CONTACTCOUNTMAXIMUM.
	 */
	unsigned char report_id;
	/**
	 * Number of supported simultaneous track-able digitizer contacts.
	 */
	unsigned char contact_count_max;
} scriptel_hid_feature_contact_count_maximum;

/**
 * This report contains ASIC frequency and amplitude settings.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_FREQUENCYAMPLITUDE.
	 */
	unsigned char report_id;
	/**
	 * ASIC Frequency
	 */
	unsigned char frequency;
	/**
	 * ASIC Pen Amplitude
	 */
	unsigned char pen_amplitude;
	/**
	 * ASIC Touch Amplitude
	 */
	unsigned char touch_amplitude;
} scriptel_hid_feature_frequency_amplitude;

/**
 * This report is capable of getting and setting the ASIC power state.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_POWERSTATE.
	 */
	unsigned char report_id;
	/**
	 * ASIC Power State - 1 for on, 0 otherwise.
	 */
	scriptel_asic_power_state power_state;
} scriptel_hid_feature_power_state;

/**
 * This report is capable of getting the device version information.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_VERSIONINFO.
	 */
	unsigned char report_id;
	/**
	 * Kernel version, defunct value, new firmware has this always the same as firmware version. Encoded in Binary Coded Decimal.
	 */
	unsigned char kernel_version[3];
	/**
	 * Version of the installed firmware. Encoded in Binary Coded Decimal.
	 */
	unsigned char firmware_version[3];
	/**
	 * Signature of the ASIC.
	 */
	unsigned short asic_signature;
} scriptel_hid_feature_version;

/**
 * This report is capable of getting the coordinate range from the device.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_COORDINATERANGE.
	 */
	unsigned char report_id;
	/**
	 * Minimum logical value in the horizontal direction to be returned by the device.
	 */
	unsigned short x_min;
	/**
	 * Maximum logical value in the horizontal direction to be returned by the device.
	 */
	unsigned short x_max;
	/**
	 * Minimum logical value in the vertical direction to be returned by the device.
	 */
	unsigned short y_min;
	/**
	 * Maximum logical value in the vertical direction to be returned by the device.
	 */
	unsigned short y_max;
} scriptel_hid_feature_coordinate_range;

/**
 * This report is capable of uploading a new page of firmware to the device.
 */
typedef struct {
	/**
	 * Report ID - Should always be SCRIPTEL_FEATURE_FIRMWAREDOWNLOAD.
	 */
	unsigned char report_id;
	/**
	 * Address of the memory page to overwrite.
	 */
	unsigned int address;
	/**
	 * Block of 128 unsigned chars making up the page to overwrite memory with.
	 */
	unsigned char data[128];
	/**
	 * The checksum of the data.
	 * The checksum can be calculated using the following algorithm:
	 * @code
	 * long checksum = 0;
     * for(int i=1;i<data.length;i++) {
     *     checksum -= data[i];
     * }
	 * checksum &= 0xFF;
	 * @endcode
	 */
	unsigned char checksum;
} scriptel_hid_feature_firmware_download;

/**
 * This report is capable of getting and setting the internal error correction
 * table on the device.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_ERRORCORRECTION.
	 */
	unsigned char report_id;
	/**
	 * Error correction table payload.
	 */
	unsigned char data[2048];
} scriptel_hid_feature_error_correction_table;

/**
 * This report is capable of getting and setting the internal serial number on
 * the device.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_SERIALNUMBER.
	 */
	unsigned char report_id;
	/**
	 * Serial number is a maximum of 32 unsigned chars. The serial number can be padded with
	 * either spaces ' ' (0x20) or NULL (0x00). It's safer to assume there isn't any padding
	 * and treat it like binary data.
	 * Serial number of the device.
	 */
	unsigned char serial_number[32];
	/**
	 * Write Key 1 - Should be 'S' for writing, cleared when reading.
	 */
	unsigned char write_key_1;
	/**
	 * Write Key 2 - Should be 'N' for writing, cleared when reading.
	 */
	unsigned char write_key_2;
} scriptel_hid_feature_serial_number;

/**
 * This report is capable of setting the default run-mode on startup.
 */
typedef struct {
	/**
	 * Report ID - Should always be #SCRIPTEL_FEATURE_SAVECONFIGURATION.
	 */
	unsigned char report_id;
	/**
	 * Startup mode, should be one of #scriptel_device_acquisition_mode.
	 */
	scriptel_device_acquisition_mode startup_mode;
} scriptel_hid_feature_save_configuration;

/**
 * This report indicates to the device that a firmware update session has been
 * completed.
 */
typedef struct {
	/**
	 * Report ID - Should always be #SCRIPTEL_FEATURE_DOWNLOADCOMMANDSTATUS.
	 */
	unsigned char report_id;
	/**
	 * Command status -- currently unused.
	 */
	unsigned short command_status;
} scriptel_hid_feature_download_command_status;

/**
 * This report gets the supported operating modes of the device.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_OPERATINGMODE.
	 */
	unsigned char report_id;
	/**
	 * Bit mask for supported capture modes, corresponds to scriptel_device_acquisition_mode.
	 */
	unsigned char supported_modes;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved;
	/**
	 * Current acquisition mode maps to #scriptel_device_acquisition_mode.
	 */
	unsigned char acquisition_mode;
	/**
	 * Current touch mode, maps to #scriptel_device_pentouch_mode.
	 */
	unsigned char touch_mode;
	/**
	 * Current pen mode, maps to #scriptel_device_pentouch_mode.
	 */
	unsigned char pen_mode;
} scriptel_hid_feature_operating_modes;

/**
 * This report gets and sets general digitizer parameters for the device.
 */
typedef struct {
	/**
	 * Report ID, Should always be SCRIPTEL_FEATURE_GENERALPARAMETERS.
	 */
	unsigned char report_id;
	/**
	 * Whether or not coordinates are going to be sent.
	 */
	unsigned char xy_output_enable;
	/**
	 * Whether or not to send ADC output.
	 */
	unsigned char adc_output_enable;
	/**
	 * Whether or not coordinates get mapped to the host range.
	 */
	unsigned char map_coordinates;
	/**
	 * Whether or not to use channel lock mode.
	 */
	unsigned char channel_lock_enable;
	/**
	 * Channel index for channel lock mode.
	 */
	unsigned char channel_lock_number;
	/**
	 * Reserved -- currently unused.
	 */
	unsigned char reserved;
	/**
	 * Number of the lower left corner.
	 */
	unsigned char corner_lower_left;
	/**
	 * Number of the upper left corner.
	 */
	unsigned char corner_upper_left;
	/**
	 * Number of the lower right corner.
	 */
	unsigned char corner_lower_right;
	/**
	 * Number of the upper right corner.
	 */
	unsigned char corner_upper_right;
	/**
	 * Samples
	 */
	unsigned short samples;
	/**
	 * Filter frequency
	 */
	unsigned char filter_frequency;
} scriptel_hid_feature_general_parameters;

/**
 * This structure allows the reset of a device ASIC.
 */
typedef struct {
	/**
	 * Report ID, should be set to #SCRIPTEL_FEATURE_ASICRESET.
	 */
	unsigned char report_id;
	/**
	 * Indicates whether or not a hardware reset (1) or software reset(0) should be used.
	 */
	unsigned char hardware_reset;
	/**
	 * Reserved for later use.
	 */
	unsigned char reserved;
} scriptel_hid_feature_asic_reset;

/**
 * This structure contains information from an ASIC register.
 */
typedef struct {
	/**
	 * Report ID, should be set to #SCRIPTEL_FEATURE_REGISTERGET.
	 */
	unsigned char report_id;
	/**
	 * Information from the ASIC registers.
	 */
	unsigned char register_info[8];
} scriptel_hid_feature_register_get;

/**
 * This structure allows setting of ASIC registers.
 */
typedef struct {
	/**
	 * Report ID, should be set to #SCRIPTEL_FEATURE_REGISTERSET.
	 */
	unsigned char report_id;
	/**
	 * Register to update.
	 */
	unsigned char register_id;
	/**
	 * Value to put into register.
	 */
	unsigned char register_value;
} scriptel_hid_feature_register_set;

/**
 * This structure contains information about pen parameters.
 */
typedef struct {
	/**
	 * Report ID, should be set to #SCRIPTEL_FEATURE_PENPARAMETERS.
	 */
	unsigned char report_id;
	/**
	 * Proximity of the pen.
	 */
	unsigned short proximity;
	/**
	 * Automatic pen threshold.
	 */
	unsigned short auto_pen_threshold;
	/**
	 * Automatic touch threshold.
	 */
	unsigned short auto_touch_threshold;
	/**
	 * Amount of ADC averaging.
	 */
	unsigned char adc_average;
	/**
	 * Amount of Coordinate averaging.
	 */
	unsigned char coordinate_average;
	/**
	 * Offset interval.
	 */
	unsigned short offset_interval;
	/**
	 * De-bounce pre-down amount.
	 */
	unsigned char debounce_pre_down;
	/**
	 * De-bounce post-down amount.
	 */
	unsigned char debounce_post_down;
	/**
	 * De-bounce pre-up amount.
	 */
	unsigned char debounce_pre_up;
	/**
	 * De-bounce post-up amount.
	 */
	unsigned char debounce_post_up;
	/**
	 * Whether or not error correction is enabled.
	 */
	unsigned char error_correction_enable;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Whether or not offset correction is enabled.
	 */
	unsigned char offset_correction_enable;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * Whether or not the tip switch is inverted.
	 */
	unsigned char tip_inverted;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_3;
	/**
	 * Interrupt rate.
	 */
	unsigned short interrupt_rate;
} scriptel_hid_feature_pen_parameters;

typedef struct {
	/**
	* Report ID, should be set to #SCRIPTEL_FEATURE_REGISTERGET.
	*/
	unsigned char report_id;
	unsigned short start_x;
	unsigned short start_y;
} scriptel_hid_feature_pixel_retrieval_location;

typedef struct {
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
} argb;

/**
 * This structure contains pixel information for getting or setting a portion of the screen
 */
typedef struct {
	/**
	* Report ID, should be set to #SCRIPTEL_FEATURE_REGISTERGET.
	*/
	unsigned char report_id;
	/**
	* The X position to begin at
	*/
	unsigned short start_x;
	/**
	* The y position to begin at
	*/
	unsigned short start_y;
	/**
	 * The number of pixels to get or set
	 */
	unsigned short number_of_pixels;
	union color_data {
		/**
		 * ARGB[256] in one 32 bit word
		 */
		unsigned long composit[256];
		/**
		 * A, R, G, B [256] with reference to each color
		 */
		argb color[256];
	} color_data;
} scriptel_hid_feature_pixel_values;

/**
 * This structure contains information about touch parameters.
 */
typedef struct {
	/**
	 * Report ID, should be set to #SCRIPTEL_FEATURE_TOUCHPARAMETERS.
	 */
	unsigned char report_id;
	/**
	 * Amount of ADC averaging.
	 */
	unsigned char adc_average;
	/**
	 * Amount of coordinate averaging.
	 */
	unsigned char coordinate_average;
	/**
	 * Slope down amount.
	 */
	short slope_down;
	/**
	 * Slope liftoff amount.
	 */
	short slope_liftoff;
	/**
	 * Slope minimum.
	 */
	unsigned char slope_minimum;
	/**
	 * Pressure Single
	 */
	unsigned short pressure_single;
	/**
	 * Pressure multiple
	 */
	unsigned short pressure_multiple;
	/**
	 * Gain
	 */
	unsigned char gain;
	/**
	 * Whether or not error correction is enabled.
	 */
	unsigned char error_correction_enable;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved;
	/**
	 * Scale in the horizontal.
	 */
	unsigned short scale_x;
	/**
	 * Offset in the horizontal.
	 */
	short offset_x;
	/**
	 * Scale in the vertical.
	 */
	unsigned short scale_y;
	/**
	 * Offset the the vertical.
	 */
	short offset_y;
} scriptel_hid_feature_touch_parameters;

/**
 * This structure contains information about output configuration.
 */
typedef struct {
    /**
     * Report ID, should be set to #SCRIPTEL_FEATURE_OUTPUTCONFIGURATION.
     */
    unsigned char report_id;
	/**
	* OutputReport, flags for future use, set to 0
	*/
    unsigned long output_report;
    /**
     * The output mode to use, see scriptel_output_mode.
     */
	scriptel_output_mode output_mode;
    /**
     * The compression mode to use, see scriptel_compression_mode.
     */
	scriptel_compression_mode compression;
    /**
     * Output rate, USB packets per second, 0 = as fast as possible
     */
    unsigned short packets_per_second;
    /**
     * SaveAsDefault
     */
    unsigned char save_as_default;
    /**
     * Load Factory Settings
     */
    unsigned char load_factory_settings;
} scriptel_hid_feature_output_parameters;
/**
 * This structure contains information about peripheral configuration.
 */
typedef struct {
	/**
	 * Report ID, should be set to #SCRIPTEL_FEATURE_PERIPHERAL_CONFIGURATION.
	 */
	unsigned char report_id;
	/**
	 * The output mode used by the peripherals, see scriptel_peripheral_output.
	 */
	scriptel_mag_card_output mag_card_output;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved1;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved2;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved3;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved4;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved5;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved6;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved7;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved8;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved9;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved10;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved11;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved12;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved13;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved14;
	/**
	 * SaveAsDefault
	 */
	unsigned char save_as_default;
	/**
	 * Load Factory Settings
	 */
	unsigned char load_factory_settings;
} scriptel_hid_feature_peripheral_configuration;
/**
 * This report contains information about a ScripTouch device's current running
 * state.
 */
typedef struct {
	/**
	 * Report ID, should be set to #SCRIPTEL_FEATURE_DEVICE_STATUS.
	 */
	unsigned char report_id;
	/**
	 * Is initializing
	 */
	unsigned char is_initializing;
	/**
	 * Is Idle (command mode)
	 */
	unsigned char is_idle;
	/**
	 * Calibration required (is not calibrated)
	 */
	unsigned char calibration_required;
	/**
	 * Is calibrating
	 */
	unsigned char is_calibrating;
	/**
	 * All output is locked
	 */
	unsigned char all_output_commands_locked;
	/**
	 * Operating mode commands locked
	 */
	unsigned char operating_mode_commands_locked;
	/**
	 * Region commands locked
	 */
	unsigned char region_commands_locked;
	/**
	 * Screen commands locked
	 */
	unsigned char screen_commands_locked;
	/**
	 * Display commands locked
	 */
	unsigned char display_commands_locked;
	/**
	 * Calibration commands locked
	 */
	unsigned char calibration_commands_locked;
	/**
	 * Reserved for future use
	 */
	unsigned char reserved1;
	/**
	* Reserved for future use
	*/
	unsigned short reserved2;
	/**
	 * The last error that occured, cleared on power up
	 */
	unsigned short last_error;
	/**
	 * An human readable string explaining the error
	 */
	char error_info[64];
} scriptel_hid_feature_device_status;

/**
 * This report contains information about the current calibration state of a
 * ScripTouch device.
 */
typedef struct {
	/**
	 * Report ID, should be set to #SCRIPTEL_FEATURE_CALIBRATION_DATA.
	 */
	unsigned char report_id;
	/**
	* The type of calibration this report is refering to. TBD
	*/
	unsigned char calibration_type;
	/**
	* Whether the device is calibrated
	*/
	unsigned char is_calibrated;
	/**
	* rotation scale
	*/
	unsigned short rotation_scale;
	/**
	* local multiplier scale
	*/
	unsigned short local_multiplier_scale;
	/**
	* host multiplier scale
	*/
	unsigned short host_multiplier_scale;
	/**
	* rotation sine scaled
	*/
	short rotation_sin_scaled;
	/**
	* rotation cosine scaled
	*/
	short rotation_cos_scaled;
	/**
	* local width calibrated
	*/
	unsigned short local_width_calibrated;
	/**
	* local height calibrated
	*/
	unsigned short local_height_calibrated;
	/**
	* local X multiplier scaled
	*/
	long local_x_multiplier_scaled;
	/**
	* local X offset
	*/
	long local_x_offset;
	/**
	* local Y multiplier scaled
	*/
	long local_y_multiplier_scaled;
	/**
	* local Y offset
	*/
	long local_y_offset;
	/**
	* host X multiplier scaled
	*/
	long host_x_multiplier_scaled;
	/**
	* host X offset
	*/
	long host_x_offset;
	/**
	* host Y multiplier scaled
	*/
	long host_y_multiplier_scaled;
	/**
	* host Y offset
	*/
	long host_y_offset;
} scriptel_hid_feature_calibration_data;

/**
 * This structure describes a particular color in 32-bit space.
 */
typedef struct {
	/**
	 * The alpha channel of this color.
	 */
	unsigned char alpha;
	/**
	 * The blue channel of this color.
	 */
	unsigned char blue;
	/**
	 * The green channel of this color.
	 */
	unsigned char green;
	/**
	 * The red channel of this color.
	 */
	unsigned char red;
} scriptel_color;

/**
 * This structure contains static information about a device's display
 * and capabilities.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_DISPLAYINFO.
	 */
	unsigned char report_id;
	/**
	 * Width of the on board display in pixels. This can be used along with
	 * height to determine the aspect ratio of the digitizer. This will also
	 * be set in devices without equipped displays for this purpose.
	 */
	unsigned short width;
	/**
	 * Height of the on board display in pixels. This can be used along with
	 * width to determine the aspect ratio of the digitizer. This will also
	 * be set in devices without equipped displays for this purpose.
	 */
	unsigned short height;
	/**
	 * The color depth of the display in bits: 1 for black and white, 4 for monochrome.
	 */
	unsigned char color_depth;
	/**
	 * The width of a single character.
	 */
	unsigned char text_width;
	/**
	 * The height of a single character.
	 */
	unsigned char text_height;
	/**
	 * The number of regions this device supports.
	 */
	unsigned char region_count;
	/**
	 * The maximum caption length supported on buttons.
	 */
	unsigned char max_caption_length;
} scriptel_hid_feature_display_info;

/**
 * This structure contains information related to display content.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_DISPLAYCONTENT.
	 */
	unsigned char report_id;
	/**
	 * Content
	 */
	unsigned char content;
} scriptel_hid_feature_display_content;

/**
 * This structure contains information about dynamic display settings.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_DISPLAYSETTINGS.
	 */
	unsigned char report_id;
	/**
	 * Brightness of the screen's back-light (if equipped). Set 0 for off
	 * and 100 for full brightness.
	 */
	unsigned char brightness;
	/**
	 * Contrast setting of the screen's display (if equipped). Set 0 for no contrast
	 * and 100 for full contrast.
	 */
	unsigned char contrast;
	/**
	 * Red adjustment value, currently unused.
	 */
	unsigned char red;
	/**
	 * Green adjustment value, currently unused.
	 */
	unsigned char green;
	/**
	 * Blue adjustment value, currently unused.
	 */
	unsigned char blue;
	/**
	 * This flag determines the how the device will apply the settings
     * in this report.
	 */
    scriptel_displaysettings_command set_command;
} scriptel_hid_feature_display_settings;

/**
 * This structure contains information about a raster that should be displayed
 * on the screen of a device.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_UPDATEDISPLAYCHUNK.
	 */
	unsigned char report_id;
	/** 
	 * Where the bitmap should be positioned horizontally.
	 */
	unsigned short x;
	/**
	 * Where the bitmap should be positioned vertically.
	 */
	unsigned short y;
	/**
	 * The width of the raster in pixels.
	 */
	unsigned short width;
	/**
	 * The height of the raster in pixels.
	 */
	unsigned short height;
	/**
	 * The number of bits per pixel, 1 for black and white, 4 for monochrome.
	 */
	unsigned char bits_per_pixel;
	/**
	 * This contains the raw raster. The image should be encoded such that each pixel is encoded
	 * with the same width as defined in bits_per_pixel. The image should start with the top left
	 * corner and end with the bottom right.
	 */
	unsigned char graphic[2039];
} scriptel_hid_feature_display_chunk;

/**
 * This structure contains all of the items that are common to all regions
 * regardless of type.
 */
typedef struct {
	/**
	 * The region number. These start from 1 and go up to region_count in the display information.
	 */
	unsigned char region_number;
	/**
	 * The type of region this is.
	 */
	scriptel_region_type region_type;
	/**
	 * Whether or not this region accepts touch.
	 */
	unsigned char accept_touch;
	/**
	 * Whether or not this region gets decoded. Essentially if true this means that coordinates
	 * can be returned for this region. Otherwise coordinates will skip over a region even if the
	 * pen is in that region and a parent region will be returned instead.
	 */
	unsigned char decode;
	/**
	 * Whether or not this region is visible on the screen.
	 */
	unsigned char visible;
	/**
	 * Flag for in use, has a similar effect to visible. Read only on most devices.
	 */
	unsigned char in_use;
	/**
	 * Whether or not a one pixel frame should be drawn around this region.
	 * The frame will be drawn in the foreground color.
	 */
	unsigned char has_frame;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_2;
	/**
	 * The left-most side of the region.
	 */
	unsigned short x1;
	/**
	 * The right-most side of the region.
	 */
	unsigned short x2;
	/**
	 * The top-most side of the region.
	 */
	unsigned short y1;
	/**
	 * The bottom-most side of the region.
	 */
	unsigned short y2;
	/**
	 * The foreground color of the region.
	 */
	scriptel_color color_foreground;
	/**
	 * The background color of the region.
	 */
	scriptel_color color_background;
} scriptel_device_region_common;

/**
 * This structure contains all container specific information about a region.
 */
typedef struct {
	/**
	 * Common items to all regions.
	 */
	scriptel_device_region_common region_info;
	/**
	 * Whether or not drawing in this area with the pen causes "ink" to be displayed.
	 */
	unsigned char ink_enable;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * Reserved for future use.
	 */
	unsigned short reserved_3;
} scriptel_device_region_container;

/**
 * This structure contains all button specific information about a region.
 */
typedef struct {
	/**
	 * Common items to all regions.
	 */
	scriptel_device_region_common region_info;
	/**
	 * Parent region for this button.
	 */
	unsigned char parent;
	/**
	 * Whether or not this button is enabled.
	 */
	unsigned char enable;
	/**
	 * Whether or not the button will highlight when selected.
	 */
	unsigned char highlight_on_select;
	/**
	 * Whether or not the button will clear when selected.
	 */
	unsigned char clear_on_select;
	/**
	 * Reserved for future use.
	 */
	unsigned short reserved;
	/** 
	 * Caption of the button. This is limited per device, see scriptel_hid_feature_display_info.max_caption_length.
	 */
	char caption[100];
} scriptel_device_region_button;

/**
 * This structure contains all line specific region information.
 */
typedef struct {
	/**
	 * Common items to all regions.
	 */
	scriptel_device_region_common region_info;
	/**
	 * Parent region for this line.
	 */
	unsigned char parent;
	/**
	 * Line width, this is not implemented on all devices.
	 */
	unsigned char width;
	/**
	 * Reserved for future use.
	 */
	unsigned short reserved;
} scriptel_device_region_line;

/**
 * This union contains all types of regions.
 */
typedef union {
	/**
	 * Items common to all regions.
	 */
	scriptel_device_region_common common;
	/**
	 * Container region information.
	 */
	scriptel_device_region_container container;
	/**
	 * Button region information.
	 */
	scriptel_device_region_button button;
	/**
	 * Line region information.
	 */
	scriptel_device_region_line line;
} scriptel_device_region_info;

/**
 * This structure transports a single region.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_REGIONSET.
	 */
	unsigned char report_id;
	/**
	 * Region information.
	 */
	scriptel_device_region_info region;
} scriptel_hid_feature_report_region;

/**
 * This structure describes an update to a region.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_REGIONUPDATE.
	 */
	unsigned char report_id;
	/**
	 * Region number to update.
	 */
	unsigned char region_number;
	/**
	 * Function enable.
	 */
	unsigned char function_enable;
	/**
	 * Whether or not this region should be visible.
	 */
	unsigned char visible;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved1;
	/**
	 * Foreground color to use.
	 */
	scriptel_color color_foreground;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved2;
} scriptel_hid_feature_region_update;

/**
 * Describes a structure that can be used to delete a region.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_REGIONDELETE.
	 */
	unsigned char report_id;
	/**
	 * Region number to delete.
	 */
	unsigned char region_number;
} scriptel_hid_feature_region_delete;

/**
 * This enumeration contains the valid options for region edit complete.
 */
typedef enum {
	/**
	 * Discard all region changes.
	 */
	SCRIPTEL_REGION_DISCARD_CHANGES = 0,
	/**
	 * Save all region changes persistently.
	 */
	SCRIPTEL_REGION_SAVE_CHANGES = 1
} scriptel_region_save_changes;

typedef enum {
	/**
	 * Discard all screen and region changes.
	 */
	SCRIPTEL_SCREEN_REGION_DISCARD_CHANGES = 0,
	/**
	 * Save all screen and region changes persistently.
	 */
	SCRIPTEL_SCREEN_REGION_SAVE_CHANGES = 1,
	/**
	 * Restore all screens and regions to factory defaults.
	 */
	SCRIPTEL_SCREEN_REGION_FACTORY_DEFAULTS = 90

} scriptel_screen_region_save_changes;
/**
 * This structure allows for the saving or discarding of updated region
 * information.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_REGIONEDITCOMPLETE.
	 */
	unsigned char report_id;
	/**
	 * Whether or not to save changes persistently.
	 */
	scriptel_region_save_changes save_changes;
} scriptel_hid_feature_region_edit_complete;
/**
* This structure allows for the saving or discarding of updated screen
* and region information.
*/
typedef struct {
	/**
	* Report ID, should always be #SCRIPTEL_FEATURE_SCREEN_REGION_SAVE_RELOAD.
	*/
	unsigned char report_id;
	/**
	* Whether or not to save changes persistently.
	*/
	scriptel_screen_region_save_changes save_changes;
} scriptel_hid_feature_screen_region_edit_complete;


/**
 * This structure contains information about whether or not the device
 * is currently calibrating.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_CALIBRATE.
	 */
	unsigned char report_id;
	/**
	 * Whether or not the device is in calibration mode.
	 */
	unsigned char calibration_mode;
	/**
	 * If the device is in calibration mode, what point is the device
	 * waiting for?
	 */
	scriptel_calibration_point point;
} scriptel_hid_feature_calibrate;
/**
 * Defines an enumeration of all the resource types.
 */
typedef enum {
	/**
	 * Rext resource.
	 */
	SCRIPTEL_RESOURCE_TYPE_TEXT = 0,
	/**
	 * Graphic resource.
	 */
	SCRIPTEL_RESOURCE_TYPE_GRAPHIC = 1,
	/**
	 * Invalid/Null resource.
	 */
	SCRIPTEL_RESOURCE_TYPE_INVALID = 255
} scriptel_resource_type;

typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_RESOURCE_INFO
	 */
	unsigned char report_id;
	/**
	 * index of the resource on the device.
	 */
	unsigned char resource_number;
	/**
	 * Defines what kind of resource this currently represents.
	 */
	scriptel_resource_type resource_type;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * The total size of the resource in bytes.
	 */
	unsigned long resource_size;
	/**
	 * CRC32 used to authenticate the resource.
	 */
	unsigned long resource_crc;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_3;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_4;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_5;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_6;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_7;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_8;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_9;
} scriptel_hid_feature_resource_info;
/**
 * Defines a structure for a resource on the device.
 * If the resource is bigger than 1024 byte, this structure needs
 * to be called multiple times to get or set a resource.
 * If used to get a resource, resource index will be represented by
 * the resource index.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_RESOURCE
	 */
	unsigned char report_id;
	/**
	 * index of the resource on the device.
	 */
	unsigned char resource_number;
	/**
	 * Defines what kind of resource this currently represents.
	 */
	scriptel_resource_type resource_type;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * The total size of the resource in bytes.
	 * If this is greater than 1024, then this structure represents
	 * a portion of the resource.
	 */
	unsigned long resource_size;
	/**
	 * CRC32 used to authenticate the resource.
	 */
	unsigned long resource_crc;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_3;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_4;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_5;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_6;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_7;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_8;
	/**
	 * Number of valid bytes incoming or outgoing in this structure.
	 */
	unsigned short bytes_valid;
	/**
	 * The data, in bytes.
	 */
	unsigned char resource_data[1024];
} scriptel_hid_feature_resource;
/**
* Structure that can be used to delete a resource.
*/
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_DELETE_RESOURCE.
	 */
	unsigned char report_id;
	/**
	 * The index of the resource to be deleted.
	 */
	unsigned char resource_number;
} scriptel_hid_feature_delete_resource;
/**
 * Defines a structure that represents the amount of open resources
 * on the device. Also determines the amount of space availble for resources.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_RESOURCE_AVAILABILITY.
	 */
	unsigned char report_id;
	/**
	 * Number of available resource saves available.
	 */
	unsigned char number_available;
	/**
	 * Amount of space left in bytes.
	 */
	unsigned long space_available;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_2;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_3;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_4;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_5;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_6;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_7;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_8;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_9;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_10;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_11;
	/**
	 * Reserved for future use.
	 */
	unsigned int reserved_12;
} scriptel_hid_feature_resource_availability;
/**
 * This structure represents a region on the device that can be get or set.
 * Getting a region is based off the region index.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_REGION.
	 */
	unsigned char report_id;
	/**
	 * The region index on the device.
	 */
	unsigned char region_index;
	/**
	 * The type of region this represents.
	 */
	scriptel_region_type region_type;
	/**
	 * Whether or not this region accepts touch.
	 */
	unsigned char accepts_touch;
	/**
	* Whether or not this region gets decoded. Essentially if true this means that coordinates
	* can be returned for this region. Otherwise coordinates will skip over a region even if the
	* pen is in that region and a parent region will be returned instead.
	*/
	unsigned char decode_region;
	/**
	 * Whether or not there is a frame around the region.
	 */
	unsigned char has_frame;
	/**
	 * Determines if this region outputs data to the host.
	 */
	unsigned char output_to_host;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_3;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_4;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_5;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_6;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_7;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_8;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_9;
	/**
	 * X minimum of the region.
	 */
	unsigned short x1;
	/**
	 * X maximum of the region.
	 */
	unsigned short x2;
	/**
	 * Y minimum of the region.
	 */
	unsigned short y1;
	/**
	 * Y maximum of the region.
	 */
	unsigned short y2;
	/**
	* Foreground color of the region.
	*/
	scriptel_color foreground_color;
	/**
	 * Background color of the region.
	 */
	scriptel_color background_color;
} scriptel_hid_feature_region_common;
/**
 * Defines a structure that represents a container/ink region on the device.
 */
typedef struct {
	/**
	 * The common data of the region.
	 */
	scriptel_hid_feature_region_common common;
	/**
	 * Whether ink appears on the screen or not.
	 */
	unsigned char ink_enabled;
	/**
	 * Width of the ink drawn on the screen.
	 */
	unsigned char ink_width;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
} scriptel_hid_feature_region_container;
/**
 * Defines a structure that represents a button region on the device.
 */
typedef struct {
	/**
	 * The common data of the region.
	 */
	scriptel_hid_feature_region_common common;
	/**
	 * Button is enabled.
	 */
	unsigned char enabled;
	/**
	 * Button highlights when it is selected.
	 */
	unsigned char highlight_on_select;
	/**
	 * Sends a clear message when selected.
	 */
	unsigned char clear_on_select;
	/**
	 * Sends a confirm message when selected.
	 */
	unsigned char confirm_on_select;
	/**
	 * Use resource instead of a caption for this button.
	 */
	unsigned char use_resource;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_3;
	/**
	 * Resource displayed when button is idle.
	 */
	unsigned char resource_number_normal;
	/**
	 * Resource displayed when button is selected.
	 */
	unsigned char resource_number_selected;
	/**
	 * Button caption.
	 */
	char caption[100];
} scriptel_hid_feature_region_button;
/**
 * Defines a structure that represents a line region on the device.
 */
typedef struct {
	/**
	 * The common data of the region.
	 */
	scriptel_hid_feature_region_common common;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Line width in pixels.
	 */
	unsigned char line_width;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_3;
} scriptel_hid_feature_region_line;
/**
 * Enum that is used to determine the vertical and horizontal
 * justification of objects within a region.
 */
typedef enum {
	/**
	 * Justifies the resouce to the left.
	 */
	SCRIPTEL_JUSTIFICATION_LEFT = 0,
	/**
	 * Justifies the resouce to the top.
	 */
	SCRIPTEL_JUSTIFICATION_TOP = 0,
	/**
	 * Justifies the resouce to the right.
	 */
	SCRIPTEL_JUSTIFICATION_RIGHT = 1,
	/**
	 * Justifies the resouce to the bottom.
	 */
	SCRIPTEL_JUSTIFICATION_BOTTOM = 1,
	/**
	 * Justifies the resouce to the center.
	 */
	SCRIPTEL_JUSTIFICATION_CENTER = 8
} scriptel_resource_justification;

/**
 * Enum that is used to specify the pen coordinate space 
 */
typedef enum {
	/**
	 * Use the coordinate system of display pixels
	 */
	SCRIPTEL_PEN_COORD_DISPLAY_SPACE = 1,
	/**
	 * Use the coordinate system of device output. This could be 0-499 for EasyScript 1.0, 0-2999 for EasyScript 3.0, or 0-65535 for ProScript.
	 */
	SCRIPTEL_PEN_COORD_DEVICE_SPACE = 2
} scriptel_pen_coord_type;

/**
 * Defines a structure that represents a graphic region on the device.
 */
typedef struct {
	/**
	 * The common data of the region.
	 */
	scriptel_hid_feature_region_common common;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Resource number where the graphic is stored.
	 */
	unsigned char graphic_number;
	/**
	 * Horizontal justification of the graphic within the region.
	 */
	scriptel_resource_justification horizontal_justification;
	/**
	 * Vertical justification of the graphic within the region.
	 */
	scriptel_resource_justification vertical_justification;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
} scriptel_hid_feature_region_graphic;
/**
 * Defines a structure that represents a text region on the device.
 */
typedef struct {
	/**
	 * The common data of the region.
	 */
	scriptel_hid_feature_region_common common;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Resource number where the text is stored.
	 */
	unsigned char resource_number;
	/**
	 * The font used to write the characters to the screen.
	 */
	unsigned char font_number;
	/**
	 * Horizontal justification of the text in this region.
	 */
	scriptel_resource_justification horizontal_justification;
	/**
	 * Vertical justification of the text in this region.
	 */
	scriptel_resource_justification vertical_justification;
} scriptel_hid_feature_region_text;
/**
 * This union contains all region information.
 */
typedef union {
	/**
	 * Common, the foundation of all regions.
	 */
	scriptel_hid_feature_region_common common;
	/**
	 * Container/Ink region.
	 */
	scriptel_hid_feature_region_container container;
	/**
	 * Button region.
	 */
	scriptel_hid_feature_region_button button;
	/**
	 * Line region.
	 */
	scriptel_hid_feature_region_line line;
	/**
	 * Graphic region.
	 */
	scriptel_hid_feature_region_graphic graphic;
	/**
	 * Text region.
	 */
	scriptel_hid_feature_region_text text;
} scriptel_hid_feature_region;
/**
 * Structure that can be used to get or set a screen on the device.
 * Getting a screen is based off the screen index.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_SCREEN.
	 */
	unsigned char report_id;
	/**
	 * Index of the screen these variables represent.
	 */
	unsigned char screen_number;
	/**
	 * Resets the screen timeout if screen is interacted with.
	 */
	unsigned char reset_timeout_on_activity;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_3;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_4;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_5;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_6;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_7;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_8;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_9;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_10;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_11;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_12;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_13;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_14;
	/**
	 * The timeout in seconds for this screen. If 0, screen never times out.
	 */
	unsigned char timeout;
	/**
	 * The screen index that the device moves to when this screen times out. Use 0xFF to do nothing.
	 */
	unsigned char timeout_screen_next;
	/**
	 * Number of regions on this screen.
	 */
	unsigned char region_count;
	/**
	 * List of regions featured on this screen in draw order.
	 */
	unsigned char region_list[100];
	/**
	 * Corresponds to region_list, determines the next screen for each region interaction. Use 0xFF to do nothing.
	 */
	unsigned char screen_next_list[100];
} scriptel_hid_feature_screen;
/**
 * Structure that can be used to delete a screen.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_DELETE_SCREEN.
	 */
	unsigned char report_id;
	/**
	 * The index of the screen to be deleted from the device.
	 */
	unsigned char screen_index;
} scriptel_hid_feature_delete_screen;
/**
 * Structure that can be used to modify a screen.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_MODIFY_SCREEN.
	 */
	unsigned char report_id;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_3;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_4;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_5;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_6;
	/**
	 * Used to add a region to the screen, cannot be used with region_delete.
	 */
	unsigned char region_add;
	/**
	 * Used to remove a region from the screen, cannot be used with region_add.
	 */
	unsigned char region_delete;
	/**
	 * Used to declare we are changing the timeout value of this screen.
	 */
	unsigned char timeout_set;
	/**
	 * The screen we are modifying data on.
	 */
	unsigned char screen_index;
	/**
	 * The region to be modified, if this report is being used for that.
	 */
	unsigned char region_index;
	/**
	 * The screen that appears when this region is interacted with. Use 0xFF to do nothing.
	 */
	unsigned char region_screen_next;
	/**
	 * The amount of time in seconds before this screen times out.
	 */
	unsigned char timeout;
	/**
	 * The index of the screen the device advances to when this screen times out. use 0xFF to do nothing.
	 */
	unsigned char timeout_screen_next;
} scriptel_hid_feature_edit_screen;
/**
 * Describes a structure that gets the screen and region limitations
 * of the current device.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_FEATURE_SCREEN_REGION_INFO.
	 */
	unsigned char report_id;
	/**
	 * Maximum number of screens that are stored on a device.
	 */
	unsigned char screen_count;
	/**
	 * Maximum number of regions that are stored on a device.
	 */
	unsigned char region_count;
	/**
	 * Maximum number of regions that can be placed on a screen.
	 */
	unsigned char regions_per_screen;
	/**
	 * Maximum number of resources that can be stored on the device.
	 */
	unsigned char resource_count;
	/**
	 * Maximum length of a button caption.
	 */
	unsigned char caption_length;
	/**
	 * Maximum length of a text region.
	 */
	unsigned char text_region_length;
	/**
	 * Widths in pixels of the a character drawn on the device.
	 */
	unsigned char text_width;
	/**
	 * Height in pixels of a character drawn on the device.
	 */
	unsigned char text_height;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_1;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_2;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_3;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_4;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_5;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_6;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_7;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_8;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_9;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_10;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_11;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_12;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_13;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_14;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_15;
	/**
	 * Reserved for future use.
	 */
	unsigned char reserved_16;
} scriptel_hid_feature_screen_region_info;

typedef struct scriptel_pen_coordinate {
	/**
	* Report ID, should always be #SCRIPTEL_INPUT_BUTTONDOWN.
	*/
	unsigned char report_id;
	/**
	 * The coordinate space to use. See scriptel_pen_coord_type
	 */
	unsigned char coordinate_type;
	/**
	 * The x position to simulate a coordinate
	 */
	unsigned short x;
	/**
	* The y position to simulate a coordinate
	*/
	unsigned short y;
	/**
	 * Whether the pen is simulated down (1) or up (0)
	 */
	unsigned char pen_down;
} scriptel_pen_coordinate;

/**
 * This structure contains a button down event.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_INPUT_BUTTONDOWN.
	 */
	unsigned char report_id;
	/**
	 * Horizontal component.
	 */
	unsigned short x;
	/**
	 * Vertical component.
	 */
	unsigned short y;
	/**
	 * Button that was pushed down.
	 */
	scriptel_device_region_info* region;
} scriptel_hid_input_button_down;

/**
 * This structure contains a button move event.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_INPUT_BUTTONMOVE.
	 */
	unsigned char report_id;
	/**
	 * Horizontal Component
	 */
	unsigned short x;
	/**
	 * Vertical Component
	 */
	unsigned short y;
	/**
	 * Button that is being moved over.
	 */
	scriptel_device_region_info* region;
} scriptel_hid_input_button_move;

/**
 * This structure contains a button up event.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_INPUT_BUTTONUP.
	 */
	unsigned char report_id;
	/**
	 * Horizontal Component
	 */
	unsigned short x;
	/**
	 * Vertical component
	 */
	unsigned short y;
	/**
	 * Button that was released.
	 */
	scriptel_device_region_info* region;
} scriptel_hid_input_button_up;

/**
 * This structure contains a button press event.
 */
typedef struct {
	/**
	 * Report ID, should always be #SCRIPTEL_INPUT_BUTTONPRESS.
	 */
	unsigned char report_id;
	/**
	 * Horizontal component.
	 */
	unsigned short x;
	/**
	 * Vertical component
	 */
	unsigned short y;
	/**
	 * Button that was pressed.
	 */
	scriptel_device_region_info* region;
} scriptel_hid_input_button_press;

/**
 * This structure is common to all input reports.
 */
typedef struct {
	/**
	 * Report ID, will be one of any of the input report constants.
	 */
	unsigned char report_id;
} scriptel_hid_input_type;

/**
 * This union contains all valid input report types.
 */
typedef union {
	/**
	 * Common items to all input types.
	 */
	scriptel_hid_input_type type;
	/**
	 * Coordinate report.
	 */
	scriptel_hid_input_coordinate coordinate;
	/**
	 * Extended coordinate report.
	 */
	scriptel_hid_input_extended_coordinate extended_coordinate;
	/**
	 * ADC Value report.
	 */
	scriptel_hid_input_adc_values adc_values;
	/**
	 * Magnetic card swipe report.
	 */
	scriptel_hid_input_card_swipe card_swipe;
	/**
	 * Debug Coordinate report.
	 */
	scriptel_hid_input_debug_coordinate debug_coordinate;
	/** 
	 * Button Down report.
	 */
	scriptel_hid_input_button_down button_down;
	/**
	 * Button move report.
	 */
	scriptel_hid_input_button_move button_move;
	/**
	 * Button up report.
	 */
	scriptel_hid_input_button_up button_up;
	/**
	 * Button press report.
	 */
	scriptel_hid_input_button_press button_press;
} scriptel_input_report;

#endif