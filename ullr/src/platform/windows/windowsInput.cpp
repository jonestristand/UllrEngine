#include "ullrpch.h"

#include "windowsInput.h"
#include "ullr/application.h"

#include <GLFW/glfw3.h>

namespace Ullr::Input {

  InputManager* InputManager::instance;

  void windowsInput::Init()
  {
    InputManager::instance = new windowsInput();
  }

  void windowsInput::Shutdown()
  {
    delete InputManager::instance;
  }

  bool windowsInput::isKeyPressed(int keyCode) {
    auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    auto state = glfwGetKey(window, keyCode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool windowsInput::isMouseButtonPressed(int buttonCode)
  {
    auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    auto state = glfwGetMouseButton(window, buttonCode);
    return state == GLFW_PRESS;
  }

  float windowsInput::getMouseX()
  {
    auto[x, y] = this->getMousePos();

    return x;
  }

  float windowsInput::getMouseY()
  {
    auto[x, y] = this->getMousePos();

    return y;
  }

  std::pair<float, float> windowsInput::getMousePos()
  {
    auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
    double xpos, ypos;

    glfwGetCursorPos(window, &xpos, &ypos);
    return { (float)xpos, (float)ypos };
  }

}
