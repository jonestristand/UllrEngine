#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

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

#include "ullr/log.h"

#ifdef ULLR_PLATFORM_WINDOWS
  #include <windows.h>
#endif
