#pragma once

#include "ullrpch.cpp"
#include "ullr/input/inputManager.h"

namespace Ullr::Input {

  class windowsInputManager : public InputManager
  {
  public:
    static void Init();
    static void Shutdown();

  protected:
    virtual bool isKeyPressed(int32 keyCode) override;
    virtual bool isMouseButtonPressed(int32 buttonCode)override;
    virtual float getMouseX() override;
    virtual float getMouseY() override;
    virtual std::pair<float, float> getMousePos() override;

  /*private:
    windowsInput() {}
    virtual ~windowsInput() {}*/
  };

}
