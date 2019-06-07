
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>

#include <olectl.h>
#include <stdio.h>

#define DEFINE_DATA

#include "SignaturePad_i.h"
//
//NTC: 05-22-2019: Just out of the fucking blue, MS Visual Studio is somehow including the SignaturePad_i.c file in the first compiled file.
// In any case, it is complaining of multiply defined symbols from that file as being defined here and in the first file compiled, which is, at this time
// bitmaps.cpp
// I'll be damned if I can find out what happened to this projects settings to have caused this, thus the #if 0 below.
//
//
//NTC: 06-02-2019: Again, just out of the fucking blue, MS Visual Studio reverted this behavior and went back to the normal operation, thus changed the #if 0 to #if 1
//
//
//NTC: 06-03-2019: Well, that lasted all of one fucking day. Here we go back to the same thing as on 5/22/2019, #if 1 becomes #if 0
#if 0
#include "SignaturePad_i.c"
#endif
#include "Properties_i.h"
#include "Properties_i.c"

#include "SignaturePad.h"

WCHAR OBJECT_NAME[][256] = {L"InnoVisioNate.InnoVisioNateSignaturePad"};
WCHAR OBJECT_NAME_V[][256] = {L"InnoVisioNate.InnoVisioNateSignaturePad.1"};
WCHAR OBJECT_VERSION[][32] = {L"1.0"};

CLSID OBJECT_CLSID[] = {CLSID_CursiVisionSignaturePad};
CLSID OBJECT_LIBID[] = {LIBID_CursiVisionSignaturePad};
  
   int GetLocation(HWND hwnd,long key,WCHAR *szwFolderLocation);

   extern "C" BOOL WINAPI DllMain(HINSTANCE hI, DWORD dwReason, LPVOID) {

   switch ( dwReason ) {

   case DLL_PROCESS_ATTACH: {

      hModule = hI;

      GetModuleFileName(hModule,szwModuleName,1024);

      WideCharToMultiByte(CP_ACP,0,szwModuleName,-1,szModuleName,MAX_PATH,0,0);

      WCHAR szwTemp[MAX_PATH];

      GetLocation(NULL,CSIDL_COMMON_APPDATA,szwTemp);

      swprintf(szwApplicationDataDirectory,L"%ls\\CursiVision",szwTemp);

      CreateDirectory(szwApplicationDataDirectory,NULL);

      WideCharToMultiByte(CP_ACP,0,szwApplicationDataDirectory,-1,szApplicationDataDirectory,MAX_PATH,0,0);

      swprintf(szwApplicationDataDirectory,L"%ls\\CursiVision\\Settings",szwTemp);

      CreateDirectory(szwApplicationDataDirectory,NULL);
      
      swprintf(szwApplicationDataDirectory,L"%ls\\CursiVision",szwTemp);

      GetLocation(NULL,CSIDL_PERSONAL,szwTemp);

      swprintf(szwUserDirectory,L"%ls\\CursiVision Files",szwTemp);

      WideCharToMultiByte(CP_ACP,0,szwUserDirectory,-1,szUserDirectory,MAX_PATH,0,0);

      }

      break;
  
   case DLL_PROCESS_DETACH:
      break;
  
   }
  
   return TRUE;
   }
  
  
   extern "C" int  __cdecl GetDocumentsLocation(HWND hwnd,WCHAR *szwFolderLocation) {
   GetLocation(hwnd,CSIDL_PERSONAL,szwFolderLocation);
   return 0;
   }

   extern "C" int __cdecl GetCommonAppDataLocation(HWND hwnd,WCHAR *szwFolderLocation) {
   GetLocation(hwnd,CSIDL_COMMON_APPDATA,szwFolderLocation);
   return 0;
   }

   int GetLocation(HWND hwnd,long key,WCHAR *szwFolderLocation) {

   ITEMIDLIST *ppItemIDList;
   IShellFolder *pIShellFolder;
   LPCITEMIDLIST pcParentIDList;

   HRESULT wasInitialized = CoInitialize(NULL);

   szwFolderLocation[0] = L'\0';

   HRESULT rc = SHGetFolderLocation(hwnd,key,NULL,0,&ppItemIDList);

   if ( S_OK != rc ) 
      return 0;

   rc = SHBindToParent(ppItemIDList, IID_IShellFolder, (void **) &pIShellFolder, &pcParentIDList);
   if ( S_OK != rc )
      return 0;

   STRRET strRet;
   rc = pIShellFolder -> GetDisplayNameOf(pcParentIDList,SHGDN_FORPARSING,&strRet);
   pIShellFolder -> Release();
   if ( S_OK != rc ) 
      return 0;

   //WideCharToMultiByte(CP_ACP,0,strRet.pOleStr,-1,szwFolderLocation,MAX_PATH,0,0);
   wcscpy(szwFolderLocation,strRet.pOleStr);

   if ( S_FALSE == wasInitialized )
      CoUninitialize();

   return 0;
   }


   class Factory : public IClassFactory {
   public:
  
      Factory(CLSID objectCLSID) : myObject(objectCLSID), refCount(0) {};
      ~Factory() {};
  
      STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();
      STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
      STDMETHOD (LockServer)(BOOL fLock);
  
   private:
      CLSID myObject;
      int refCount;

   };
  
  
   static Factory factory(OBJECT_CLSID[0]);

   STDAPI DllCanUnloadNow(void) {
   return S_OK;
   }
  
  
   STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppObject) {
   *ppObject = NULL;
   if ( rclsid != OBJECT_CLSID[0] )
      return CLASS_E_CLASSNOTAVAILABLE;
   return factory.QueryInterface(riid,ppObject);
   }
  
  
   STDAPI DllRegisterServer() {

   HRESULT rc = S_OK;
   ITypeLib *ptLib;
   HKEY keyHandle,clsidHandle;
   DWORD disposition;
   WCHAR szwTemp[256],szwCLSID[256];
   LPOLESTR oleString;
  
   for ( long objectIndex = 0; objectIndex < 1; objectIndex++ ) {

      StringFromCLSID(OBJECT_CLSID[objectIndex],&oleString);

      wcscpy(szwCLSID,oleString);
      //WideCharToMultiByte(CP_ACP,0,oleString,-1,szwCLSID,256,0,0);
    
      if ( S_OK != (rc = LoadTypeLib(szwModuleName,&ptLib)) )
         rc = ResultFromScode(SELFREG_E_TYPELIB);
      else
         if ( S_OK != (rc = RegisterTypeLib(ptLib,szwModuleName,NULL)) )
            rc = ResultFromScode(SELFREG_E_TYPELIB);

      rc = RegOpenKeyEx(HKEY_CLASSES_ROOT,L"CLSID",0,KEY_CREATE_SUB_KEY,&keyHandle);
    
         rc = RegCreateKeyEx(keyHandle,szwCLSID,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&clsidHandle,&disposition);
         swprintf(szwTemp,L"InnoVisioNate Signature Pad Object");
         rc = RegSetValueEx(clsidHandle,NULL,0,REG_SZ,(BYTE *)szwTemp,2 * (DWORD)wcslen(szwTemp));

         swprintf(szwTemp,L"Control");
         rc = RegCreateKeyEx(clsidHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         swprintf(szwTemp,L"");
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwTemp,2 * (DWORD)wcslen(szwTemp));
    
         swprintf(szwTemp,L"ProgID");
         rc = RegCreateKeyEx(clsidHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         swprintf(szwTemp,OBJECT_NAME_V[objectIndex]);
         rc = RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwTemp,2 * (DWORD)wcslen(szwTemp));
    
         swprintf(szwTemp,L"InprocServer");
         RegCreateKeyEx(clsidHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwModuleName,2 * (DWORD)wcslen(szwModuleName));
    
         swprintf(szwTemp,L"InprocServer32");
         rc = RegCreateKeyEx(clsidHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         rc = RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwModuleName,2 * (DWORD)wcslen(szwModuleName));
// //      RegSetValueEx(keyHandle,"ThreadingModel",0,REG_SZ,(BYTE *)"Free",5);
         rc = RegSetValueEx(keyHandle,L"ThreadingModel",0,REG_SZ,(BYTE *)L"Apartment",18);
    
         swprintf(szwTemp,L"LocalServer");
         rc = RegCreateKeyEx(clsidHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         rc = RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwModuleName,2 * (DWORD)wcslen(szwModuleName));
       
         swprintf(szwTemp,L"TypeLib");
         rc = RegCreateKeyEx(clsidHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
       
         StringFromCLSID(OBJECT_LIBID[objectIndex],&oleString);
         //WideCharToMultiByte(CP_ACP,0,oleString,-1,szwTemp,256,0,0);
         wcscpy(szwTemp,oleString);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwTemp,2 * (DWORD)wcslen(szwTemp));
           
         swprintf(szwTemp,L"ToolboxBitmap32");
         RegCreateKeyEx(clsidHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
// //      swprintf(szwTemp,"%s, 1",szModuleName);
// //      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwTemp,wcslen(szModuleName));
    
         swprintf(szwTemp,L"Version");
         RegCreateKeyEx(clsidHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         swprintf(szwTemp,OBJECT_VERSION[objectIndex]);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwTemp,2 * (DWORD)wcslen(szwTemp));
    
         swprintf(szwTemp,L"MiscStatus");
         RegCreateKeyEx(clsidHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         swprintf(szwTemp,L"0");
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwTemp,2 * (DWORD)wcslen(szwTemp));
    
         swprintf(szwTemp,L"1");
         RegCreateKeyEx(keyHandle,szwTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         swprintf(szwTemp,L"%ld",
                    OLEMISC_ALWAYSRUN |
                    OLEMISC_ACTIVATEWHENVISIBLE | 
                    OLEMISC_RECOMPOSEONRESIZE | 
                    OLEMISC_INSIDEOUT |
                    OLEMISC_SETCLIENTSITEFIRST |
                    OLEMISC_CANTLINKINSIDE );
//s//swprintf(szwTemp,"%ld",131473L);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwTemp,2 * (DWORD)wcslen(szwTemp));
    
      RegCreateKeyEx(HKEY_CLASSES_ROOT,OBJECT_NAME[objectIndex],0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         RegCreateKeyEx(keyHandle,L"CurVer",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         swprintf(szwTemp,OBJECT_NAME_V[objectIndex]);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwTemp,(DWORD)wcslen(szwTemp));
    
      RegCreateKeyEx(HKEY_CLASSES_ROOT,OBJECT_NAME_V[objectIndex],0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         RegCreateKeyEx(keyHandle,L"CLSID",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szwCLSID,2 * (DWORD)wcslen(szwCLSID));
    
   }

   return S_OK;
   }
  
  
   STDAPI DllUnregisterServer() {

   HRESULT rc = S_OK;
   HKEY keyHandle;
   WCHAR szwCLSID[256];
   LPOLESTR oleString;
  
   for ( long objectIndex = 0; objectIndex < 1; objectIndex++ ) {

      StringFromCLSID(OBJECT_CLSID[objectIndex],&oleString);

      //WideCharToMultiByte(CP_ACP,0,oleString,-1,szCLSID,256,0,0);
      wcscpy(szwCLSID,oleString);
  
      rc = RegOpenKeyEx(HKEY_CLASSES_ROOT,L"CLSID",0,KEY_CREATE_SUB_KEY,&keyHandle);

      rc = SHDeleteKey(keyHandle,szwCLSID);

      rc = SHDeleteKey(HKEY_CLASSES_ROOT,OBJECT_NAME[objectIndex]);

      rc = SHDeleteKey(HKEY_CLASSES_ROOT,OBJECT_NAME_V[objectIndex]);

   }
   return S_OK;
   }
  
  
   long __stdcall Factory::QueryInterface(REFIID iid, void **ppv) { 
   *ppv = NULL; 
   if ( iid == IID_IUnknown || iid == IID_IClassFactory ) 
      *ppv = this; 
   else 
      return E_NOINTERFACE; 
   AddRef(); 
   return S_OK; 
   } 
  
  
   unsigned long __stdcall Factory::AddRef() { 
   return ++refCount; 
   } 
  
  
   unsigned long __stdcall Factory::Release() { 
   return --refCount;
   } 
  
  
   HRESULT STDMETHODCALLTYPE Factory::CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv) { 
   *ppv = NULL; 
   SignaturePad *pef = new SignaturePad(punkOuter);
   return pef -> QueryInterface(riid,ppv);
   } 
  
  
   long __stdcall Factory::LockServer(int fLock) { 
   return S_OK; 
   }