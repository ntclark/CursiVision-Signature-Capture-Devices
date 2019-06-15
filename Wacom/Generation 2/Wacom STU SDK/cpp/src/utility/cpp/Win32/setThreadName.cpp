#include <WacomGSS/setThreadName.hpp>
#include <WacomGSS/Win32/windows.hpp>

namespace WacomGSS
{

  // *** note:
  // Windows 10 introduces SetThreadDescription() but is not used here for
  // a) compatibility; and
  // b) it take wchar_t so a string conversion would be needed.
  //
  // It looks like the new API stores the string as part of the process. This means it can be
  // retrieved later, unlike the old mechanism (below) that requires the debugger to be
  // present at the time of the call.



  // http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx#3

  #pragma pack(push,8)
  struct ThreadNameInfo
  {
      DWORD  dwType;     // Must be 0x1000.
      LPCSTR szName;     // Pointer to name (in user addr space).
      DWORD  dwThreadID; // Thread ID (-1=caller thread).
      DWORD  dwFlags;    // Reserved for future use, must be zero.
  };
  #pragma pack(pop)

  const DWORD MS_VC_EXCEPTION = 0x406D1388;

  void setThreadName(uint32_t threadId, char const * threadName)
  {
    //if (::IsDebuggerPresent())
    {
      ThreadNameInfo threadNameInfo;
      threadNameInfo.dwType = 0x1000;
      threadNameInfo.szName = threadName;
      threadNameInfo.dwThreadID = threadId;
      threadNameInfo.dwFlags = 0;
      __try
      {
        ::RaiseException(MS_VC_EXCEPTION, 0, sizeof(threadNameInfo) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR const *>(&threadNameInfo));
      }
      __except (EXCEPTION_EXECUTE_HANDLER)
      {
      }
    }
  }
    

  void setThreadName(char const * threadName)
  {
    setThreadName(0xffffffff, threadName);
  }


} // namespace WacomGSS
