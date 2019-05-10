#include "ullrpch.h"

#include "windowsInputManager.h"
#include "ullr/application.h"

#include <GLFW/glfw3.h>

namespace Ullr::Input {

  InputManager* InputManager::instance;

  void windowsInputManager::Init()
  {
    InputManager::instance = new windowsInputManager();
  }

  void windowsInputManager::Shutdown()
  {
    delete InputManager::instance;
  }

  void windowsInputManager::captureMouse(bool capture) {
    auto window = (GLFWwindow*)Application::Get().getWindow().GetNativeWindow();
    if (capture) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    else
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
    }
  }

  bool windowsInputManager::isKeyPressed(int32 keyCode) {
    auto window = (GLFWwindow*)Application::Get().getWindow().GetNativeWindow();
    auto state = glfwGetKey(window, keyCode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool windowsInputManager::isMouseButtonPressed(int32 buttonCode)
  {
    auto window = (GLFWwindow*)Application::Get().getWindow().GetNativeWindow();
    auto state = glfwGetMouseButton(window, buttonCode);
    return state == GLFW_PRESS;
  }

  float windowsInputManager::getMouseX()
  {
    auto[x, y] = this->getMousePos();

    return x;
  }

  float windowsInputManager::getMouseY()
  {
    auto[x, y] = this->getMousePos();

    return y;
  }

  std::pair<float, float> windowsInputManager::getMousePos()
  {
    auto window = (GLFWwindow*)Application::Get().getWindow().GetNativeWindow();
    double xpos, ypos;

    glfwGetCursorPos(window, &xpos, &ypos);
    return { (float)xpos, (float)ypos };
  }

}
