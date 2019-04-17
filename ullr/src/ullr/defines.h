#pragma once

#include <cstdint>

// Custom types
typedef uint8_t   byte;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;

typedef int8_t    sbyte;
typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;

#define S(x) #x
#define SS(x) S(x)

#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define REVISION 0
#define VERSION_STRING SS(MAJOR_VERSION.MINOR_VERSION.REVISION)

#define OGL_MAJOR 4
#define OGL_MINOR 6

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
