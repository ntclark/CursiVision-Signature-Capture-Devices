
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>

#include <olectl.h>
#include <stdio.h>

#define DEFINE_DATA

#include "SignaturePad_i.h"
#include "SignaturePad_i.c"
#include "Properties_i.h"
#include "Properties_i.c"

#include "SignaturePad.h"

char OBJECT_NAME[][256] = {"InnoVisioNate.InnoVisioNateSignaturePad"};
char OBJECT_NAME_V[][256] = {"InnoVisioNate.InnoVisioNateSignaturePad.1"};
char OBJECT_VERSION[][32] = {"1.0"};

CLSID OBJECT_CLSID[] = {CLSID_CursiVisionSignaturePad};
CLSID OBJECT_LIBID[] = {LIBID_CursiVisionSignaturePad};
  
   int GetLocation(HWND hwnd,long key,char *szFolderLocation);

   OLECHAR wstrModuleName[256];

   extern "C" BOOL WINAPI DllMain(HINSTANCE hI, DWORD dwReason, LPVOID) {

   switch ( dwReason ) {

   case DLL_PROCESS_ATTACH: {

      hModule = hI;

      GetModuleFileName(hModule,szModuleName,1024);

      memset(wstrModuleName,0,sizeof(wstrModuleName));

      MultiByteToWideChar(CP_ACP, 0, szModuleName, -1, wstrModuleName, strlen(szModuleName));  

      char szTemp[MAX_PATH];

      GetLocation(NULL,CSIDL_COMMON_APPDATA,szTemp);
//      GetCommonAppDataLocation(NULL,szTemp);

      sprintf(szApplicationDataDirectory,"%s\\CursiVision",szTemp);

      CreateDirectory(szApplicationDataDirectory,NULL);

      sprintf(szApplicationDataDirectory,"%s\\CursiVision\\Settings",szTemp);

      CreateDirectory(szApplicationDataDirectory,NULL);
      
      sprintf(szApplicationDataDirectory,"%s\\CursiVision",szTemp);

      GetLocation(NULL,CSIDL_PERSONAL,szTemp);
//      GetDocumentsLocation(NULL,szTemp);

      sprintf(szUserDirectory,"%s\\CursiVision Files",szTemp);

      }

      break;
  
   case DLL_PROCESS_DETACH:
      break;
  
   }
  
   return TRUE;
   }
  
  
   extern "C" int  __cdecl GetDocumentsLocation(HWND hwnd,char *szFolderLocation) {
   GetLocation(hwnd,CSIDL_PERSONAL,szFolderLocation);
   return 0;
   }

   extern "C" int __cdecl GetCommonAppDataLocation(HWND hwnd,char *szFolderLocation) {
   GetLocation(hwnd,CSIDL_COMMON_APPDATA,szFolderLocation);
   return 0;
   }

   int GetLocation(HWND hwnd,long key,char *szFolderLocation) {

   ITEMIDLIST *ppItemIDList;
   IShellFolder *pIShellFolder;
   LPCITEMIDLIST pcParentIDList;

   HRESULT wasInitialized = CoInitialize(NULL);

   szFolderLocation[0] = '\0';

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

   WideCharToMultiByte(CP_ACP,0,strRet.pOleStr,-1,szFolderLocation,MAX_PATH,0,0);

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
   char szTemp[256],szCLSID[256];
   LPOLESTR oleString;
  
   for ( long objectIndex = 0; objectIndex < 1; objectIndex++ ) {

      StringFromCLSID(OBJECT_CLSID[objectIndex],&oleString);

      WideCharToMultiByte(CP_ACP,0,oleString,-1,szCLSID,256,0,0);
    
      if ( S_OK != (rc = LoadTypeLib(wstrModuleName,&ptLib)) )
         rc = ResultFromScode(SELFREG_E_TYPELIB);
      else
         if ( S_OK != (rc = RegisterTypeLib(ptLib,wstrModuleName,NULL)) )
            rc = ResultFromScode(SELFREG_E_TYPELIB);

      rc = RegOpenKeyEx(HKEY_CLASSES_ROOT,"CLSID",0,KEY_CREATE_SUB_KEY,&keyHandle);
    
         rc = RegCreateKeyEx(keyHandle,szCLSID,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&clsidHandle,&disposition);
         sprintf(szTemp,"InnoVisioNate Signature Pad Object");
         rc = RegSetValueEx(clsidHandle,NULL,0,REG_SZ,(BYTE *)szTemp,strlen(szTemp));
    
         sprintf(szTemp,"Control");
         rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         sprintf(szTemp,"");
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,strlen(szTemp));
    
         sprintf(szTemp,"ProgID");
         rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         sprintf(szTemp,OBJECT_NAME_V[objectIndex]);
         rc = RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,strlen(szTemp));
    
         sprintf(szTemp,"InprocServer");
         RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szModuleName,strlen(szModuleName));
    
         sprintf(szTemp,"InprocServer32");
         rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         rc = RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szModuleName,strlen(szModuleName));
// //      RegSetValueEx(keyHandle,"ThreadingModel",0,REG_SZ,(BYTE *)"Free",5);
         rc = RegSetValueEx(keyHandle,"ThreadingModel",0,REG_SZ,(BYTE *)"Apartment",9);
    
         sprintf(szTemp,"LocalServer");
         rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         rc = RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szModuleName,strlen(szModuleName));
       
         sprintf(szTemp,"TypeLib");
         rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
       
         StringFromCLSID(OBJECT_LIBID[objectIndex],&oleString);
         WideCharToMultiByte(CP_ACP,0,oleString,-1,szTemp,256,0,0);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,strlen(szTemp));
           
         sprintf(szTemp,"ToolboxBitmap32");
         RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
// //      sprintf(szTemp,"%s, 1",szModuleName);
// //      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,strlen(szModuleName));
    
         sprintf(szTemp,"Version");
         RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         sprintf(szTemp,OBJECT_VERSION[objectIndex]);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,strlen(szTemp));
    
         sprintf(szTemp,"MiscStatus");
         RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         sprintf(szTemp,"0");
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,strlen(szTemp));
    
         sprintf(szTemp,"1");
         RegCreateKeyEx(keyHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         sprintf(szTemp,"%ld",
                    OLEMISC_ALWAYSRUN |
                    OLEMISC_ACTIVATEWHENVISIBLE | 
                    OLEMISC_RECOMPOSEONRESIZE | 
                    OLEMISC_INSIDEOUT |
                    OLEMISC_SETCLIENTSITEFIRST |
                    OLEMISC_CANTLINKINSIDE );
//s//sprintf(szTemp,"%ld",131473L);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,strlen(szTemp));
    
      RegCreateKeyEx(HKEY_CLASSES_ROOT,OBJECT_NAME[objectIndex],0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         RegCreateKeyEx(keyHandle,"CurVer",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         sprintf(szTemp,OBJECT_NAME_V[objectIndex]);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,strlen(szTemp));
    
      RegCreateKeyEx(HKEY_CLASSES_ROOT,OBJECT_NAME_V[objectIndex],0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         RegCreateKeyEx(keyHandle,"CLSID",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
         RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szCLSID,strlen(szCLSID));
    
   }

   return S_OK;
   }
  
  
   STDAPI DllUnregisterServer() {

   HRESULT rc = S_OK;
   HKEY keyHandle;
   char szCLSID[256];
   LPOLESTR oleString;
  
   for ( long objectIndex = 0; objectIndex < 1; objectIndex++ ) {

      StringFromCLSID(OBJECT_CLSID[objectIndex],&oleString);

      WideCharToMultiByte(CP_ACP,0,oleString,-1,szCLSID,256,0,0);
  
      rc = RegOpenKeyEx(HKEY_CLASSES_ROOT,"CLSID",0,KEY_CREATE_SUB_KEY,&keyHandle);

      rc = SHDeleteKey(keyHandle,szCLSID);

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