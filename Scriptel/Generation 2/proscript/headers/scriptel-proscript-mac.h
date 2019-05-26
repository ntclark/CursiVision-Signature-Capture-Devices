/*******************************************************************************
 * scriptel-proscript-mac.h - Copyright 2014 - Scriptel Corporation
 * ----
 * This header contains Mac OSX specific information required to interact with
 * devices on that platform.
 ******************************************************************************/
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/IOKitLib.h>

#ifdef SCRIPTEL_LIBRARY_BUILD
    #include "scriptel-proscript-mac-private.h"
#endif

/**
 * Symbol used to export public symbols.
 */
#define SCRIPTEL_SYMBOL_EXPORT extern

/**
 * Macro is used by Windows and needs to be ignored by Mac
 */
#define SCRIPTEL_SYMBOL_LINKAGE(return_type) extern return_type

/**
 * Maximum size of any input report.
 */
#define SCRIPTEL_MAC_MAX_INPUT_REPORT 512

/**
 * Mac OSX specific information that needs to be carried around
 * with the device handle.
 */
typedef struct {
	/**
	 * HID Device Handle
	 */
    IOHIDDeviceRef handle;
	/** 
	 * List of outstanding input events.
	 */
    void* input_events;
	/**
	 * Reading event loop reference.
	 */
    CFStringRef loop_mode;
	/**
	 * Whether or not the thread that is going to be reading
	 * has registered with the local event loop.
	 */
    unsigned char read_registered;
	/**
	 * Whether or not the device was forcefully unmounted.
	 * OSX cleans up removed devices automatically invalidating
	 * handles. If this is true the handle is completely invalid.
	 */
    unsigned char hard_unplug;
	/**
	 * Whether or not the event loop has been registered.
	 */
    unsigned char event_loop_registered;
	/**
	 * Baton for the read buffer.
	 */
    unsigned char read_buffer[SCRIPTEL_MAC_MAX_INPUT_REPORT];
    /**
     * Pointer to the read until done thread.
     */
    pthread_t readUntilDoneThread;
    unsigned char hid_report_map[255];
} scriptel_device_os_params;