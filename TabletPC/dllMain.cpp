
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>

#include <olectl.h>
#include <stdio.h>

#define OBJECT_NAME "InnoVisioNate.CursiVisionSignaturePad"
#define OBJECT_NAME_V "InnoVisioNate.CursiVisionSignaturePad.1"
#define OBJECT_VERSION "1.0"
#define OBJECT_CLSID CLSID_CursiVisionSignaturePad
#define OBJECT_LIBID LIBID_CursiVisionSignaturePad

#define DEFINE_DATA

#include "utilities.h"

#include "SignaturePad_i.h"
#include "SignaturePad_i.c"
#include "Properties_i.h"
#include "Properties_i.c"

#include "TabletPC.h"

   OLECHAR wstrModuleName[256];

   extern "C" BOOL WINAPI DllMain(HINSTANCE hI, DWORD dwReason, LPVOID) {

   switch ( dwReason ) {

   case DLL_PROCESS_ATTACH: {

      hModule = hI;

      GetModuleFileName(hModule,szModuleName,1024);
      memset(wstrModuleName,0,sizeof(wstrModuleName));

      MultiByteToWideChar(CP_ACP, 0, szModuleName, -1, wstrModuleName, (DWORD)strlen(szModuleName));  

      char szTemp[MAX_PATH];

      GetCommonAppDataLocation(NULL,szTemp);

      sprintf(szApplicationDataDirectory,"%s\\CursiVision",szTemp);

      CreateDirectory(szApplicationDataDirectory,NULL);

      sprintf(szApplicationDataDirectory,"%s\\CursiVision\\Settings",szTemp);

      CreateDirectory(szApplicationDataDirectory,NULL);
      
      sprintf(szApplicationDataDirectory,"%s\\CursiVision",szTemp);

      GetDocumentsLocation(NULL,szTemp);

      sprintf(szUserDirectory,"%s\\CursiVision Files",szTemp);

      }

      break;
  
   case DLL_PROCESS_DETACH:
      break;
  
   }
  
   return TRUE;
   }
  

   class Factory : public IClassFactory {
   public:
  
      STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();
      STDMETHOD (CreateInstance)(IUnknown *punkOuter, REFIID riid, void **ppv);
      STDMETHOD (LockServer)(BOOL fLock);
  
      Factory() : refCount(0) {};
      ~Factory() {};
  
   private:
      int refCount;
   };
  
  
   static Factory factory;
  
   STDAPI DllCanUnloadNow(void) {
   return S_OK;
   }
  
  
  
   STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppObject) {
   *ppObject = NULL;
   if ( rclsid != OBJECT_CLSID ) 
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
  
   StringFromCLSID(OBJECT_CLSID,&oleString);
   WideCharToMultiByte(CP_ACP,0,oleString,-1,szCLSID,256,0,0);
  
   if ( S_OK != (rc = LoadTypeLib(wstrModuleName,&ptLib)) )
      rc = ResultFromScode(SELFREG_E_TYPELIB);
   else
      if ( S_OK != (rc = RegisterTypeLib(ptLib,wstrModuleName,NULL)) )
         rc = ResultFromScode(SELFREG_E_TYPELIB);

   rc = RegOpenKeyEx(HKEY_CLASSES_ROOT,"CLSID",0,KEY_CREATE_SUB_KEY,&keyHandle);
  
      rc = RegCreateKeyEx(keyHandle,szCLSID,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&clsidHandle,&disposition);
      sprintf(szTemp,"InnoVisioNate CursiVision Signature Pad Object");
      rc = RegSetValueEx(clsidHandle,NULL,0,REG_SZ,(BYTE *)szTemp,(DWORD)strlen(szTemp));
  
      sprintf(szTemp,"Control");
      rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      sprintf(szTemp,"");
      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,(DWORD)strlen(szTemp));
  
      sprintf(szTemp,"ProgID");
      rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      sprintf(szTemp,OBJECT_NAME_V);
      rc = RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,(DWORD)strlen(szTemp));
  
      sprintf(szTemp,"InprocServer");
      RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szModuleName,(DWORD)strlen(szModuleName));
  
      sprintf(szTemp,"InprocServer32");
      rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      rc = RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szModuleName,(DWORD)strlen(szModuleName));
//      RegSetValueEx(keyHandle,"ThreadingModel",0,REG_SZ,(BYTE *)"Free",5);
      rc = RegSetValueEx(keyHandle,"ThreadingModel",0,REG_SZ,(BYTE *)"Apartment",9);
  
      sprintf(szTemp,"LocalServer");
      rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      rc = RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szModuleName,(DWORD)strlen(szModuleName));
    
      sprintf(szTemp,"TypeLib");
      rc = RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
    
      StringFromCLSID(OBJECT_LIBID,&oleString);
      WideCharToMultiByte(CP_ACP,0,oleString,-1,szTemp,256,0,0);
      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,(DWORD)strlen(szTemp));
        
      sprintf(szTemp,"ToolboxBitmap32");
      RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
//      sprintf(szTemp,"%s, 1",szModuleName);
//      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,(DWORD)strlen(szModuleName));
  
      sprintf(szTemp,"Version");
      RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      sprintf(szTemp,OBJECT_VERSION);
      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,(DWORD)strlen(szTemp));
  
      sprintf(szTemp,"MiscStatus");
      RegCreateKeyEx(clsidHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      sprintf(szTemp,"0");
      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,(DWORD)strlen(szTemp));
  
      sprintf(szTemp,"1");
      RegCreateKeyEx(keyHandle,szTemp,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      sprintf(szTemp,"%ld",
                 OLEMISC_ALWAYSRUN |
                 OLEMISC_ACTIVATEWHENVISIBLE | 
                 OLEMISC_RECOMPOSEONRESIZE | 
                 OLEMISC_INSIDEOUT |
                 OLEMISC_SETCLIENTSITEFIRST |
                 OLEMISC_CANTLINKINSIDE );
      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,(DWORD)strlen(szTemp));
  
   RegCreateKeyEx(HKEY_CLASSES_ROOT,OBJECT_NAME,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      RegCreateKeyEx(keyHandle,"CurVer",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      sprintf(szTemp,OBJECT_NAME_V);
      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szTemp,(DWORD)strlen(szTemp));
  
   RegCreateKeyEx(HKEY_CLASSES_ROOT,OBJECT_NAME_V,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      RegCreateKeyEx(keyHandle,"CLSID",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);
      RegSetValueEx(keyHandle,NULL,0,REG_SZ,(BYTE *)szCLSID,(DWORD)strlen(szCLSID));
  



   return S_OK;
   }
  
  
   STDAPI DllUnregisterServer() {

   HRESULT rc = S_OK;
   HKEY keyHandle;
   char szCLSID[256];
   LPOLESTR oleString;
  
   StringFromCLSID(OBJECT_CLSID,&oleString);
   WideCharToMultiByte(CP_ACP,0,oleString,-1,szCLSID,256,0,0);
  
   rc = RegOpenKeyEx(HKEY_CLASSES_ROOT,"CLSID",0,KEY_CREATE_SUB_KEY,&keyHandle);

   rc = SHDeleteKey(keyHandle,szCLSID);

   rc = SHDeleteKey(HKEY_CLASSES_ROOT,OBJECT_NAME);

   rc = SHDeleteKey(HKEY_CLASSES_ROOT,OBJECT_NAME_V);

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
   HRESULT hres;
   *ppv = NULL; 
   TabletPC *pef = new TabletPC(punkOuter);
   hres = pef -> QueryInterface(riid,ppv);
   if ( !*ppv ) delete pef;
   return hres;
   } 
  
  
   long __stdcall Factory::LockServer(int fLock) { 
   return S_OK; 
   }