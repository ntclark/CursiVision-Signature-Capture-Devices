/// @file      delayLoadWGSS.cpp
/// @copyright Copyright(c) 2018 Wacom Co., Ltd.
/// @author    kparkes
/// @date      2018-11-14
/// @brief     Delay load hook function for loading from %CommonFiles%\WacomGSS

#include <WacomGSS/Win32/windows.hpp>

#pragma warning(push, 3)
#pragma warning(disable: 4191 5039)

#include <delayimp.h>

#include <tchar.h>
#include <strsafe.h>
#include <Shlobj.h>
#include <memory>

#include <atlbase.h>
#pragma warning(pop)

namespace WacomGSS {

  namespace Win32 {

    FARPROC WINAPI dliHook(unsigned dliNotify, PDelayLoadInfo pdli)
    {
      ATLTRACE(L"DliHook::hookFn(%x, %S)\n", dliNotify, pdli->szDll ? pdli->szDll : "<null>");
      if (dliNotify == dliNotePreLoadLibrary) // capture the LoadLibrary call only
      {
        try
        {
          WCHAR  fullPath[4096];
          LPWSTR pszPath = NULL;

          if (FAILED(SHGetKnownFolderPath(FOLDERID_ProgramFilesCommon, 0, NULL, &pszPath))) {
            return NULL;
          }
          HRESULT hr = StringCbPrintf(fullPath, sizeof(fullPath), L"%s\\WacomGSS\\%S", pszPath, pdli->szDll);
          CoTaskMemFree(pszPath);
          if (FAILED(hr)) {
            return NULL;
          }

          // Attempt explicit load of DLL.
          HMODULE hmod = LoadLibraryExW(fullPath, nullptr, 0);
          ATLTRACE(L"LoadLibraryExW('%s') = 0x%x. GLE=%d\n", fullPath, hmod, GetLastError());
          return reinterpret_cast<FARPROC>(hmod);
        }
        catch (...)
        {
          // Should never happen
          ATLTRACE(L"dliHook exception\n");
        }
      }
      return NULL;
    }


  }

}
