/*******************************************************************************
 * scriptel-proscript-linux.h - Copyright 2014 - Scriptel Corporation
 * ----
 * This header contains the definitions for Linux specific functionality.
 ******************************************************************************/

#ifdef SCRIPTEL_LIBRARY_BUILD
    #include "scriptel-proscript-linux-private.h"
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
 * @brief This structure contains Linux-specific device information.
 */
typedef struct {
	int handle;
	unsigned char interrupted;
	pthread_t readUntilDoneThread;
    unsigned char hid_report_map[255];
} scriptel_device_os_params;
