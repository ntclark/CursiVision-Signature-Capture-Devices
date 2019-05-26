/*******************************************************************************
 * scriptel-proscript-windows.h - Copyright 2014 - Scriptel Corporation
 * ----
 * This header contains Windows specific information required to interact with
 * devices on that platform.
 ******************************************************************************/

#ifndef _SCRIPTEL_PROSCRIPT_WINDOWS_H_
#define _SCRIPTEL_PROSCRIPT_WINDOWS_H_

#include <Windows.h>

/**
 * This definition is used to export symbols in DLL's
 */
#define SCRIPTEL_SYMBOL_EXPORT __declspec(dllexport)
#ifdef SCRIPTEL_EXPORT_DLL
	/**
	 * When producing the library export symbols.
	 */
	#define SCRIPTEL_SYMBOL_LINKAGE(return_type) __declspec(dllexport) return_type __cdecl
#elif SCRIPTEL_EXPORT_VB_DLL
	#define SCRIPTEL_SYMBOL_LINKAGE(return_type) return_type __stdcall
#elif SCRIPTEL_EXPORT_STATIC
	#define SCRIPTEL_SYMBOL_LINKAGE(return_type) return_type
#else
	#ifdef SCRIPTEL_IMPORT_STATIC
		#define SCRIPTEL_SYMBOL_LINKAGE(return_type) return_type
	#else
		#define SCRIPTEL_SYMBOL_LINKAGE(return_type) __declspec(dllimport) return_type __cdecl
	#endif
#endif

/**
 * This structure defines Windows specific information required to be
 * carried around with the device structure.
 */
typedef struct {
	/**
	 * Handle to the device.
	 */
	HANDLE handle;
	/**
	 * An event handle that indiciates that the device has been closed.
	 */
	HANDLE closeHandle;
	/**
	 * An event handle that indiciates there is data to be read.
	 */
	HANDLE readHandle;
	/**
	 * A thread handle that points to the read until done thread. thread.
	 */
	HANDLE readUntilDoneThread;
	/**
	 * The length of an output report going to this device. No longer populated.
	 */
	unsigned int output_report_length;
	/**
	 * The length of an input report coming from this device. No longer populated.
	 */
	unsigned int input_report_length;
	/**
	 * Thread ID of the read until done thread.
	 */
	DWORD readUntilDoneThreadId;
} scriptel_device_os_params;

#endif