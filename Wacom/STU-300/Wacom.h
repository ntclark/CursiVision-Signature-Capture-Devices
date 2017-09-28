#ifndef __SIGPADDOTNET_H__
#define __SIGPADDOTNET_H__
#include "Report.h"

typedef	struct {
	      int got_capabilities;
			int max_x;
			int max_y;
			int max_press;
			int lcd_width;
			int lcd_height;
		} CAPS;

typedef	struct {
			int x;
			int y;
			int press;
			int sw;
			int time;
		} PEN;

typedef struct {
	int width;
	int colour;
} PENPROPS;


extern "C" bool __declspec(dllimport) isConnected();
extern "C" typedef bool (CALLBACK *funcPtr)(PEN,void*);
extern "C" int __declspec(dllimport) onCDllReceive(REPORT *rep);
extern "C" void __declspec(dllimport) setImage(char *path);
extern "C" bool __declspec(dllimport) Connect(HWND hWnd, int nPort);
extern "C" bool __declspec(dllimport) ConnectByUid(HWND hWnd, int nPort, int device_id, unsigned long device_uid);
extern "C" void __declspec(dllimport) BeginCapture();
extern "C" void __declspec(dllimport) EndCapture();
extern "C" unsigned long __declspec(dllimport) GetUID();
extern "C" void __declspec(dllimport) ClearScreen();
extern "C" int __declspec(dllimport) onCDllCallback_ext(funcPtr cb,void* pUser); 
extern "C" int __declspec(dllimport) Disconnect();
extern "C" CAPS __declspec(dllimport) GetCapabilities();
extern "C" bool __declspec(dllimport) setInkingMode(int mode);
extern "C" void __declspec(dllimport) SetUID(unsigned char *uid);
extern "C" bool __declspec(dllimport) enableTimecount();
extern "C" bool __declspec(dllimport) disableTimecount();
extern "C" bool __declspec(dllimport) setHandwritingArea(unsigned int leftx, unsigned int lefty, unsigned int rightx, unsigned int righty);
extern "C" int __declspec(dllimport) setnoZImage(SIZE_T length, BYTE *data);
extern "C" unsigned int __declspec(dllimport) GetBacklightBrightness();
extern "C" bool __declspec(dllimport) SetBacklightBrightness(unsigned int brightness);
extern "C" PENPROPS __declspec(dllimport) GetPenProperties();
extern "C" bool __declspec(dllimport) SetPenProperties(unsigned int width, unsigned int colour);
extern "C" bool __declspec(dllimport) SetClearScreenColour(unsigned int colour);
extern "C" unsigned int __declspec(dllimport) GetClearScreenColour();


#endif//__SIGPADDOTNET_H__