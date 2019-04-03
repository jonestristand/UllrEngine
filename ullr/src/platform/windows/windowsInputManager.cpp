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

  bool windowsInputManager::isKeyPressed(int keyCode) {
    auto window = (GLFWwindow*)Application::Get().getWindow().GetNativeWindow();
    auto state = glfwGetKey(window, keyCode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool windowsInputManager::isMouseButtonPressed(int buttonCode)
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
