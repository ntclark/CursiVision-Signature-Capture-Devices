/*******************************************************************************
* scriptel-proscript.h - Copyright 2014 - Scriptel Corporation
* ----
* This header contains the definitions for all of the public structures and
* functions for this library.
******************************************************************************/
#ifndef _SCRIPTEL_PROSCRIPT_H_
#define _SCRIPTEL_PROSCRIPT_H_
/**
* @mainpage Scriptel ProScript Platform Independent C API
* This documentation describes how the ScripTouch ProScript platform
* independent C library functions and how to use it to add ScripTouch
* integration into your project.
*
* If you're just starting out with the library it would probably be a good idea
* to check out the [examples](examples.html). It would also be good to check
* out scriptel_list_devices(), scriptel_open_device() and #scriptel_device.
*
* Supported Platforms:
* * Microsoft Windows (Windows 7 - Windows 10)
* * Microsoft Windows Server (2008 - 2012)
* * Linux (Kernel 3.0.0 or greater, requires hidraw and udev)
* * Apple Macintosh OSX (10.7 or greater)
*/

/**
* @file scriptel-proscript.h
* This file contains the headers for the core of the Scriptel ScripTouch
* ProScript library.
*/

/**
* @example example-connecting.c
* This is a simple example demonstrating how to enumerate and connect to
* a ScripTouch device.
*/

/**
* @example example-reading.c
* This is a simple example demonstrating how to get input from a device.
*/

/**
* @example example-image-cairo.c
* This is a simple example of how to connect to a ScripTouch device and
* output a PNG image using the Cairo drawing library.
*/

/**
* @example example-image-gdiplus.cpp
* This is a simple example of how to connect to a ScripTouch device and
* output a PNG image using the GDI+ drawing library included in Windows.
*/

/**
* @example example-screen-editing.c
* This is a simple example of how to create screens, regions, and resources.  
*  This example requires firmware 4.0.0 or greater.<br> 
* <b>This example will not make the screen edits permanent.</b> To do so, please refer to scriptel_set_screen_region_commit().<br> 
* <b>This example will not expose the new screen to normal operations.</b> To do so, you will need to edit the other screens.<br> 
*/

#include "scriptel-list.h"

//Include the hid report structures.
#include "scriptel-hid-reports.h"

/**
* This enumeration contains a list of codes indicating
* whether or not a device has been added to or removed from
* the system.
*/
typedef enum scriptel_hotplug_type {
	/**
	* The device was added to the system.
	*/
	SCRIPTEL_HOTPLUG_ATTACHED = 0,
	/**
	* The device was removed from the system.
	*/
	SCRIPTEL_HOTPLUG_DETACHED = 1
} scriptel_hotplug_type;

/**
* This enumeration contains all of the return codes that
* functions in this library can return.
*/
typedef enum scriptel_error_code {
	/**
	* This code indicates that the function did not succeed. Check the last error with scriptel_get_last_error() for more details.
	*/
	SCRIPTEL_CODE_ERROR = 0,
	/**
	* This code indicates that the function did succeed.
	*/
	SCRIPTEL_CODE_SUCCESS = 1
} scriptel_error_code;

/**
* This enumeration contains constants relating to the alignment
* of text.
*/
typedef enum scriptel_font_alignment {
	/**
	* Aligns text to the left.
	*/
	SCRIPTEL_ALIGN_LEFT = 0,
	/**
	* Aligns text to the right.
	*/
	SCRIPTEL_ALIGN_RIGHT = 1,
	/**
	* Aligns text to the center.
	*/
	SCRIPTEL_ALIGN_CENTER = 2
} scriptel_font_alignment;

//Forward declaration of scriptel_device.
typedef struct scriptel_device scriptel_device;
typedef struct scriptel_device_driver scriptel_device_driver;
/**
* This typedef should initialize an abstract ScripTouch driver.
*/
typedef scriptel_error_code(*scriptel_driver_init_func)(void);
/**
* This typedef should destroy an abstract ScripTouch driver.
*/
typedef scriptel_error_code(*scriptel_driver_destroy_func)(scriptel_device_driver* driver);
/**
* This typdef should open an abstract ScripTouch device.
*/
typedef scriptel_error_code(*scriptel_open_device_func)(scriptel_device*);
/**
* This typedef should close an abstract ScripTouch device.
*/
typedef scriptel_error_code(*scriptel_close_device_func)(scriptel_device* device);
/**
* This typdef should list a set of abstract ScripTouch devices.
*/
typedef scriptel_error_code(*scriptel_list_devices_func)(scriptel_device_driver* driver, scriptel_device** start);
/**
* This typedef should read from an abstract ScripTouch device.
*/
typedef scriptel_error_code(*scriptel_read_device_func)(scriptel_device* device);
/**
* This typedef should send an output report to an abstract ScripTouch device.
*/
typedef scriptel_error_code(*scriptel_set_output_report_func)(scriptel_device* device, unsigned char* buffer, unsigned int length);
/**
* This typedef should send a feature report to an abstract ScripTouch device.
*/
typedef scriptel_error_code(*scriptel_set_feature_report_func)(scriptel_device* device, unsigned char* buffer, unsigned int length);
/**
* This typedef should get a feature report from an abstract ScripTouch device.
*/
typedef scriptel_error_code(*scriptel_get_feature_report_func)(scriptel_device* device, unsigned char* buffer, unsigned int length);
/**
* This typedef should return whether or not there's a thread running for this driver.
*/
typedef unsigned char(*scriptel_driver_thread_func)(void);
/**
* This typedef should return wait for the device's thread to exit.
*/
typedef void(*scriptel_driver_thread_wait_func)(void);
/**
* This typedef should get a driver name from an abstract driver.
*/
typedef char* (*scriptel_get_driver_name_func)(void);
/**
* This typedef gets called when ProScript is getting destroyed.
*/
typedef void(*scriptel_driver_shutdown_func)(void);

typedef scriptel_list*(*get_hotplug_callbacks_func)(void);

typedef scriptel_error_code(*scriptel_driver_device_free_func)(scriptel_device* device);

typedef void(*scriptel_report_error_func)(char* fmt, ...);

typedef void(*scriptel_debug_func)(const char* function, const char* file, int line_num, int level, char* message, ...);

typedef void(*scriptel_deliver_input_report_func)(scriptel_device* device, unsigned char* report, unsigned int len);

/**
* This structure contains function pointers to the functions
* required to interact with a device.
*/
typedef struct scriptel_device_driver {
	/**
	* Function pointer to driver initializer.
	*/
	scriptel_driver_init_func driver_init;
	/**
	* Function pointer to driver destructor.
	*/
	scriptel_driver_destroy_func driver_destroy;
	/**
	* Function pointer to open this device.
	*/
	scriptel_open_device_func open_device;
	/**
	* Function pointer to close this device.
	*/
	scriptel_close_device_func close_device;
	/**
	* Function pointer to list devices for this driver.
	*/
	scriptel_list_devices_func list_devices;
	/**
	* Function pointer to read from a device.
	*/
	scriptel_read_device_func device_read;
	/**
	* Function pointer to set output report.
	*/
	scriptel_set_output_report_func set_output_report;
	/**
	* Function pointer to set feature report.
	*/
	scriptel_set_feature_report_func set_feature_report;
	/**
	* Function pointer to get feature report.
	*/
	scriptel_get_feature_report_func get_feature_report;
	/**
	* Function pointer to get name function.
	*/
	scriptel_get_driver_name_func get_name;
	/**
	* Whether or not this driver has a thread running.
	*/
	scriptel_driver_thread_func thread_running;
	/**
	* Wait for the thread to finish.
	*/
	scriptel_driver_thread_wait_func wait_thread;
	/**
	* Whether or not this driver has been initialized or not.
	*/
	unsigned char initialized;
	/**
	* This function gets called when proscript is getting destroyed.
	*/
	scriptel_driver_shutdown_func shutdown;
	/**
	* function pointer to get the list of hotplug callbacks.
	*/
	get_hotplug_callbacks_func get_hotplug_callbacks;
	/**
	* Function to call to free a device
	*/
	scriptel_driver_device_free_func free_device;
    /**
     * Function for the driver to call to report a problem.
     */
    scriptel_report_error_func report_error;
    /**
     * Function for the driver to call to do debugging.
     */
    scriptel_debug_func debug;
    /**
     * Function for the driver to call to report an input report.
     */
    scriptel_deliver_input_report_func input_report;
} scriptel_device_driver;

//Include the platform specific components.
#ifdef __linux__
//We're compiling on Linux, require the Linux header.
#include "scriptel-proscript-linux.h"
#elif _WIN32
//We're compiling on Windows, require the Windows header.
#include "scriptel-proscript-windows.h"
#elif __MACH__
//We're compiling on Mac, require the Mac header.
#include "scriptel-proscript-mac.h"
#endif

/**
* This structure represents a physically attached Scriptel ScripTouch device.
* This structure is the primary means of interacting with devices through library
* functions.
*/
typedef struct scriptel_device {
	/**
	* The path to the device. This is an identifier that identifies
	* the operating system resource being used to connect to the
	* device. The format of this member will change between platforms.
	*/
	char* path;
	/**
	* The product id of this device as returned by the operating
	* system's HID driver.
	*/
	unsigned short product_id;
	/**
	* The vendor id of this device as returned by the operating
	* system's HID driver.
	*/
	unsigned short vendor_id;
	/**
	* The textual name of the device as returned by the operating
	* system's HID driver.
	*/
	char* model;
	/**
	* The textual name of the manufacturer as returned by the operating
	* system's HID driver.
	*/
	char* manufacturer;
	/**
	* The 16-byte universally unique identifier of this device. This identifier
	* identifies the signature pad across all platforms.
	*/
	unsigned char* uuid;
	/**
	* Indicates whether or not the device is in the open state.
	*/
	unsigned char open;
	/**
	* Indicates whether or not a button is currently depressed. This
	* shouldn't ever need to be directly read or written.
	*/
	unsigned char button_down;
	/**
	* Stores the last button that was depressed. This
	* shouldn't ever need to be directly read or written.
	*/
	unsigned char last_button;
	/**
	* Cached display information from the device at open time.
	*/
	scriptel_hid_feature_display_info display_info;
	/**
	* Cached version information from the device at open time.
	*/
	scriptel_hid_feature_version version_info;
	/**
	* Pointer to cached region information from the device at open time.
	*/
	scriptel_device_region_info* regions;
	/**
	* A list of input callbacks, these will be called when input is received from
	* the device. Don't access this directly, use scriptel_register_input_callback.
	*/
	scriptel_list* input_callbacks;
	/**
	* A null pointer that can be used by libraries to attach information to the device
	* as it's passed around.
	*/
	void* user_defined;
	/**
	* A pointer to the next device if there is one.
	*/
	struct scriptel_device* next;
	/**
	* The driver that can be used to talk to this device.
	*/
	scriptel_device_driver* driver;
	/**
	* A pointer to anything that a specific driver/plugin may need to manage a device.  Specifically
	* the Signotec plugin attaches a struct of things it needs per device here.  This is intended to be used only
	* by "drivers"
	*/
	void* driver_defined;
	/**
	* A pointer to the font used when displaying text on the device.
	*/
	void* font;
	/**
	* Operating system specific parameters required to interact with the
	* device. This should never be interacted with directly. Leave this last in the
	* structure so that variances in size won't affect the other structure members if
	* the wrong headers / defines get used.
	*/
	scriptel_device_os_params parameters;

} scriptel_device;

/**
* This function pointer type implements a function that can be called to pass
* information received from the device back to the client application.
*/
typedef void(
#ifdef SCRIPTEL_EXPORT_VB_DLL
	__stdcall
#endif 
	*scriptel_input_callback)(scriptel_device* device, scriptel_input_report* report, unsigned char report_id, unsigned char* report_buffer, unsigned int report_len
	);

/**
* This function pointer type implements a function that can be called to pass
* information about connected and disconnected devices back to the client application.
*/
typedef void(
#ifdef SCRIPTEL_EXPORT_VB_DLL
	__stdcall
#endif
	*scriptel_hotplug_callback)(scriptel_hotplug_type type, char* device_path);

/**
* This function takes a pointer and will attempt to enumerate any attached ScripTouch
* devices. If any are found the pointer will be overwritten with the address of the first
* device in the list. The list can be walked through with scriptouch_device->next.
* Don't forget to call scriptel_free_device_list on the first element of the list
* when you're done with the list.
* @param start An over-writable pointer to redirect to the first element in the device list.
* @return The error code returned by the function.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_list_devices(scriptel_device** start);
/**
* This function crawls a scriptel_device list and attempts to free any library managed
* memory associated with the list. Make sure if any of the devices are open that they
* are closed prior to being freed.
* @param device_list List to free.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_free_device_list(scriptel_device* device_list);
/**
* This function counts the number of devices in a given device list.
* @param list List to count
* @return Number of devices in the list.
*/
SCRIPTEL_SYMBOL_LINKAGE(unsigned int) scriptel_count_device_list(scriptel_device* list);
/**
* This function should be called prior to using any other library functions. It should
* only be called once. This initializes the library and any required operating system
* hooks.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_init(void);
/**
* This function should be called when you're done with the library and no longer want
* to interact with any functionality in this library. This will tear down the library
* and any associated operating system hooks. You're still responsible for closing
* resources and freeing memory prior to performing this operation.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_destroy(void);
/**
* This function attempts to open a local device returned by scriptel_list_devices().
* @param device Device to open.
* @return Whether or not opening the device was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_open_device(scriptel_device* device);
/**
* This function attempts to close a local device that was previously opened. This does
* not free the memory associated with the device structure, use scriptel_free_device_list()
* for this after closing the device.
* @param device Device to close.
* @return Whether or not closing the device was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_close_device(scriptel_device* device);
/**
* This function gets the last error message generated by the library. This can be called
* immediately after a non-successful function call to get more information about the
* failure.
* @return Textual information about the last error.
*/
SCRIPTEL_SYMBOL_LINKAGE(char*) scriptel_get_last_error(void);
/**
* This function attempts to read from the passed in device. Received information will
* be passed back through callbacks registered with scriptel_register_input_callback().
* This function blocks until data is received.
* @param device Device to read from.
* @return Whether or not the last read was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_device_read(scriptel_device* device);
/**
* This function will start a new thread that will call scriptel_device_read() in a loop.
* Use this function if you want a thread to read from the device and you don't care about
* implementing the details yourself. If you use this make sure to close the device with
* scriptel_device_read_done() and NOT scriptel_close_device(). The former will ensure
* the thread gets cleaned up properly.
* This function does not block.
* @param device Device to read from.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_device_read_until_done(scriptel_device* device);
/**
* This function will signal the reading thread created by calling scriptel_device_read_until_done()
* that it should end and close the device. This function will block until the thread has
* been completely destroyed. After calling this function it is safe to call scriptel_free_device_list()
* on the device.
* @param device Device to close that is currently reading with scriptel_device_read_until_done()
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_device_read_done(scriptel_device* device);
/**
* This function registers an input callback for a device. Callbacks will be called
* when scriptel_device_read() receives input from a device.
* @param device Device to associate the callback with.
* @param callback Callback to call upon getting data.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_register_input_callback(scriptel_device* device, scriptel_input_callback callback);
/**
* This function unregisters an input callback so that it will no longer be called when
* scriptel_device_read() receives input from a device.
* @param device Device to disassociate the callback from.
* @param callback Callback to unregister.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_unregister_input_callback(scriptel_device* device, scriptel_input_callback callback);
/**
* This function registers a hot-plug callback for the system. Callbacks will be called
* when ScripTouch devices are added to or removed from the system.
* @param callback Callback to register.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_register_hotplug_callback(scriptel_hotplug_callback callback);
/**
* This function unregisters a hot-plug callback from the system.
* @param callback Callback to unregister.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_unregister_hotplug_callback(scriptel_hotplug_callback callback);

/**
* This function gets static display information from the device.
* @param device Device to query.
* @return Static display information.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_display_info) scriptel_get_display_info(scriptel_device* device);
/**
* This function gets the output configuration information from the device.
* @param device Device to query.
* @param report Pointer to a structure to output the configuration report to.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_output_configuration_ref(scriptel_device* device, scriptel_hid_feature_output_parameters* report);
/**
* This function gets the output configuration information from the device.
* @param device Device to query.
* @return Output configuration report.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_output_parameters) scriptel_get_output_configuration(scriptel_device* device);
/**
* This function sends the output configuration to the device.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to set.
* @param settings The output configuration settings.
* @return Success of error code
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_output_configuration(scriptel_device* device, scriptel_hid_feature_output_parameters settings);
/**
* This function prints the information from an output parameters structure to standard out.
* @param settings The structure to print in human readable form.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_output_configuration(scriptel_hid_feature_output_parameters* settings);
/**
* This function gets the peripheral configuration information from the device.
* @param device Device to query.
* @param report Pointer to a structure to output the configuration report to.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_peripheral_configuration_ref(scriptel_device* device, scriptel_hid_feature_peripheral_configuration* report);
/**
* This function gets the output configuration information from the device.
* @param device Device to query.
* @return Peripheral configuration report.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_peripheral_configuration) scriptel_get_peripheral_configuration(scriptel_device* device);
/**
* This function sends the output configuration to the device.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to set.
* @param settings The peripheral configuration settings.
* @return Success of error code
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_peripheral_configuration(scriptel_device* device, scriptel_hid_feature_peripheral_configuration settings);
/**
* This function prints the information from a peripheral configuration structure to standard out.
* @param settings The structure to print in human readable form.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_peripheral_configuration(scriptel_hid_feature_peripheral_configuration* settings);
/**
* This function gets data about calibration. It can be used as part of save and restore calibration
* @param device Device to get the data from.
* @param calibration_data Pointer to a structure to output the calibration report to.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_calibration_data_ref(scriptel_device* device, scriptel_hid_feature_calibration_data* calibration_data);
/**
* This function gets data about calibration. It can be used as part of save and restore calibration
* @param device Device to get the data from.
* @return a copy of the calibration data structure
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_calibration_data) scriptel_get_calibration_data(scriptel_device* device);
/**
* This function sends the calibration data to the device.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to send the calibration data to.
* @param calibration_data Calibration data to send.
* @return an error code. 0 is good.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_calibration_data(scriptel_device* device, scriptel_hid_feature_calibration_data calibration_data);
/**
* This function prints out the contents of the calibration data in human readable format.
* @param calibration_data The data to be printed.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_calibration_data(scriptel_hid_feature_calibration_data* calibration_data);
/**
* This function gets the current status of the device.
* @param device Device from which to obtain status.
* @param status A structure containing the current status of the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_device_status_ref(scriptel_device* device, scriptel_hid_feature_device_status* status);
/**
* This function gets the current status of the device.
* @param device Device from which to obtain status.
* @return A structure containing the current status of the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_device_status) scriptel_get_device_status(scriptel_device* device);
/**
* This function clears the error data from the device.
* @param device Device from which to obtain status.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_clear_last_error(scriptel_device* device);
/**
* This function prints out the status structure in human readable format.
* @param status The data to be printed.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_device_status(scriptel_hid_feature_device_status* status);
/**
* This function gets static display information from the device.
* @param device Device to query.
* @param report reference to the report
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_display_info_ref(scriptel_device* device, scriptel_hid_feature_display_info* report);
/**
* This function prints the information from a display information structure to standard out.
* @param info Information to print.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_display_info(scriptel_hid_feature_display_info* info);
/**
* This function gets device capabilities (if supported).
* @param device Device to query.
* @return Static capability information from the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_capabilities) scriptel_get_capabilities(scriptel_device* device);
/**
* This function gets device capabilities (if supported).
* @param device Device to query.
* @param report Reference to the report
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_capabilities_ref(scriptel_device* device, scriptel_hid_capabilities* report);
/**
* This function prints the static device capabilities.
* @param caps Capabilities
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_capabilities(scriptel_hid_capabilities* caps);
/**
* This function gets dynamic display settings from the device.
* @param device Device to query.
* @return Dynamic display settings.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_display_settings) scriptel_get_display_settings(scriptel_device* device);
/**
* This function gets dynamic display settings from the device.
* @param device Device to query.
* @param report reference to the report
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_display_settings_ref(scriptel_device* device, scriptel_hid_feature_display_settings* report);
/**
* This function prints the information from a display settings structure to standard out.
* @param settings Settings to print.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_display_settings(scriptel_hid_feature_display_settings* settings);
/**
* This function gets the active device configuration from a ScripTouch mouse device.
* @param device Device to query.
* @return Device configuration.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_device_configure) scriptel_get_device_configure(scriptel_device* device);
/**
* This function gets the active device configuration from a ScripTouch mouse device.
* @param device Device to query.
* @param report reference to the report
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_device_configure_ref(scriptel_device* device, scriptel_hid_feature_device_configure* report);
/**
* This function prints the device configuration to standard out.
* @param config Configuration to print.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_device_configure(scriptel_hid_feature_device_configure* config);
/**
* This function gets the maximum number of multi-touch contacts supported from a
* ScripTouch multi-touch device.
* @param device Device to query.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_contact_count_maximum) scriptel_get_contact_count_maximum(scriptel_device* device);
/**
* This function adds a region to the specified screen.
* @param screen screen to add the region to.
* @param region the region being added.
* @param next the region to move to.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_add_region_to_screen(scriptel_hid_feature_screen *screen, int region, int next);
/**
* This function sets the device to easyscript mode if possible.
* @param device device to set to easyscript mode.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_easyscript_mode(scriptel_device* device, char commit_changes);
/**
* This function sets the device to proscript mode if possible.
* @param device device to set to proscript mode.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_proscript_mode(scriptel_device* device, char commit_changes);
/**
* This function gets the maximum number of multi-touch contacts supported from a
* ScripTouch multi-touch device.
* @param device Device to query.
* @param report reference to the report
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_contact_count_maximum_ref(scriptel_device* device, scriptel_hid_feature_contact_count_maximum* report);
/**
* This function prints the contact count maximum information to standard out.
* @param contact Contact information to print.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_contact_count_maximum(scriptel_hid_feature_contact_count_maximum* contact);
/**
* This function gets the current frequency and amplitude settings being used for
* the digitizer.
* @param device Device to query
* @return Frequency and amplitude information
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_frequency_amplitude) scriptel_get_frequency_amplitude(scriptel_device* device);
/**
* This function gets the current frequency and amplitude settings being used for
* the digitizer.
* @param device Device to query
* @param fa reference to the frequency_amplitude structure
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_frequency_amplitude_ref(scriptel_device* device, scriptel_hid_feature_frequency_amplitude* fa);
/**
* This function prints the a frequency and amplitude settings structure to standard out.
* @param freq Structure to print.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_frequency_amplitude(scriptel_hid_feature_frequency_amplitude* freq);
/**
* This function gets the current power state of the device.
* @param device Device to query.
* @return Current power state of the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_power_state) scriptel_get_power_state(scriptel_device* device);
/**
* This function prints a power state structure to standard out.
* @param state State to print.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_power_state(scriptel_hid_feature_power_state* state);
/**
* This function gets the current version of the device.
* @param device Device to query.
* @return Version information from the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_version) scriptel_get_version(scriptel_device* device);
/**
* This function gets the current version of the device.
* @param device Device to query.
* @param report Version information from the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_version_ref(scriptel_device* device, scriptel_hid_feature_version* report);
/**
* This function prints a version information structure to standard out.
* @param version Structure to print.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_version(scriptel_hid_feature_version* version);
/**
* This function gets the logical coordinate range that will be returned from
* the device when points are delivered. The logical coordinate space may not
* be the same aspect as the physical space. You can use the display information
* to get the physical aspect ratio of the device (see scriptel_hid_feature_display_info).
* @param device Device to query.
* @return Logical coordinate range of the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_coordinate_range) scriptel_get_coordinate_range(scriptel_device* device);
/**
* This function gets the logical coordinate range that will be returned from
* the device when points are delivered. The logical coordinate space may not
* be the same aspect as the physical space. You can use the display information
* to get the physical aspect ratio of the device (see scriptel_hid_feature_display_info).
* @param device Device to query.
* @param report reference to the report
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_coordinate_range_ref(scriptel_device* device, scriptel_hid_feature_coordinate_range* report);
/**
* This function prints the logical coordinate range from the device to standard out.
* @param range Structure to print.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_coordinate_range(scriptel_hid_feature_coordinate_range* range);
/**
* This function gets the error correction table from the device. This table
* is used to correct for natural imperfections in the digitizer's sensor.
* @param device Device to query.
* @return Error correction table.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_error_correction_table) scriptel_get_error_correction_table(scriptel_device* device);
/**
* This function gets the error correction table from the device. This table
* is used to correct for natural imperfections in the digitizer's sensor.
* @param device Device to query.
* @param table Error correction table reference to store the data.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_error_correction_table_ref(scriptel_device* device, scriptel_hid_feature_error_correction_table* table);
/**
* This function prints the error correction table to standard out.
* @param table Table to print.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_error_correction_table(scriptel_hid_feature_error_correction_table* table);
/**
* This function gets the serial number of the device (if set).
* @param device Device to query.
* @return Serial number of the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_serial_number) scriptel_get_serial_number(scriptel_device* device);
/**
* This function gets the serial number of the device (if set).
* @param device Device to query.
* @param sn reference to the serial number report to store the data
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_serial_number_ref(scriptel_device* device, scriptel_hid_feature_serial_number* sn);
/**
* This function prints a serial number structure to standard out.
* @param serial Structure to print.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_serial_number(scriptel_hid_feature_serial_number* serial);
/**
* This function gets supported operating modes from the device.
* @param device Device to query.
* @return Supported operating modes.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_operating_modes) scriptel_get_operating_modes(scriptel_device* device);
/**
* This function gets supported operating modes from the device.
* @param device Device to query.
* @param modes reference to operating modes structure
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_operating_modes_ref(scriptel_device* device, scriptel_hid_feature_operating_modes* modes);
/**
* This function prints a supported operating modes structure to standard out.
* @param modes Modes structure to print.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_operating_modes(scriptel_hid_feature_operating_modes* modes);
/**
* This function gets general operating parameters from a device.
* @param device Device to query.
* @return General parameters from the device.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_general_parameters) scriptel_get_general_parameters(scriptel_device* device);
/**
* This function gets general operating parameters from a device.
* @param device Device to query.
* @param gparams reference to the general parameters structure
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_general_parameters_ref(scriptel_device* device, scriptel_hid_feature_general_parameters* gparams);
/**
* This function prints a general operating parameters structure to standard out
* @param params Parameters to print.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_general_parameters(scriptel_hid_feature_general_parameters* params);
/**
* This function can be used to get the ASIC register from a device.
* @param device Device to query.
* @return ASIC register.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_register_get) scriptel_get_asic_register(scriptel_device* device);
/**
* This function can be used to get the ASIC register from a device.
* @param device Device to query.
* @param reg reference to the get_register feature report
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_asic_register_ref(scriptel_device* device, scriptel_hid_feature_register_get* reg);
/**
* This function prints an ASIC register structure to standard out.
* @param reg Register to print.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_asic_register(scriptel_hid_feature_register_get* reg);
/**
* This function gets operating parameters for pen input from the
* device.
* @param device Device to query.
* @return Pen parameters.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_pen_parameters) scriptel_get_pen_parameters(scriptel_device* device);
/**
* This function gets operating parameters for pen input from the
* device.
* @param device Device to query.
* @param report reference to the report
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_pen_parameters_ref(scriptel_device* device, scriptel_hid_feature_pen_parameters* report);
/**
* This function prints a pen parameters structure to standard out.
* @param pen Structure to print.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_pen_parameters(scriptel_hid_feature_pen_parameters* pen);
/**
* This function gets operating parameters for touch input from the device.
* @param device Device to query.
* @return Touch parameters
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_touch_parameters) scriptel_get_touch_parameters(scriptel_device* device);
/**
* This function gets operating parameters for touch input from the device.
* @param device Device to query.
* @param report reference to the report
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_touch_parameters_ref(scriptel_device* device, scriptel_hid_feature_touch_parameters* report);
/**
* This function prints a touch parameters structure to standard out.
* @param touch Touch parameters to print.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_touch_parameters(scriptel_hid_feature_touch_parameters* touch);
/**
* This function gets the current list of regions from the device as an array. The number of regions
* can be retrieved by calling scriptel_get_display_info and looking at the region_count member.
* Unlike all of the other get functions this method uses dynamically allocated memory (since each
* device can have a different number of regions). Be sure to call scriptel_free_regions() on the
* array when you're done.
*
* This function is deprecated for firmware greater than or equal to version 4.0.0.
* Do not use this function for new development.
*
* @param device Device to query.
* @return Dynamically allocated array of regions from the device.
* @deprecated This function is deprecated in favor of scriptel_get_device_regions().
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_device_region_info*) scriptel_get_regions(scriptel_device* device);
/**
* This function gets the default list of regions from the device as an array. Default regions are
* the regions that will be restored on a power cycle or reboot. The number of regions
* can be retrieved by calling scriptel_get_display_info and looking at the region_count member.
* Unlike all of the other get functions this method uses dynamically allocated memory (since each
* device can have a different number of regions). Be sure to call scriptel_free_regions() on the
* array when you're done. This method uses report 110 to obtain Regions.
*
* This function is deprecated for firmware greater than or equal to version 4.0.0.
* Do not use this function for new development.
*
* @param device Device to query.
* @return Dynamically allocated array of regions from the device.
* @deprecated This function is deprecated as devices with firmware greater than 4.0.0 will no longer have default regions.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_device_region_info*) scriptel_get_default_regions(scriptel_device* device);
/**
* This function frees a region array returned from scriptel_get_regions() or scriptel_get_default_regions().
* @param regions Regions array to free.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_free_regions(scriptel_device_region_info* regions);
/**
* This function prints a set (1 or more) regions to standard out.
* @param regions Regions array to print.
* @param count Number of elements in the array.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_regions(scriptel_device_region_info* regions, unsigned int count);
/**
* This function gets the current list of regions from the device as an array. The number of regions
* can be retrieved by calling scriptel_get_display_info and looking at the region_count member.
* Unlike all of the other get functions this method uses dynamically allocated memory (since each
* device can have a different number of regions). Be sure to call scriptel_free_regions() on the
* array when you're done. This method uses report 152 to obtain Regions.
* @param device Device to query.
* @return Dynamically allocated array of regions from the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_region*) scriptel_get_device_regions(scriptel_device* device);
/**
* This function frees a region array returned from scriptel_get_device_regions().
* @param regions Regions array to free.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_free_device_regions(scriptel_hid_feature_region* regions);
/**
* This function prints a set (1 or more) regions to standard out.
* @param regions Regions array to print.
* @param count Number of elements in the array.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_device_regions(scriptel_hid_feature_region* regions, unsigned int count);
/**
* This function returns the current calibration state of a device.
* @param device Device to query.
* @return Current calibration state of the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_calibrate) scriptel_get_calibrate_state(scriptel_device* device);
/**
* This function returns the current calibration state of a device.
* @param device Device to query.
* @param report reference to the report
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_calibrate_state_ref(scriptel_device* device, scriptel_hid_feature_calibrate* report);
/**
* This function prints a calibration state structure to standard out.
* @param state Calibration state structure to print.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_calibrate_state(scriptel_hid_feature_calibrate* state);
/**
* This function returns the UUID of a device as a byte array. It is the
* responsibility of the caller to free the character buffer returned.
* @param device Device to query.
* @return Byte array containing the UUID of the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(unsigned char*) scriptel_get_device_uuid(scriptel_device* device);
/**
* This function frees the UUID of a device.
* @param uuid variable to free.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_free_device_uuid(unsigned char* uuid);
/**
* This function prints a UUID byte array to standard out.
* @param uuid UUID to display.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_print_device_uuid(unsigned char* uuid);
/**
* This function sets the run mode of the device.
* @param device Device to set the run mode on.
* @param run_mode The run mode to set on the device.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_run_mode(scriptel_device* device, scriptel_device_run_mode run_mode);
/**
* This function sets the device configuration on ScripTouch mouse devices.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to set the configuration on.
* @param configure Configuration to set.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_device_configure(scriptel_device* device, scriptel_hid_feature_device_configure configure);
/**
* This function sets the frequency and amplitude settings for a device.
* @param device Device to set the frequency and amplitude settings on.
* @param freq Frequency and amplitude settings to set.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_frequency_amplitude(scriptel_device* device, scriptel_hid_feature_frequency_amplitude freq);
/**
* This function sets the power state of the device.
* @param device Device to set the power state on.
* @param state Power state to set.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_power_state(scriptel_device* device, scriptel_hid_feature_power_state state);
/**
* This function sets a firmware page on the device. This function should not be
* used by third party applications as improper use of this function can render
* a device inoperable.
* @param device Device to set the firmware page on.
* @param firmware Page to set.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_firmware(scriptel_device* device, scriptel_hid_feature_firmware_download firmware);
/**
* This function is used to indicate to a device that the firmware download is complete.
* This function should not be used by third party applications as improper use of this
* function can render a device inoperable.
* @param device Device to notify.
* @param status Download status to set.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_firmware_download_status(scriptel_device* device, scriptel_hid_feature_download_command_status status);
/**
* This function can be used to set the sensor error correction table on the device.
* This function should not be used by third party applications as improper use of this
* function can render a device inoperable.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to update.
* @param errcor Error correction table to write.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_error_correction(scriptel_device* device, scriptel_hid_feature_error_correction_table errcor);
/**
* This function can be used to set the serial number on a device. This function should
* not be used by third party applications as it could interfere with warranty claims.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to update.
* @param serial_number Serial number to write.
* @param len Length of the serial number.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_serial_number(scriptel_device* device, char* serial_number, unsigned int len);
/**
* This function can be used to set the default start up mode of a device.
* @param device Device to update.
* @param mode Default mode to start in.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_startup_mode(scriptel_device* device, scriptel_device_acquisition_mode mode);
/**
* This function can be used to update general operating parameters.
* This function should not be used by third party applications as it could
* negatively impact the operating of the digitizer.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to update.
* @param params Parameters to set.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_general_parameters(scriptel_device* device, scriptel_hid_feature_general_parameters params);
/**
* This function resets the internal digitizer ASIC. This should not be
* used by third party applications.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to reset.
* @param reset Reset parameters.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_asic_reset(scriptel_device* device, scriptel_hid_feature_asic_reset reset);
/**
* This function sets a register in the digitizer ASIC. This should not
* be used by third party applications.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to set register on.
* @param reg Register to set.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_asic_register(scriptel_device* device, scriptel_hid_feature_register_set reg);
/**
* This function sets the operating parameters for pen input.
* This function should not be used by third party applications.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to set parameters for.
* @param params Parameters to set.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_pen_parameters(scriptel_device* device, scriptel_hid_feature_pen_parameters params);
/**
* This function sets the operating parameters for touch input.
* This function should not be used by third party applications.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to set parameters for.
* @param params Parameters to set.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_touch_parameters(scriptel_device* device, scriptel_hid_feature_touch_parameters params);
/**
* This function sets display content.
* @param device Device to update.
* @param content Content to update.
* @return Error code indicating whether or not the call was successful.
* @internal
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_display_content(scriptel_device* device, scriptel_hid_feature_display_content content);
/**
* This function pushes new values for the dynamic display settings to a device.
* @param device Device to update.
* @param settings New values for settings.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_display_settings(scriptel_device* device, scriptel_hid_feature_display_settings settings);
/**
* This function pushes a raster "chunk" to the display of the target device.
* @param device Device to receive raster.
* @param chunk Chunk to push.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_display_chunk(scriptel_device* device, scriptel_hid_feature_display_chunk chunk);
/**
* This function overwrites a region in the device replacing it in memory.
* Unless scriptel_set_region_commit is used any updates made by this
* function will be lost after a power cycle or device reset.
*
* This function is deprecated for firmware greater than or equal to version 4.0.0.
* Do not use this function for new development.
*
* @param device Device to update.
* @param region Region to overwrite.
* @return Error code indicating whether or not the call was successful.
* @deprecated This function is deprecated in favor of scriptel_set_region_contents().
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_region(scriptel_device* device, scriptel_device_region_info* region);
/**
* This function clears the screen of the device.
* @param device Device to clear.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_clear_screen(scriptel_device* device);
/**
* This function can be used to update the settings for a single region. This should not
* be confused with scriptel_set_region which overwrites a region completely (not just
* settings).
*
* This function is deprecated for firmware greater than or equal to version 4.0.0.
* Do not use this function for new development.
*
* @param device Device to update.
* @param region Region settings to update with.
* @return Error code indicating whether or not the call was successful.
* @deprecated This function is deprecated as it isn't much smaller than calling scriptel_set_region_contents() and doesn't do as much.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_update_region(scriptel_device* device, scriptel_hid_feature_region_update* region);
/**
* This function can be used to either commit the current set of regions to the device (making them the new defaults) or
* it can be used to discard the current set of regions.
*
* This function is deprecated for firmware greater than or equal to version 4.0.0.
* Do not use this function for new development.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* @param device Device to update.
* @param commit Commit settings.
* @return Error code indicating whether or not the call was successful.
* @deprecated This function is deprecated in favor of scriptel_set_screen_region_commit().
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_region_commit(scriptel_device* device, scriptel_region_save_changes commit);
/**
* This function "deletes" a region, effectively removing it from the screen.
*
* This function is deprecated for firmware greater than or equal to version 4.0.0.
* Do not use this function for new development.
*
* @param device Device to update.
* @param region Region ID to remove (1-indexed).
* @return Error code indicating whether or not the call was successful.
* @deprecated This function is deprecated in favor of scriptel_delete_device_region().
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_delete_region(scriptel_device* device, unsigned char region);
/**
* This function "deletes" a resource which will flag it for garbage collection.
* 
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to update
* @param resource_index the resource to be deleted.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_delete_resource(scriptel_device* device, unsigned char resource_index);
	/**
* This function "deletes" a region, effectively removing it from the screen.
* @param device Device to update.
* @param region Region ID to remove (1-indexed).
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_delete_device_region(scriptel_device* device, unsigned char region);
/**
* This function forces the device into calibration mode.
* @param device Device to calibrate
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_device_calibrate(scriptel_device* device);
/**
* This function forces the device out of calibration mode.
* @param device Device to stop calibrating.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_device_cancel_calibrate(scriptel_device* device);
/**
* This function draws text on the display of the device (if equipped)
* using a built in font. This function assumes UTF-8 encoded text.
* @param device Device to draw the text upon.
* @param text Text to draw on the screen.
* @param x The horizontal coordinate where the text should be anchored.
* @param y The vertical coordinate where the text should be anchored.
* @param alignment The position of the text anchor.
* @param invert Whether or not to invert the text.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_device_draw_text(scriptel_device* device, char* text, unsigned int x, unsigned int y, scriptel_font_alignment alignment, unsigned char invert);
/**
* This function draws text on the display of the device (if equipped)
* using a built in font. This function allows you to specify the character
* encoding.
* @param device Device to draw the text upon.
* @param data Encoded text to draw on the screen in some encoding, must be null terminated.
* @param dataLen The length of the data buffer.
* @param x The horizontal coordinate where the text should be anchored.
* @param y The vertical coordinate where the text should be anchored.
* @param alignment The position of the text anchor.
* @param invert Whether or not to invert the text.
* @param encoding The string format that text is in. 
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_device_draw_text_encoded(scriptel_device* device, char* data, unsigned int dataLen, unsigned int x, unsigned int y, scriptel_font_alignment alignment, unsigned char invert, const char* encoding);
/**
* This function measures the width of a particular string using the built
* in font.
* @param text Text to measure horizontally.
* @return The size of the string horizontally.
*/
SCRIPTEL_SYMBOL_LINKAGE(unsigned int) scriptel_measure_text_width(scriptel_device* device, char* text);
/**
* This function measures the height of a particular string using the built
* in font.
* @param device Device that is connected
* @param text Text to measure vertically.
* @return The size of the string vertically.
*/
SCRIPTEL_SYMBOL_LINKAGE(unsigned int) scriptel_measure_text_height(scriptel_device* device, char* text);
/**
* This function measures the width of a particular string using the built
* in font.
* @param device Device that is connected
* @param data Text to measure.
* @param dataLen Byte length of the text to measure.
* @param encoding Encoding to use to decode data.
* @return The size of the string horizontally.
*/
SCRIPTEL_SYMBOL_LINKAGE(unsigned int) scriptel_measure_text_width_encoding(scriptel_device* device, char* data, unsigned int dataLen, char* encoding);
/**
* This function measures the height of a particular string using the built
* in font.
* @param device Device that is connected
* @param data Text to measure.
* @param dataLen Byte length of the text to measure.
* @param encoding Encoding to use to decode data.
*/
SCRIPTEL_SYMBOL_LINKAGE(unsigned int) scriptel_measure_text_height_encoding(scriptel_device* device, char* data, unsigned int dataLen, char* encoding);
/**
* This function will attempt to restore the default region configuration to the
* device.
* @param device Device that is connected
* @param device Device to restore default regions on.
* @return Whether or not the operation completed successfully.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_restore_default_regions(scriptel_device* device);
/**
* This function will return a string containing the version number.
* @return Version number.
*/
SCRIPTEL_SYMBOL_LINKAGE(char*) scriptel_get_proscript_version(void);
/**
* This function will return the major version of the ProScript library.
* @return Major version of the ProScript library.
*/
SCRIPTEL_SYMBOL_LINKAGE(unsigned int) scriptel_get_proscript_version_major(void);
/**
* This function will return the minor version of the ProScript library.
* @return Minor version of the ProScript library.
*/
SCRIPTEL_SYMBOL_LINKAGE(unsigned int) scriptel_get_proscript_version_minor(void);
/**
* This function will return the release version of the ProScript library.
* @return Release version of the ProScript library.
*/
SCRIPTEL_SYMBOL_LINKAGE(unsigned int) scriptel_get_proscript_version_release(void);
/**
* This function will return date that the ProScript library was built in format: yyyy-MM-dd HH:mm:ssZ
* @return Date the ProScript library was built.
*/
SCRIPTEL_SYMBOL_LINKAGE(char*) scriptel_get_proscript_build_date(void);
/**
* This function returns the index of a region on the signature pad.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to view.
* @return index of the region
*/
SCRIPTEL_SYMBOL_LINKAGE(int) scriptel_get_region_idx(scriptel_device* device);
/**
* This function sets the region index for use with scriptel_get_region.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to set the region index on.
* @param region_index The index of the region.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_region_idx(scriptel_device* device, unsigned char region_index);
/**
* This function retrives the information about a region on the device based off the region index.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device the region is on
* @return details about a region on the signature pad
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_region) scriptel_get_region(scriptel_device* device);
/**
* This function retrives the information about a region on the device based off the region index.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device the region is on
* @param region hosts the details about a region on the signature pad
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_region_ref(scriptel_device* device, scriptel_hid_feature_region* region);
/**
* This function sets the information about a region on the device.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device the region is on
* @param region hosts the details about a region on the signature pad
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_region_contents(scriptel_device* device, scriptel_hid_feature_region* region);
/**
* This function retrieves the available space on the device for resources.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to view.
* @return Resource availability for the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_resource_availability) scriptel_get_resource_availability(scriptel_device* device);
/**
* This function retrieves the available space on the device for resources.
* @param device Device to view.
*
* This function requires firmware 4.0.0 or greater.
*
* @param info hosts the details about resource availability on the device.
* @return Resource availability for the device.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_resource_availability_ref(scriptel_device* device, scriptel_hid_feature_resource_availability* info);
/**
* This function returns the index of a resource on the signature pad based off the resource index.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to view.
* @return index of the resource
*/
SCRIPTEL_SYMBOL_LINKAGE(int) scriptel_get_resource_idx(scriptel_device* device);
/**
* This function sets the screen index for use with scriptel_get_resource and scriptel_get_resource_info.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to set the resource index on.
* @param resource_index The resource of the screen.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_resource_idx(scriptel_device* device, unsigned char resource_index);
/**
* This function gets the information of a resource on the device based off the resource index.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to view.
* @return Resource information about the resource at the index.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_resource_info) scriptel_get_resource_info(scriptel_device* device);
/**
* This function gets the information of a resource on the device based off the resource index.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to view.
* @param info Resource information about the resource at the index.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_resource_info_ref(scriptel_device* device, scriptel_hid_feature_resource_info* info);
/**
* This function gets the information of a resource on the device based off the resource index.
* If resource size is greater than 1024, this will return a portion of the resource. Continue calling for the rest.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to view.
* @return Resource at the index.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_resource) scriptel_get_resource(scriptel_device* device);
/**
* This function gets the information of a resource on the device based off the resource index.
* If resource size is greater than 1024, this will return a portion of the resource. Continue calling for the rest.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to view.
* @param resource Resource at the index.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_resource_ref(scriptel_device* device, scriptel_hid_feature_resource* resource);
/**
* This function sets the information of a resource on the device.
* If resource size is greater than 1024, this will set a portion of the resource. Continue calling until all data is set.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to view.
* @param resource Resource or portion of resource to set.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_resource(scriptel_device* device, scriptel_hid_feature_resource* resource);
/**
* This function can be used to either commit the current set of screens and regions to the device(making them the new defaults) or
* it can be used to discard the current set of screens and regions.
*
* This function requires that the firmware be placed in command mode prior to use.
* Please see scriptel_set_run_mode() for details.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to update.
* @param commit Commit settings.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_screen_region_commit(scriptel_device* device, scriptel_screen_region_save_changes commit);
/**
* This function retrives the information about a screen that can be displayed on the device.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device the screen is a part of
* @return screen variable that holds all the information and number of regions in use.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_screen) scriptel_get_screen(scriptel_device* device);
/**
* This function retrives the information about a screen that can be displayed on the device based off the screen index.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device the screen is a part of
* @param screen Screen variable that holds all the information and number of regions in use.
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_screen_ref(scriptel_device* device, scriptel_hid_feature_screen* screen);
/**
* This function sets the information about a screen that can be displayed on the device based off the screen index.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device the screen is a part of
* @param screen Screen variable that holds all the information and number of regions in use.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_screen(scriptel_device* device, scriptel_hid_feature_screen* screen);
/**
* This function returns the index of a screen on the signature pad
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to view.
* @return index of the screen
*/
SCRIPTEL_SYMBOL_LINKAGE(int) scriptel_get_screen_idx(scriptel_device* device);
/**
* This function sets the screen index for use with scriptel_get_screen.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to set the screen index on.
* @param screen_index The index of the screen.
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_screen_idx(scriptel_device* device, unsigned char screen_index);
/**
* This function deletes a screen that is set on the device.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to delete the screen of
* @param screen_index the index of the screen that is to be deleted
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_delete_screen(scriptel_device* device, unsigned char screen_index);
/**
* This function edits a screen that is set on the device.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device to edit the screen of
* @param screen_edit this variable holds all the information about the edited screen, including the index
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_edit_screen(scriptel_device* device, scriptel_hid_feature_edit_screen* screen_edit);
/**
* This function recieves the overall information about the number of screens and regions and other information.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device that is connected
* @return information about screens and regions on the device
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_hid_feature_screen_region_info) scriptel_get_screen_region_info(scriptel_device* device);
/**
* This function recieves the overall information about the number of screens and regions and other information.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device that is connected
* @param info information about screens and regions on the device
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_get_screen_region_info_ref(scriptel_device* device, scriptel_hid_feature_screen_region_info* info);
/**
* This function returns the index of the active screen on the signature pad.
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device that is connected
* @return index of the active screen, -1 if error.
*/
SCRIPTEL_SYMBOL_LINKAGE(int) scriptel_get_active_screen(scriptel_device* device);
/**
* This function sets the index of the active screen on the signature pad
*
* This function requires firmware 4.0.0 or greater.
*
* @param device Device that is connected
* @param screen_index index of the target active screen
* @return Error code indicating whether or not the call was successful.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_set_active_screen(scriptel_device* device, unsigned char screen_index);
/**
* This function is used for unit testing older firmware which does not check the device status locking flags before setting 
* regions, modes, display, or before doing calibration.
* @param legacy set to true (non-zero) for legacy (no wait) behavior
*/
SCRIPTEL_SYMBOL_LINKAGE(void) scriptel_set_legacy_firmware(int version, char legacy);

/**
* This function is used for unit testing. It simulates putting a pen on the screen (or lifting the pen off).
* @param dev Device that is connected
* @param coord This variable holds all of the info for setting a pen coordinate.
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_simulate_pen_input(scriptel_device *dev, scriptel_pen_coordinate *coord);
/**
* This function reads the pixels on the screen for testing purposes. Given a starting position (x,y) the function reads 256 pixels, wrapping
* When it reaches the end of a raster. The number of pixels read may be less than 256 if the end of the screen is found first.
* @param device Device that is connected
* @param pixel_values is a pointer to a structure that gets filled in by the function that includes the pixel data and metadata
* @param x the x coordinate in terms of screen pixels to start reading from
* @param y the y coordinate in terms of screen rasters to start reading from
* @return SCRIPTEL_CODE_SUCCESS or SCRIPTEL_CODE_ERROR
*/
SCRIPTEL_SYMBOL_LINKAGE(scriptel_error_code) scriptel_get_pixels(scriptel_device* device, scriptel_hid_feature_pixel_values *pixel_values);

extern scriptel_device_driver scriptel_real_driver;
#endif