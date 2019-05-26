/* ************************************************************************** */
/*! 
	\file Report.h
    
	\brief Report IDs definition.

	Copyright (c) 2007 WACOM Co.,Ltd. All rights reserved.
*/
/* ************************************************************************** */

#pragma once

/*!
	\brief Report data struct
*/
typedef struct {
	int id;
	union {
		struct {
			DWORD sessionId;
			BOOL rdy;
			int x;
			int y;
			int press;
			int sw;
		} pen;
		struct {
			int max_x;
			int max_y;
			int max_press;
			int lcd_width;
			int lcd_height;
		} caps;
		struct {
			TCHAR name[8];
			int major_version;
			int minor_version;
		} info;
		struct {
			unsigned char status_code;
			unsigned char last_error;
			unsigned __int16 status_word;
		} status;
	};
} REPORT;
