#pragma once

#include "ullr/defines.h"

namespace Ullr::Input {

  // Interface
  class InputManager
  {
  public:
    inline static bool IsKeyPressed(int32 keyCode) {
      UL_CORE_ASSERT(instance, "Input Manager not initialized");
      return instance->isKeyPressed(keyCode);
    }

    inline static bool IsMouseButtonPressed(int32 buttonCode) {
      UL_CORE_ASSERT(instance, "Input Manager not initialized");
      return instance->isMouseButtonPressed(buttonCode);
    }

    inline static float GetMouseX() {
      UL_CORE_ASSERT(instance, "Input Manager not initialized");
      return instance->getMouseX();
    }

    inline static float GetMouseY() {
      UL_CORE_ASSERT(instance, "Input Manager not initialized");
      return instance->getMouseY();
    }

    inline static std::pair<float, float> GetMousePos() {
      UL_CORE_ASSERT(instance, "Input Manager not initialized");
      return instance->getMousePos();
    }

  protected:
    virtual bool isKeyPressed(int32 keyCode) = 0;
    virtual bool isMouseButtonPressed(int32 buttonCode) = 0;
    virtual float getMouseX() = 0;
    virtual float getMouseY() = 0;
    virtual std::pair<float, float> getMousePos() = 0;

    static InputManager* instance;

  };
}
