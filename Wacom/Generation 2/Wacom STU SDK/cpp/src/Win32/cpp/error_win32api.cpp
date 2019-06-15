#include <WacomGSS/Win32/windows.hpp>
#include <WacomGSS/Win32/wstringConvert.hpp>
#include <sstream>
#include <array>
#include <system_error>

namespace WacomGSS
{
  namespace Win32
  {

    class win32api_error_category_impl : public std::error_category
    {
    public:
      char const * name() const noexcept override
      {
        return "win32api";
      }
      
      std::string message(int ev) const override;

      std::error_condition default_error_condition(int ev) const noexcept override;

      bool equivalent(int ev, std::error_condition const & condition) const noexcept override;      
      bool equivalent(std::error_code const &, int) const noexcept override;
    };
    

    static bool messageFrom(int ev, PWSTR & pBuffer, wchar_t const * dll)
    {
      HMODULE hModule = GetModuleHandle(dll);
      if (hModule)
      {
        DWORD dw = ::FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
          hModule,
          static_cast<DWORD>(ev),
          0,
          reinterpret_cast<PWSTR>(&pBuffer),
          0,
          nullptr);
        return dw != 0;
      }
      return false;
    }

    std::string win32api_error_category_impl::message(int ev) const
    {    
      PWSTR pBuffer = nullptr;      
      try
      {
        std::wstring wret;

        DWORD dw = ::FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
          nullptr,
          static_cast<DWORD>(ev),
          0,
          reinterpret_cast<PWSTR>(&pBuffer),
          0,
          nullptr);
        if (dw != 0)
        {
          wret = pBuffer;
        }
        else
        {
          std::wstringstream o;
          if (messageFrom(ev, pBuffer, L"NTDLL.DLL"))
          {
            o << L"[NTDLL]: " << pBuffer;
          }
          else if (messageFrom(ev, pBuffer, L"WININET.DLL"))
          {
            o << L"[WinInet]: " << pBuffer;
          }
          else            
          {
            o << static_cast<DWORD>(ev);
          }
          wret = o.str();
        }

        if (pBuffer)
        {
          ::LocalFree(pBuffer);
          pBuffer = nullptr;
        }

        std::string ret;
        {
          ret = Win32::wstring_to_string(wret);
        }
        return ret;
      }
      catch (...)
      {
        if (pBuffer)
        {
          ::LocalFree(pBuffer);
        }
        throw;
      }
    }



    template<typename First, typename Second>
    struct pair
    {
      First  first;
      Second second;
    };

#if defined(_MSC_VER) && _MSC_VER < 1800
#define WacomGSS_std_errc std::errc::errc
#else
#define WacomGSS_std_errc std::errc
#endif
    static const pair<int, WacomGSS_std_errc> map[] =
    {
      {  ERROR_INVALID_FUNCTION       , std::errc::invalid_argument               },  /* 1 */
      {  ERROR_FILE_NOT_FOUND         , std::errc::no_such_file_or_directory      },  /* 2 */
      {  ERROR_PATH_NOT_FOUND         , std::errc::no_such_file_or_directory      },  /* 3 */
      {  ERROR_TOO_MANY_OPEN_FILES    , std::errc::too_many_files_open            },  /* 4 */
      {  ERROR_ACCESS_DENIED          , std::errc::permission_denied              },  /* 5 */
      {  ERROR_INVALID_HANDLE         , std::errc::bad_file_descriptor            },  /* 6 */
      {  ERROR_ARENA_TRASHED          , std::errc::not_enough_memory              },  /* 7 */
      {  ERROR_NOT_ENOUGH_MEMORY      , std::errc::not_enough_memory              },  /* 8 */
      {  ERROR_INVALID_BLOCK          , std::errc::not_enough_memory              },  /* 9 */
      {  ERROR_BAD_ENVIRONMENT        , std::errc::argument_list_too_long         },  /* 10 */
      {  ERROR_BAD_FORMAT             , std::errc::executable_format_error        },  /* 11 */
      {  ERROR_INVALID_ACCESS         , std::errc::invalid_argument               },  /* 12 */
      {  ERROR_INVALID_DATA           , std::errc::invalid_argument               },  /* 13 */
      {  ERROR_INVALID_DRIVE          , std::errc::no_such_file_or_directory      },  /* 15 */
      {  ERROR_CURRENT_DIRECTORY      , std::errc::permission_denied              },  /* 16 */
      {  ERROR_NOT_SAME_DEVICE        , std::errc::cross_device_link              },  /* 17 */
      {  ERROR_NO_MORE_FILES          , std::errc::no_such_file_or_directory      },  /* 18 */
      {  ERROR_LOCK_VIOLATION         , std::errc::permission_denied              },  /* 33 */
      {  ERROR_BAD_NETPATH            , std::errc::no_such_file_or_directory      },  /* 53 */
      {  ERROR_NETWORK_ACCESS_DENIED  , std::errc::permission_denied              },  /* 65 */
      {  ERROR_BAD_NET_NAME           , std::errc::no_such_file_or_directory      },  /* 67 */
      {  ERROR_FILE_EXISTS            , std::errc::file_exists                    },  /* 80 */
      {  ERROR_CANNOT_MAKE            , std::errc::permission_denied              },  /* 82 */
      {  ERROR_FAIL_I24               , std::errc::permission_denied              },  /* 83 */
      {  ERROR_INVALID_PARAMETER      , std::errc::invalid_argument               },  /* 87 */
      {  ERROR_NO_PROC_SLOTS          , std::errc::resource_unavailable_try_again },  /* 89 */
      {  ERROR_DRIVE_LOCKED           , std::errc::permission_denied              },  /* 108 */
      {  ERROR_BROKEN_PIPE            , std::errc::broken_pipe                    },  /* 109 */
      {  ERROR_DISK_FULL              , std::errc::no_space_on_device             },  /* 112 */
      {  ERROR_INVALID_TARGET_HANDLE  , std::errc::bad_file_descriptor            },  /* 114 */
      {  ERROR_INVALID_HANDLE         , std::errc::invalid_argument               },  /* 124 */
      {  ERROR_WAIT_NO_CHILDREN       , std::errc::no_child_process               },  /* 128 */
      {  ERROR_CHILD_NOT_COMPLETE     , std::errc::no_child_process               },  /* 129 */
      {  ERROR_DIRECT_ACCESS_HANDLE   , std::errc::bad_file_descriptor            },  /* 130 */
      {  ERROR_NEGATIVE_SEEK          , std::errc::invalid_argument               },  /* 131 */
      {  ERROR_SEEK_ON_DEVICE         , std::errc::permission_denied              },  /* 132 */
      {  ERROR_DIR_NOT_EMPTY          , std::errc::directory_not_empty            },  /* 145 */
      {  ERROR_NOT_LOCKED             , std::errc::permission_denied              },  /* 158 */
      {  ERROR_BAD_PATHNAME           , std::errc::no_such_file_or_directory      },  /* 161 */
      {  ERROR_MAX_THRDS_REACHED      , std::errc::resource_unavailable_try_again },  /* 164 */
      {  ERROR_LOCK_FAILED            , std::errc::permission_denied              },  /* 167 */
      {  ERROR_ALREADY_EXISTS         , std::errc::file_exists                    },  /* 183 */
      {  ERROR_FILENAME_EXCED_RANGE   , std::errc::no_such_file_or_directory      },  /* 206 */
      {  ERROR_NESTING_NOT_ALLOWED    , std::errc::resource_unavailable_try_again },  /* 215 */
      {  ERROR_NOT_ENOUGH_QUOTA       , std::errc::not_enough_memory              }   /* 1816 */
    };

    std::error_condition win32api_error_category_impl::default_error_condition(int ev) const noexcept
    {
      auto map_begin = map;
      auto map_end   = map + _countof(map);

      for (auto i = map_begin; i != map_end && i->first <= ev; ++i)
      {
        if (i->first == ev)
          return i->second;
      }
      return std::error_condition(ev, *this);
    }



    bool win32api_error_category_impl::equivalent(int ev, std::error_condition const & condition) const noexcept
    {
      switch (ev)
      {
        case ERROR_FILE_NOT_FOUND:
          if (condition == std::errc::no_such_device_or_address ||
              condition == std::errc::no_such_device ||
              condition == std::errc::no_such_file_or_directory)
            return true;

      }
      return default_error_condition(ev) == condition;
    }



    bool win32api_error_category_impl::equivalent(std::error_code const & code, int condition) const noexcept
    {
      return *this == code.category() && code.value() == condition;
    }
    


    static win32api_error_category_impl ec;

    std::error_category const & win32api_error_category() noexcept
    {      
      return ec;
    }

  }
}
