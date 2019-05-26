// Copyright 2000-2018, signotec GmbH, Ratingen, Germany, All Rights Reserved
// signotec GmbH
// Am Gierath 20b
// 40885 Ratingen
// Tel: +49 (2102) 5 35 75-10
// Fax: +49 (2102) 5 35 75-39
// E-Mail: <info@signotec.de>
//
//	Header file for STPadLib.dll and libSTPadLib.so.
//
//	Date:		26.11.2018
//	Version:	8.4.2.0

#ifndef _STPadLib_
#define _STPadLib_

#ifdef __linux__
typedef void* 				LPVOID;
typedef void 				VOID;
typedef int 				LONG;
typedef double				DOUBLE;
typedef const wchar_t* 		LPCWSTR;
typedef wchar_t 			WCHAR;
typedef bool				BOOL;
typedef unsigned int  		DWORD;
typedef DWORD 				COLORREF;
typedef unsigned char 		BYTE;

#ifndef TRUE
#define TRUE true
#endif

#define PAD_API __attribute__ ((__visibility__("default")))
#define __declspec(dllexport) extern  "C" __attribute__ ((__visibility__("default")))
#define __stdcall
#endif // __linux__

#ifdef STPADLIB_EXPORTS
#define STPADLIB_API __declspec(dllexport)
#else
#define STPADLIB_API __declspec(dllimport)
#endif

// error codes
#define STPAD_ERROR_OK					(  0L)
#define STPAD_ERROR_PARNULL             (- 1L)
#define STPAD_ERROR_INVALIDPAR          (- 3L)
#define STPAD_ERROR_DEVICEUNAVAILABLE   ( -4L)
#define STPAD_ERROR_NOTOPEN             (- 5L)
#define STPAD_ERROR_ALREADYOPEN         (- 6L)
#define STPAD_ERROR_MAXDEVICE           (- 7L)
#define STPAD_ERROR_INVALIDDEVICEID     (- 8L)
#define STPAD_ERROR_INVALIDLED          ( -9L)
#define STPAD_ERROR_SESSIONSTARTED      (-12L)
#define STPAD_ERROR_MAXBUTTON           (-13L)
#define STPAD_ERROR_BUTTONOVERLAP       (-14L)
#define STPAD_ERROR_NOSIGNATUREWINDOW   (-15L)
#define STPAD_ERROR_NOSESSIONSTARTED    (-17L)
#define STPAD_ERROR_MEMORYALLOCATION    (-18L)
#define STPAD_ERROR_SYSTEM              (-19L)
#define STPAD_ERROR_PROTOCOL			(-20L)
#define STPAD_ERROR_TOOSMALL            (-21L)
#define STPAD_ERROR_DEVICEDETACHED      (-22L)
#define STPAD_ERROR_NOTSUPPORTED		(-25L)
#define STPAD_ERROR_FILE				(-26L)
#define STPAD_ERROR_RSASTORE            (-27L)
#define STPAD_ERROR_RSANOKEY            (-28L)
#define STPAD_ERROR_RSACALC             (-30L)
#define STPAD_ERROR_RSAINVALIDDATA      (-31L)
#define STPAD_ERROR_LOSTDATA	        (-32L)
#define STPAD_ERROR_CONTENTSIGNING		(-33L)
#define STPAD_ERROR_RSAPASSWORD         (-34L)
#define STPAD_ERROR_OPTIONPASSWORD      (-36L)
#define STPAD_ERROR_SIGNATUREWINDOW     (-38L)
#define STPAD_ERROR_PENSCROLLENABLED    (-39L)
#define STPAD_ERROR_NONFCREADER		    (-40L)
#define STPAD_ERROR_PASSWORD			(-81L)
#define STPAD_ERROR_PDFLIB				(-82L)
#define STPAD_ERROR_NODOCUMENT			(-83L)
#define STPAD_ERROR_SECUREMODE			(-84L)
#define STPAD_ERROR_NOSIGNDATA			(-85L)
#define STPAD_ERROR_DECRYPT				(-86L)
#define STPAD_ERROR_CERTDOESNTMATCH		(-87L)
#define STPAD_ERROR_CERTENCODE			(-88L)
#define STPAD_ERROR_INVALIDPASSWORD		(-89L)
#define STPAD_ERROR_NOHASH1				(-90L)
#define STPAD_ERROR_NOHASHALGO			(-91L)
#define STPAD_ERROR_CONTENTSIGNINGEXIT	(-92L)
#define STPAD_ERROR_OVERLAYSET			(-93L)
#define STPAD_ERROR_ISSCROLLED			(-94L)
#define STPAD_ERROR_CANTSCROLL			(-95L)
#define STPAD_ERROR_INIT				(-97L)

// callback typedef
typedef VOID (*CBPTR)(LONG nEvent, LPVOID pData, LONG nDataSize, LPVOID pCustomPar);

// callback event IDs
#define STPAD_CALLBACK_DISCONNECT		0
#define STPAD_CALLBACK_HOTSPOT			1
#define STPAD_CALLBACK_TIMEOUT			2
#define STPAD_CALLBACK_SCROLL			3
#define STPAD_CALLBACK_SIGNATURE		4

// device capabilities
#define STPAD_CAP_COLORDISPLAY			0x000001
#define STPAD_CAP_BACKLIGHT				0x000002
#define STPAD_CAP_VERTICALSCROLLING		0x000004
#define STPAD_CAP_HORIZONTALSCROLLING	0x000008
#define STPAD_CAP_PENSCROLLING			0x000010
#define STPAD_CAP_SERVICEMENU			0x000020
#define STPAD_CAP_RSA					0x000040
#define STPAD_CAP_CONTENTSIGNING		0x000080
#define STPAD_CAP_H2CONTENTSIGNING		0x000100
#define STPAD_CAP_GENERATESIGNKEY		0x000200
#define STPAD_CAP_STORESIGNKEY			0x000400
#define STPAD_CAP_STOREENCRYPTKEY		0x000800
#define STPAD_CAP_SIGNEXTERNALHASH		0x001000
#define STPAD_CAP_RSAPASSWORD			0x002000
#define STPAD_CAP_SECUREMODEPASSWORD	0x004000
#define STPAD_CAP_4096BITKEY			0x008000
#define STPAD_CAP_NFCREADER				0x010000

// erase flags
#define STPAD_ERASE_SIGNATURE			0x01

// signature image flags
#define STPAD_SIMG_TIMESTAMP			0x0001
#define STPAD_SIMG_BACKIMAGE			0x0002
#define STPAD_SIMG_HOTSPOTS				0x0004
#define STPAD_SIMG_NOCROPPING			0x0008
#define STPAD_SIMG_ALIGNLEFT			0x0010
#define STPAD_SIMG_ALIGNRIGHT			0x0020
#define STPAD_SIMG_ALIGNTOP				0x0040
#define STPAD_SIMG_ALIGNBOTTOM			0x0080
#define STPAD_SIMG_TIMESTAMPIMGREL		0x0100
#define STPAD_SIMG_DONTSMOOTH			0x0200
#define STPAD_SIMG_SMOOTH				0x0400
#define STPAD_SIMG_OVERLAYIMAGE			0x0800
#define STPAD_SIMG_TRANSPARENT			0x1000
#define STPAD_SIMG_CURRENTIMAGES		0x2000
#define STPAD_SIMG_VARIABLEPENWIDTH		0x4000

// display image flags
#define STPAD_DIMG_HOTSPOTS				0x01
#define STPAD_DIMG_BUFFER				0x02
#define STPAD_DIMG_CURRENTTARGET		0x04

// font style defines
#define STPAD_FONT_NORMAL				0x00
#define STPAD_FONT_BOLD					0x01
#define STPAD_FONT_UNDERLINE			0x02
#define STPAD_FONT_ITALIC				0x04

// text options
#define STPAD_TEXT_NORESIZE				0x01

// LED bitmask
#define STPAD_LED_YELLOW				0x01
#define STPAD_LED_GREEN					0x02

// NFC modes
#define STPAD_NFC_OFF					0
#define STPAD_NFC_ON					1
#define STPAD_NFC_PERMANENTLYOFF		2
#define STPAD_NFC_PERMANENTLYON			3

// signature bounds options
#define STPAD_BOUNDS_DISPLAY			0
#define STPAD_BOUNDS_SIGNRECT			1

// display targets
#define STPAD_TARGET_LARGESTORE			-2
#define STPAD_TARGET_STANDARDSTORE		-1
#define STPAD_TARGET_FOREGROUND			0
#define STPAD_TARGET_BACKGROUND			1
#define STPAD_TARGET_OVERLAY			2
#define STPAD_TARGET_DISPLAYHASH		1000

// PDF flags
#define STPAD_PDF_CACHE					0x01

// RSA signing flags
#define STPAD_RSA_LITTLEENDIAN			0x01
#define STPAD_RSA_PKCS7					0x02
#define STPAD_RSA_HASHDATA				0x02

// RSA SignData flags
#define STPAD_DECRYPTSIGNDATA_TIMESTAMP	0x01
#define STPAD_DECRYPTSIGNDATA_SERIAL	0x02
#define STPAD_DECRYPTSIGNDATA_KEYSOURCE 0x04
#define STPAD_DECRYPTSIGNDATA_FIRMWARE	0x08
#define STPAD_DECRYPTSIGNDATA_HASH1		0x10
#define STPAD_DECRYPTSIGNDATA_HASH2		0x20
#define STPAD_GETSIGNDATA_UNCOMPRESSED	0x01

// ISO data flags
#define STPAD_ISODATA_EXTENDED			0x01

// string sizes
#define VERSION_SIZE					16
#define SERIAL_SIZE						16
#define IP_ADDRESS_SIZE					32

enum ERASEOPTION
{
	kComplete = 0,
	kSignature
};

enum FILETYPE
{
	kTiff = 0,
	kPng,
	kBmp,
	kJpeg,
	kGif
};

enum ALIGN
{
    kLeft = 0,
    kCenter,
    kRight,
    kLeftCenteredVertically,
    kCenterCenteredVertically,
    kRightCenteredVertically,
    kLeftNoWrap,
    kCenterNoWrap,
    kRightNoWrap
};

enum BACKLIGHT
{
	kOff = 0,
	kOn,
	kMedium,
	kMaximum
};

enum HOTSPOTMODE
{
	kInactive = 0,
	kActive,
	kInvertOff
};

enum HOTSPOTTYPE
{
	kScrollDown = 0,
	kScrollUp,
	kScrollRight,
	kScrollLeft,
	kScrollable
};

enum HASHALGO
{
	kSha1 = 0,
	kSha256,
	kSha512
};

enum HASHVALUE
{
	kCombination = 0,
	kHash1,
	kHash2
};

enum RSASCHEME
{
	kNoHashOID = 0,
	kPKCS1_V1_5,
	kPSS
};

enum CERTTYPE
{
	kCert_DER = 0,
	kCSR_DER
};

enum MEASUREMENTUNIT
{
	kPixels = 0,
	kMillimetres,
	kInches
};

extern "C" STPADLIB_API VOID STControlSetAppName(LPCWSTR szName);
extern "C" STPADLIB_API VOID STControlSetCallback(CBPTR pCallback, LPVOID pCustomPar);
extern "C" STPADLIB_API LONG STControlSetLogDirectory(LPCWSTR szPath);
extern "C" STPADLIB_API LONG STControlGetVersion(WCHAR szVersion[VERSION_SIZE]);
extern "C" STPADLIB_API LONG STControlGetErrorString(LPCWSTR szError, LONG* pnStringLength, LONG nErrorId);
extern "C" STPADLIB_API VOID STControlExit();
extern "C" STPADLIB_API LONG STDeviceSetComPort(LPCWSTR szPortList);
extern "C" STPADLIB_API LONG STDeviceGetComPort(LONG nIndex);
extern "C" STPADLIB_API LONG STDeviceGetIPAddress(WCHAR szAddress[IP_ADDRESS_SIZE], LONG nIndex);
extern "C" STPADLIB_API LONG STDeviceGetConnectionType(LONG nIndex);
extern "C" STPADLIB_API LONG STDeviceGetCount();
extern "C" STPADLIB_API LONG STDeviceGetInfo(WCHAR szSerial[SERIAL_SIZE], LONG* pnType, LONG nIndex);
extern "C" STPADLIB_API LONG STDeviceGetVersion(WCHAR szVersion[VERSION_SIZE], LONG nIndex);
extern "C" STPADLIB_API LONG STDeviceGetCapabilities(LONG nIndex);
extern "C" STPADLIB_API LONG STDeviceOpen(LONG nIndex, BOOL bEraseDisplay=TRUE);
extern "C" STPADLIB_API LONG STDeviceClose(LONG nIndex);
extern "C" STPADLIB_API LONG STDeviceSetLed(LONG nLedColor);
extern "C" STPADLIB_API LONG STDeviceStartService(LONG nType);
extern "C" STPADLIB_API LONG STDeviceGetNFCMode(LONG nIndex);
extern "C" STPADLIB_API LONG STDeviceSetNFCMode(LONG nMode, LONG nIndex);
extern "C" STPADLIB_API BOOL STDeviceGetLedDefaultFlag();
extern "C" STPADLIB_API VOID STDeviceSetLedDefaultFlag(BOOL bFlag);
extern "C" STPADLIB_API LONG STDisplaySetTarget(LONG nTarget);
extern "C" STPADLIB_API LONG STDisplayErase();
extern "C" STPADLIB_API LONG STDisplayEraseRect(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG STDisplaySetFont(LPCWSTR szName, LONG nSize, LONG nOptions);
extern "C" STPADLIB_API LONG STDisplaySetFontColor(COLORREF clrFont);
extern "C" STPADLIB_API LONG STDisplaySetText(LONG nXPos, LONG nYPos, ALIGN nAlignment, LPCWSTR szText);
extern "C" STPADLIB_API LONG STDisplaySetTextInRect(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight, ALIGN nAlignment, LPCWSTR szText, LONG nOptions=0);
#ifdef _WINDOWS
extern "C" STPADLIB_API LONG STDisplaySetImage(LONG nXPos, LONG nYPos, HBITMAP hBitmap);
#endif
extern "C" STPADLIB_API LONG STDisplaySetImageFromFile(LONG nXPos, LONG nYPos, LPCWSTR szPath);
#ifdef _WINDOWS
extern "C" STPADLIB_API LONG STDisplaySetPDF(LONG nXPos, LONG nYPos, LONG nPage, DOUBLE dScale, LONG nOptions);
#endif
extern "C" STPADLIB_API LONG STDisplaySetImageFromStore(LONG nStoreId);
extern "C" STPADLIB_API LONG STDisplaySetOverlayRect(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG STDisplaySetScrollPos(LONG nXPos, LONG nYPos);
extern "C" STPADLIB_API LONG STDisplayGetScrollPos(LONG* pnXPos, LONG* pnYPos);
extern "C" STPADLIB_API LONG STDisplayGetStandbyId(LPCWSTR szId, LONG* pnStringLength);
#ifdef _WINDOWS
extern "C" STPADLIB_API LONG STDisplaySetStandbyImage(HBITMAP hBitmap);
#endif
extern "C" STPADLIB_API LONG STDisplaySetStandbyImageFromFile(LPCWSTR szPath);
extern "C" STPADLIB_API LONG STDisplayConfigSlideShow(LPCWSTR szSlideList, LONG nDuration);
extern "C" STPADLIB_API LONG STDisplayConfigSlideShowEx(LPCWSTR szSlideList, LPCWSTR szDurationList);
#ifdef _WINDOWS
extern "C" STPADLIB_API LONG STDisplaySaveImageAsStream(BYTE* pbtImage, LONG* pnSize, FILETYPE nFileType, LONG nOptions);
#endif
extern "C" STPADLIB_API LONG STDisplaySaveImageAsFile(LPCWSTR szPath, FILETYPE nFileType, LONG nOptions);
extern "C" STPADLIB_API LONG STDisplayConfigPen(LONG nWidth, COLORREF clrPen);
extern "C" STPADLIB_API LONG STDisplaySetBacklight(BACKLIGHT nMode);
extern "C" STPADLIB_API LONG STDisplayGetWidth();
extern "C" STPADLIB_API LONG STDisplayGetHeight();
extern "C" STPADLIB_API DOUBLE STDisplayGetResolution();
extern "C" STPADLIB_API LONG STDisplayGetTargetWidth();
extern "C" STPADLIB_API LONG STDisplayGetTargetHeight();
extern "C" STPADLIB_API LONG STDisplayGetScrollSpeed();
extern "C" STPADLIB_API LONG STDisplaySetScrollSpeed(LONG nSpeed);
extern "C" STPADLIB_API LONG STDisplayGetRotation();
extern "C" STPADLIB_API LONG STDisplaySetRotation(LONG nRotation);
#ifdef _WINDOWS
extern "C" STPADLIB_API LONG STSignatureSetSecureMode(BOOL bEnable);
#endif
extern "C" STPADLIB_API LONG STSignatureStart();
extern "C" STPADLIB_API LONG STSignatureRetry();
extern "C" STPADLIB_API LONG STSignatureStop();
extern "C" STPADLIB_API LONG STSignatureConfirm();
extern "C" STPADLIB_API LONG STSignatureCancel(ERASEOPTION nErase=kComplete);
extern "C" STPADLIB_API LONG STSignatureScaleToDisplay(LONG nSensorValue);
extern "C" STPADLIB_API LONG STSignatureGetBounds(LONG* pnLeft, LONG* pnTop, LONG* pnRight, LONG* pnBottom, LONG nOptions);
extern "C" STPADLIB_API LONG STSignatureSaveAsStreamEx(BYTE* pbtImage, LONG* pnSize, LONG nResolution, LONG nWidth, LONG nHeight, FILETYPE nFileType, LONG nPenWidth, COLORREF clrPen, LONG nOptions);
extern "C" STPADLIB_API LONG STSignatureSaveAsFileEx(LPCWSTR szPath, LONG nResolution, LONG nWidth, LONG nHeight, FILETYPE nFileType, LONG nPenWidth, COLORREF clrPen, LONG nOptions);
extern "C" STPADLIB_API BOOL STSignatureGetState();
extern "C" STPADLIB_API LONG STSignatureGetSignData(BYTE* pbtSignData, LONG* pnSize);
extern "C" STPADLIB_API LONG STSignatureGetIsoData(BYTE* pbtIsoData, LONG* pnSize, LONG nOptions, BYTE* pbtCustomData, LONG nCustomDataSize, LONG nCustomDataBlocks);
extern "C" STPADLIB_API LONG STSensorGetSampleRateMode();
extern "C" STPADLIB_API LONG STSensorSetSampleRateMode(LONG nMode);
extern "C" STPADLIB_API LONG STSensorStartTimer(LONG nWaitBeforeAction, LONG nWaitAfterAction, LONG nOptions);
extern "C" STPADLIB_API LONG STSensorStopTimer();
extern "C" STPADLIB_API LONG STSensorSetScrollArea(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG STSensorSetPenScrollingEnabled(BOOL bEnable);
extern "C" STPADLIB_API LONG STSensorSetSignRect(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG STSensorClearSignRect();
extern "C" STPADLIB_API LONG STSensorAddHotSpot(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG STSensorAddScrollHotSpot(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight, HOTSPOTTYPE nOptions);
extern "C" STPADLIB_API LONG STSensorClearHotSpots();
extern "C" STPADLIB_API LONG STSensorSetHotspotMode(HOTSPOTMODE nMode, LONG nHotSpotId);
#ifdef _WINDOWS
extern "C" STPADLIB_API LONG STRSAGenerateSigningCert(LONG nKeyLen, LONG nValidity);
extern "C" STPADLIB_API LONG STRSAGenerateSigningCertPw(LONG nKeyLen, LONG nValidity, LPCWSTR szDevicePassword);
extern "C" STPADLIB_API LONG STRSASetSigningCert(BYTE* pbtCert, LONG nSize, LPCWSTR szPassword);
extern "C" STPADLIB_API LONG STRSASetSigningCertPw(BYTE* pbtCert, LONG nSize, LPCWSTR szPassword, LPCWSTR szDevicePassword);
extern "C" STPADLIB_API LONG STRSASaveSigningCertAsStream(BYTE* pbtCert, LONG* pnSize, CERTTYPE nType);
extern "C" STPADLIB_API LONG STRSASaveSigningCertAsFile(LPCWSTR szPath, CERTTYPE nType);
extern "C" STPADLIB_API LONG STRSASetHash(BYTE* pbtData, HASHALGO nAlgorithm, LONG nOptions, LONG nDataSize=0);
extern "C" STPADLIB_API LONG STRSACreateDisplayHash(BYTE* pbtImageData, LONG* pnSize, HASHALGO nAlgorithm, LONG nSource);
extern "C" STPADLIB_API LONG STRSACreateHashedImage(BYTE* pbtImage, LONG* pnSize, BYTE* pbtImageData, LONG nImageDataSize, COLORREF clrBack, LONG nModelType, FILETYPE nFileType);
extern "C" STPADLIB_API LONG STRSASign(BYTE* pbtSignature, LONG* pnSize, RSASCHEME nScheme, HASHVALUE nHashValue, LONG nOptions);
extern "C" STPADLIB_API LONG STRSASetEncryptionCert(BYTE* pbtCert, LONG nSize);
extern "C" STPADLIB_API LONG STRSASetEncryptionCertPw(BYTE* pbtCert, LONG nSize, LPCWSTR szDevicePassword);
extern "C" STPADLIB_API LONG STRSAGetEncryptionCertId(LPCWSTR szId, LONG* pnStringLength);
extern "C" STPADLIB_API LONG STRSAGetSignData(BYTE* pbtSignData, LONG* pnSize, LONG nOptions);
extern "C" STPADLIB_API LONG STRSADecryptSignData(BYTE* pbtDecrypted, LONG* pnDecryptedSize, BYTE* pbtEncrypted, LONG nEncryptedSize, BYTE* pbtCert, LONG nCertSize, LPCWSTR szPassword, LONG* pnExtraData);
extern "C" STPADLIB_API LONG STPDFLoad(BYTE* pbtDocument, LONG nSize, LPCWSTR szPassword);
extern "C" STPADLIB_API LONG STPDFGetPageCount();
extern "C" STPADLIB_API DOUBLE STPDFGetWidth(LONG nPage, MEASUREMENTUNIT nUnit);
extern "C" STPADLIB_API DOUBLE STPDFGetHeight(LONG nPage, MEASUREMENTUNIT nUnit);
extern "C" STPADLIB_API LONG STPDFSelectRect(LONG nPage, DOUBLE dLeft, DOUBLE dTop, DOUBLE dWidth, DOUBLE dHeight, MEASUREMENTUNIT nUnit);
extern "C" STPADLIB_API LONG STPDFAddImage(LONG nPage, LONG nLeft, LONG nTop, HBITMAP hBitmap);
extern "C" STPADLIB_API LONG STPDFAddImageFromFile(LONG nPage, LONG nLeft, LONG nTop, LPCWSTR szPath);
extern "C" STPADLIB_API LONG STPDFRemoveImage(LONG nPage, LONG nId);
#endif

#ifdef _WINDOWS
#if !_WIN64
extern "C" STPADLIB_API VOID __stdcall STControlSetAppName_stdcall(LPCWSTR szName);
extern "C" STPADLIB_API VOID __stdcall STControlSetCallback_stdcall(CBPTR pCallback, LPVOID pCustomPar);
extern "C" STPADLIB_API LONG __stdcall STControlSetLogDirectory_stdcall(LPCWSTR szPath);
extern "C" STPADLIB_API LONG __stdcall STControlGetVersion_stdcall(WCHAR szVersion[VERSION_SIZE]);
extern "C" STPADLIB_API LONG __stdcall STControlGetErrorString_stdcall(LPCWSTR szError, LONG* pnStringLength, LONG nErrorId);
extern "C" STPADLIB_API VOID __stdcall STControlExit_stdcall();
extern "C" STPADLIB_API LONG __stdcall STDeviceSetComPort_stdcall(LPCWSTR szPortList);
extern "C" STPADLIB_API LONG __stdcall STDeviceGetComPort_stdcall(LONG nIndex);
extern "C" STPADLIB_API LONG __stdcall STDeviceGetIPAddress_stdcall(WCHAR szAddress[IP_ADDRESS_SIZE], LONG nIndex);
extern "C" STPADLIB_API LONG __stdcall STDeviceGetConnectionType_stdcall(LONG nIndex);
extern "C" STPADLIB_API LONG __stdcall STDeviceGetCount_stdcall();
extern "C" STPADLIB_API LONG __stdcall STDeviceGetInfo_stdcall(WCHAR szSerial[SERIAL_SIZE], LONG* pnType, LONG nIndex);
extern "C" STPADLIB_API LONG __stdcall STDeviceGetVersion_stdcall(WCHAR szVersion[VERSION_SIZE], LONG nIndex);
extern "C" STPADLIB_API LONG __stdcall STDeviceGetCapabilities_stdcall(LONG nIndex);
extern "C" STPADLIB_API LONG __stdcall STDeviceOpen_stdcall(LONG nIndex, BOOL bEraseDisplay=TRUE);
extern "C" STPADLIB_API LONG __stdcall STDeviceClose_stdcall(LONG nIndex);
extern "C" STPADLIB_API LONG __stdcall STDeviceSetLed_stdcall(LONG nLedColor);
extern "C" STPADLIB_API LONG __stdcall STDeviceStartService_stdcall(LONG nType);
extern "C" STPADLIB_API LONG __stdcall STDeviceGetNFCMode_stdcall(LONG nIndex);
extern "C" STPADLIB_API LONG __stdcall STDeviceSetNFCMode_stdcall(LONG nMode, LONG nIndex);
extern "C" STPADLIB_API BOOL __stdcall STDeviceGetLedDefaultFlag_stdcall();
extern "C" STPADLIB_API VOID __stdcall STDeviceSetLedDefaultFlag_stdcall(BOOL bFlag);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetTarget_stdcall(LONG nTarget);
extern "C" STPADLIB_API LONG __stdcall STDisplayErase_stdcall();
extern "C" STPADLIB_API LONG __stdcall STDisplayEraseRect_stdcall(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetFont_stdcall(LPCWSTR szName, LONG nSize, LONG nOptions);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetFontColor_stdcall(COLORREF clrFont);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetText_stdcall(LONG nXPos, LONG nYPos, ALIGN nAlignment, LPCWSTR szText);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetTextInRect_stdcall(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight, ALIGN nAlignment, LPCWSTR szText, LONG nOptions=0);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetImage_stdcall(LONG nXPos, LONG nYPos, HBITMAP hBitmap);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetImageFromFile_stdcall(LONG nXPos, LONG nYPos, LPCWSTR szPath);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetPDF_stdcall(LONG nXPos, LONG nYPos, LONG nPage, DOUBLE dScale, LONG nOptions);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetImageFromStore_stdcall(LONG nStoreId);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetOverlayRect_stdcall(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetScrollPos_stdcall(LONG nXPos, LONG nYPos);
extern "C" STPADLIB_API LONG __stdcall STDisplayGetScrollPos_stdcall(LONG* pnXPos, LONG* pnYPos);
extern "C" STPADLIB_API LONG __stdcall STDisplayGetStandbyId_stdcall(LPCWSTR szId, LONG* pnStringLength);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetStandbyImage_stdcall(HBITMAP hBitmap);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetStandbyImageFromFile_stdcall(LPCWSTR szPath);
extern "C" STPADLIB_API LONG __stdcall STDisplayConfigSlideShow_stdcall(LPCWSTR szSlideList, LONG nDuration);
extern "C" STPADLIB_API LONG __stdcall STDisplayConfigSlideShowEx_stdcall(LPCWSTR szSlideList, LPCWSTR szDurationList);
extern "C" STPADLIB_API LONG __stdcall STDisplaySaveImageAsStream_stdcall(BYTE* pbtImage, LONG* pnSize, FILETYPE nFileType, LONG nOptions);
extern "C" STPADLIB_API LONG __stdcall STDisplaySaveImageAsFile_stdcall(LPCWSTR szPath, FILETYPE nFileType, LONG nOptions);
extern "C" STPADLIB_API LONG __stdcall STDisplayConfigPen_stdcall(LONG nWidth, COLORREF clrPen);
extern "C" STPADLIB_API LONG __stdcall STDisplaySetBacklight_stdcall(BACKLIGHT nMode);
extern "C" STPADLIB_API LONG __stdcall STDisplayGetWidth_stdcall();
extern "C" STPADLIB_API LONG __stdcall STDisplayGetHeight_stdcall();
extern "C" STPADLIB_API DOUBLE __stdcall STDisplayGetResolution_stdcall();
extern "C" STPADLIB_API LONG __stdcall STDisplayGetTargetWidth_stdcall();
extern "C" STPADLIB_API LONG __stdcall STDisplayGetTargetHeight_stdcall();
extern "C" STPADLIB_API LONG __stdcall STDisplayGetScrollSpeed_stdcall();
extern "C" STPADLIB_API LONG __stdcall STDisplaySetScrollSpeed_stdcall(LONG nSpeed);
extern "C" STPADLIB_API LONG __stdcall STDisplayGetRotation_stdcall();
extern "C" STPADLIB_API LONG __stdcall STDisplaySetRotation_stdcall(LONG nRotation);
extern "C" STPADLIB_API LONG __stdcall STSignatureSetSecureMode_stdcall(BOOL bEnable);
extern "C" STPADLIB_API LONG __stdcall STSignatureStart_stdcall();
extern "C" STPADLIB_API LONG __stdcall STSignatureRetry_stdcall();
extern "C" STPADLIB_API LONG __stdcall STSignatureStop_stdcall();
extern "C" STPADLIB_API LONG __stdcall STSignatureConfirm_stdcall();
extern "C" STPADLIB_API LONG __stdcall STSignatureCancel_stdcall(ERASEOPTION nErase=kComplete);
extern "C" STPADLIB_API LONG __stdcall STSignatureScaleToDisplay_stdcall(LONG nSensorValue);
extern "C" STPADLIB_API LONG __stdcall STSignatureGetBounds_stdcall(LONG* pnLeft, LONG* pnTop, LONG* pnRight, LONG* pnBottom, LONG nOptions);
extern "C" STPADLIB_API LONG __stdcall STSignatureSaveAsStreamEx_stdcall(BYTE* pbtImage, LONG* pnSize, LONG nResolution, LONG nWidth, LONG nHeight, FILETYPE nFileType, LONG nPenWidth, COLORREF clrPen, LONG nOptions);
extern "C" STPADLIB_API LONG __stdcall STSignatureSaveAsFileEx_stdcall(LPCWSTR szPath, LONG nResolution, LONG nWidth, LONG nHeight, FILETYPE nFileType, LONG nPenWidth, COLORREF clrPen, LONG nOptions);
extern "C" STPADLIB_API BOOL __stdcall STSignatureGetState_stdcall();
extern "C" STPADLIB_API LONG __stdcall STSignatureGetSignData_stdcall(BYTE* pbtSignData, LONG* pnSize);
extern "C" STPADLIB_API LONG __stdcall STSignatureGetIsoData_stdcall(BYTE* pbtIsoData, LONG* pnSize, LONG nOptions, BYTE* pbtCustomData, LONG nCustomDataSize, LONG nCustomDataBlocks);
extern "C" STPADLIB_API LONG __stdcall STSensorGetSampleRateMode_stdcall();
extern "C" STPADLIB_API LONG __stdcall STSensorSetSampleRateMode_stdcall(LONG nMode);
extern "C" STPADLIB_API LONG __stdcall STSensorStartTimer_stdcall(LONG nWaitBeforeAction, LONG nWaitAfterAction, LONG nOptions);
extern "C" STPADLIB_API LONG __stdcall STSensorStopTimer_stdcall();
extern "C" STPADLIB_API LONG __stdcall STSensorSetScrollArea_stdcall(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG __stdcall STSensorSetPenScrollingEnabled_stdcall(BOOL bEnable);
extern "C" STPADLIB_API LONG __stdcall STSensorSetSignRect_stdcall(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG __stdcall STSensorClearSignRect_stdcall();
extern "C" STPADLIB_API LONG __stdcall STSensorAddHotSpot_stdcall(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight);
extern "C" STPADLIB_API LONG __stdcall STSensorAddScrollHotSpot_stdcall(LONG nLeft, LONG nTop, LONG nWidth, LONG nHeight, HOTSPOTTYPE nOptions);
extern "C" STPADLIB_API LONG __stdcall STSensorClearHotSpots_stdcall();
extern "C" STPADLIB_API LONG __stdcall STSensorSetHotspotMode_stdcall(HOTSPOTMODE nMode, LONG nHotSpotId);
extern "C" STPADLIB_API LONG __stdcall STRSAGenerateSigningCert_stdcall(LONG nKeyLen, LONG nValidity);
extern "C" STPADLIB_API LONG __stdcall STRSAGenerateSigningCertPw_stdcall(LONG nKeyLen, LONG nValidity, LPCWSTR szDevicePassword);
extern "C" STPADLIB_API LONG __stdcall STRSASetSigningCert_stdcall(BYTE* pbtCert, LONG nSize, LPCWSTR szPassword);
extern "C" STPADLIB_API LONG __stdcall STRSASetSigningCertPw_stdcall(BYTE* pbtCert, LONG nSize, LPCWSTR szPassword, LPCWSTR szDevicePassword);
extern "C" STPADLIB_API LONG __stdcall STRSASaveSigningCertAsStream_stdcall(BYTE* pbtCert, LONG* pnSize, CERTTYPE nType);
extern "C" STPADLIB_API LONG __stdcall STRSASaveSigningCertAsFile_stdcall(LPCWSTR szPath, CERTTYPE nType);
extern "C" STPADLIB_API LONG __stdcall STRSASetHash_stdcall(BYTE* pbtData, HASHALGO nAlgorithm, LONG nOptions, LONG nDataSize=0);
extern "C" STPADLIB_API LONG __stdcall STRSACreateDisplayHash_stdcall(BYTE* pbtImageData, LONG* pnSize, HASHALGO nAlgorithm, LONG nSource);
extern "C" STPADLIB_API LONG __stdcall STRSACreateHashedImage_stdcall(BYTE* pbtImage, LONG* pnSize, BYTE* pbtImageData, LONG nImageDataSize, COLORREF clrBack, LONG nModelType, FILETYPE nFileType);
extern "C" STPADLIB_API LONG __stdcall STRSASign_stdcall(BYTE* pbtSignature, LONG* pnSize, RSASCHEME nScheme, HASHVALUE nHashValue, LONG nOptions);
extern "C" STPADLIB_API LONG __stdcall STRSASetEncryptionCert_stdcall(BYTE* pbtCert, LONG nSize);
extern "C" STPADLIB_API LONG __stdcall STRSASetEncryptionCertPw_stdcall(BYTE* pbtCert, LONG nSize, LPCWSTR szDevicePassword);
extern "C" STPADLIB_API LONG __stdcall STRSAGetEncryptionCertId_stdcall(LPCWSTR szId, LONG* pnStringLength);
extern "C" STPADLIB_API LONG __stdcall STRSAGetSignData_stdcall(BYTE* pbtSignData, LONG* pnSize, LONG nOptions);
extern "C" STPADLIB_API LONG __stdcall STRSADecryptSignData_stdcall(BYTE* pbtDecrypted, LONG* pnDecryptedSize, BYTE* pbtEncrypted, LONG nEncryptedSize, BYTE* pbtCert, LONG nCertSize, LPCWSTR szPassword, LONG* pnExtraData);
extern "C" STPADLIB_API LONG __stdcall STPDFLoad_stdcall(BYTE* pbtDocument, LONG nSize, LPCWSTR szPassword);
extern "C" STPADLIB_API LONG __stdcall STPDFGetPageCount_stdcall();
extern "C" STPADLIB_API DOUBLE __stdcall STPDFGetWidth_stdcall(LONG nPage, MEASUREMENTUNIT nUnit);
extern "C" STPADLIB_API DOUBLE __stdcall STPDFGetHeight_stdcall(LONG nPage, MEASUREMENTUNIT nUnit);
extern "C" STPADLIB_API LONG __stdcall STPDFSelectRect_stdcall(LONG nPage, DOUBLE dLeft, DOUBLE dTop, DOUBLE dWidth, DOUBLE dHeight, MEASUREMENTUNIT nUnit); 
extern "C" STPADLIB_API LONG __stdcall STPDFAddImage_stdcall(LONG nPage, LONG nLeft, LONG nTop, HBITMAP hBitmap);
extern "C" STPADLIB_API LONG __stdcall STPDFAddImageFromFile_stdcall(LONG nPage, LONG nLeft, LONG nTop, LPCWSTR szPath);
extern "C" STPADLIB_API LONG __stdcall STPDFRemoveImage_stdcall(LONG nPage, LONG nId);
#endif
#endif
#endif
