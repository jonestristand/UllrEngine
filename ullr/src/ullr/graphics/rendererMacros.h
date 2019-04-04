#pragma once

#include "ullr/log.h"
//#define UL_CORE_TRACE(...) log(__VA_ARGS__)
// ==== Helper Macros ====
#define UL_RENDER_PASTE2(a, b) a ## b
#define UL_RENDER_PASTE(a, b) UL_RENDER_PASTE2(a, b)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)


// ============================================================================
// These macros wrap a code block in a struct that contains the arguments needed
// by that code block, and then pushes the struct onto the render command queue
// ============================================================================

// ==== Create and dispatch a function for FOUR ARGUMENTS ====
#define DISPATCH_RENDER_FN4(commandName, arg0, arg1, arg2, arg3, code) do {\
  typedef typename decltype(arg0) arg0Type;\
  typedef typename decltype(arg1) arg1Type;\
  typedef typename decltype(arg2) arg2Type;\
  typedef typename decltype(arg3) arg3Type;\
  struct UL_RENDER_PASTE(commandName, Command) {\
    UL_RENDER_PASTE(commandName, Command)(arg0Type arg0, arg1Type arg1, arg2Type arg2, arg3Type arg3)\
    : arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3)\
    {}\
\
    static void Execute(void* argBuffer) {\
      auto arg0 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg0;\
      auto arg1 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg1;\
      auto arg2 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg2;\
      auto arg3 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg3;\
      code\
      UL_CORE_TRACE("Render Queue >>::{0} ({1}, {2}, {3}, {4})", TOSTRING(UL_RENDER_PASTE(commandName, Command)), arg0, arg1, arg2, arg3);\
    }\
\
    arg0Type arg0;\
    arg1Type arg1;\
    arg2Type arg2;\
    arg3Type arg3;\
  };\
  auto mem = ::Ullr::Graphics::Renderer::Submit(UL_RENDER_PASTE(commandName, Command)::Execute, sizeof(UL_RENDER_PASTE(commandName, Command)));\
  new (mem) UL_RENDER_PASTE(commandName, Command)(arg0, arg1, arg2, arg3);\
} while (0)

// ==== Create and dispatch a function for THREE ARGUMENTS ====
#define DISPATCH_RENDER_FN3(commandName, arg0, arg1, arg2, code) do {\
  typedef typename decltype(arg0) arg0Type;\
  typedef typename decltype(arg1) arg1Type;\
  typedef typename decltype(arg2) arg2Type;\
  struct UL_RENDER_PASTE(commandName, Command) {\
    UL_RENDER_PASTE(commandName, Command)(arg0Type arg0, arg1Type arg1, arg2Type arg2)\
    : arg0(arg0), arg1(arg1), arg2(arg2)\
    {}\
\
    static void Execute(void* argBuffer) {\
      auto arg0 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg0;\
      auto arg1 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg1;\
      auto arg2 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg2;\
      code\
      UL_CORE_TRACE("Render Queue >>::{0} ({1}, {2}, {3})", TOSTRING(UL_RENDER_PASTE(commandName, Command)), arg0, arg1, arg2);\
    }\
\
    arg0Type arg0;\
    arg1Type arg1;\
    arg2Type arg2;\
  };\
  auto mem = ::Ullr::Graphics::Renderer::Submit(UL_RENDER_PASTE(commandName, Command)::Execute, sizeof(UL_RENDER_PASTE(commandName, Command)));\
  new (mem) UL_RENDER_PASTE(commandName, Command)(arg0, arg1, arg2);\
} while (0)

// ==== Create and dispatch a function for TWO ARGUMENTS ====
#define DISPATCH_RENDER_FN2(commandName, arg0, arg1, code) do {\
  typedef typename decltype(arg0) arg0Type;\
  typedef typename decltype(arg1) arg1Type;\
  struct UL_RENDER_PASTE(commandName, Command) {\
    UL_RENDER_PASTE(commandName, Command)(arg0Type arg0, arg1Type arg1)\
    : arg0(arg0), arg1(arg1)\
    {}\
\
    static void Execute(void* argBuffer) {\
      auto arg0 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg0;\
      auto arg1 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg1;\
      code\
      UL_CORE_TRACE("Render Queue >>::{0} ({1}, {2})", TOSTRING(UL_RENDER_PASTE(commandName, Command)), arg0, arg1);\
    }\
\
    arg0Type arg0;\
    arg1Type arg1;\
  };\
  auto mem = ::Ullr::Graphics::Renderer::Submit(UL_RENDER_PASTE(commandName, Command)::Execute, sizeof(UL_RENDER_PASTE(commandName, Command)));\
  new (mem) UL_RENDER_PASTE(commandName, Command)(arg0, arg1);\
} while (0)

// ==== Create and dispatch a function for ONE ARGUMENT ====
#define DISPATCH_RENDER_FN1(commandName, arg0, code) do {\
  typedef typename decltype(arg0) arg0Type;\
  struct UL_RENDER_PASTE(commandName, Command) {\
    UL_RENDER_PASTE(commandName, Command)(arg0Type arg0)\
    : arg0(arg0)\
    {}\
\
    static void Execute(void* argBuffer) {\
      auto arg0 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg0;\
      code\
      UL_CORE_TRACE("Render Queue >>::{0} ({1})", TOSTRING(UL_RENDER_PASTE(commandName, Command)), arg0);\
    }\
\
    arg0Type arg0;\
  };\
  auto mem = ::Ullr::Graphics::Renderer::Submit(UL_RENDER_PASTE(commandName, Command)::Execute, sizeof(UL_RENDER_PASTE(commandName, Command)));\
  new (mem) UL_RENDER_PASTE(commandName, Command)(arg0);\
} while (0)

// ==== Create and dispatch a function with NO ARGUMENTS ====
#define DISPATCH_RENDER_FN(commandName, code) do {\
  struct UL_RENDER_PASTE(commandName, Command) {\
    UL_RENDER_PASTE(commandName, Command)()\
    {}\
\
    static void Execute(void* argBuffer) {\
      code\
      UL_CORE_TRACE("Render Queue >>::{0} ()", TOSTRING(UL_RENDER_PASTE(commandName, Command)));\
    }\
\
  };\
  auto mem = ::Ullr::Graphics::Renderer::Submit(UL_RENDER_PASTE(commandName, Command)::Execute, sizeof(UL_RENDER_PASTE(commandName, Command)));\
  new (mem) UL_RENDER_PASTE(commandName, Command)();\
} while (0)

// ==== Create and dispatch a function for THREE ARGUMENTS with a SELF POINTER ====
#define DISPATCH_RENDER_SELF_FN3(commandName, arg0, arg1, arg2, code) auto self = this;\
do {\
  typedef typename decltype(self) selfType;\
  typedef typename decltype(arg0) arg0Type;\
  typedef typename decltype(arg1) arg1Type;\
  typedef typename decltype(arg2) arg2Type;\
  struct UL_RENDER_PASTE(commandName, Command) {\
    UL_RENDER_PASTE(commandName, Command)(selfType self, arg0Type arg0, arg1Type arg1, arg2Type arg2)\
    : self(self), arg0(arg0), arg1(arg1), arg2(arg2)\
    {}\
\
    static void Execute(void* argBuffer) {\
      auto self = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->self;\
      auto arg0 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg0;\
      auto arg1 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg1;\
      auto arg2 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg2;\
      code\
      UL_CORE_TRACE("Render Queue >>::{0} ({1}, {2}, {3})", TOSTRING(UL_RENDER_PASTE(commandName, Command)), arg0, arg1, arg2);\
    }\
\
    selfType self;\
    arg0Type arg0;\
    arg1Type arg1;\
    arg2Type arg2;\
  };\
  auto mem = ::Ullr::Graphics::Renderer::Submit(UL_RENDER_PASTE(commandName, Command)::Execute, sizeof(UL_RENDER_PASTE(commandName, Command)));\
  new (mem) UL_RENDER_PASTE(commandName, Command)(self, arg0, arg1, arg2);\
} while (0)

// ==== Create and dispatch a function for TWO ARGUMENTS with a SELF POINTER ====
#define DISPATCH_RENDER_SELF_FN2(commandName, arg0, arg1, code) auto self = this;\
do {\
  typedef typename decltype(self) selfType;\
  typedef typename decltype(arg0) arg0Type;\
  typedef typename decltype(arg1) arg1Type;\
  struct UL_RENDER_PASTE(commandName, Command) {\
    UL_RENDER_PASTE(commandName, Command)(selfType self, arg0Type arg0, arg1Type arg1)\
    : self(self), arg0(arg0), arg1(arg1)\
    {}\
\
    static void Execute(void* argBuffer) {\
      auto self = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->self;\
      auto arg0 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg0;\
      auto arg1 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg1;\
      code\
      UL_CORE_TRACE("Render Queue >>::{0} ({1}, {2})", TOSTRING(UL_RENDER_PASTE(commandName, Command)), arg0, arg1);\
    }\
\
    selfType self;\
    arg0Type arg0;\
    arg1Type arg1;\
  };\
  auto mem = ::Ullr::Graphics::Renderer::Submit(UL_RENDER_PASTE(commandName, Command)::Execute, sizeof(UL_RENDER_PASTE(commandName, Command)));\
  new (mem) UL_RENDER_PASTE(commandName, Command)(self, arg0, arg1);\
} while (0)

// ==== Create and dispatch a function for ONE ARGUMENT with a SELF POINTER ====
#define DISPATCH_RENDER_SELF_FN1(commandName, arg0, code) auto self = this;\
do {\
  typedef typename decltype(self) selfType;\
  typedef typename decltype(arg0) arg0Type;\
  struct UL_RENDER_PASTE(commandName, Command) {\
    UL_RENDER_PASTE(commandName, Command)(selfType self, arg0Type arg0)\
    : self(self), arg0(arg0)\
    {}\
\
    static void Execute(void* argBuffer) {\
      auto self = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->self;\
      auto arg0 = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->arg0;\
      code\
      UL_CORE_TRACE("Render Queue >>::{0} ({1})", TOSTRING(UL_RENDER_PASTE(commandName, Command)), arg0);\
    }\
\
    selfType self;\
    arg0Type arg0;\
  };\
  auto mem = ::Ullr::Graphics::Renderer::Submit(UL_RENDER_PASTE(commandName, Command)::Execute, sizeof(UL_RENDER_PASTE(commandName, Command)));\
  new (mem) UL_RENDER_PASTE(commandName, Command)(self, arg0);\
} while (0)

// ==== Create and dispatch a function for NO ARGUMENTS with a SELF POINTER ====
#define DISPATCH_RENDER_SELF_FN(commandName, code) auto self = this;\
do {\
  typedef typename decltype(self) selfType;\
  struct UL_RENDER_PASTE(commandName, Command) {\
    UL_RENDER_PASTE(commandName, Command)(selfType self)\
    : self(self)\
    {}\
\
    static void Execute(void* argBuffer) {\
      auto self = ((UL_RENDER_PASTE(commandName, Command)*)argBuffer)->self;\
      code\
      UL_CORE_TRACE("Render Queue >>::{0} ()", TOSTRING(UL_RENDER_PASTE(commandName, Command)));\
    }\
\
    selfType self;\
  };\
  auto mem = ::Ullr::Graphics::Renderer::Submit(UL_RENDER_PASTE(commandName, Command)::Execute, sizeof(UL_RENDER_PASTE(commandName, Command)));\
  new (mem) UL_RENDER_PASTE(commandName, Command)(self);\
} while (0)
