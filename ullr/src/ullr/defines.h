#pragma once

#define S(x) #x
#define SS(x) S(x)

#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define REVISION 0
#define VERSION_STRING SS(MAJOR_VERSION.MINOR_VERSION.REVISION)

#define OGL_MAJOR 4
#define OGL_MINOR 6

#ifdef UL_DYNAMIC_LINK
  #if defined ULLR_PLATFORM_WINDOWS || defined __CYGWIN__
    #define ULLR_API_DLL_IMPORT __declspec(dllimport)
    #define ULLR_API_DLL_EXPORT __declspec(dllexport)
    #define ULLR_API_DLL_LOCAL
  #else
    #if __GNUC__ >= 4
      #define ULLR_API_DLL_IMPORT __attribute__ ((visibility ("default")))
      #define ULLR_API_DLL_EXPORT __attribute__ ((visibility ("default")))
      #define ULLR_API_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #else
      #define ULLR_API_DLL_IMPORT
      #define ULLR_API_DLL_EXPORT
      #define ULLR_API_DLL_LOCAL
    #endif
  #endif

  #ifdef ULLR_BUILD_DLL // defined if Ullr is compiled as a DLL
    #define ULLR_API ULLR_API_DLL_EXPORT
  #else
    #define ULLR_API ULLR_API_DLL_IMPORT
  #endif
  #define ULLR_LOCAL ULLR_API_DLL_LOCAL
#else
  #define ULLR_API
  #define ULLR_LOCAL
#endif

#ifdef UL_ENABLE_ASSERTS
  #ifdef ULLR_PLATFORM_WINDOWS
    #define UL_DEBUG_STOP() __debugbreak()
  #else
    #include <signal.h>
    #define UL_DEBUG_STOP()  raise(SIGTRAP)
  #endif

  #define UL_ASSERT(x, ...) { if(!(x)) { UL_FATAL("Assertion Failed: {0}", __VA_ARGS__); UL_DEBUG_STOP(); } }
  #define UL_CORE_ASSERT(x, ...) { if(!(x)) { UL_CORE_FATAL("Assertion Failed: {0}", __VA_ARGS__); UL_DEBUG_STOP(); } }
#else
  #define UL_ASSERT(x, ...)
  #define UL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define ULLR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
