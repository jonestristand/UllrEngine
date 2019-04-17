#include "ullrpch.h"

#include "application.h"
#include "window.h"
#include "graphics/renderManager.h"

#include "utils/fpsImGuiLayer.h"
#include "graphics/command/imguiCommands.hpp"

// TODO: THIS IS TEMPORARY
#include "input/keyCodes.h"
#include <GLFW/glfw3.h>


namespace Ullr {

  Application* Application::instance = nullptr;

  Application::Application()
  {
    UL_CORE_ASSERT(!Application::instance, "Application already exists!")
    Application::instance = this;

    this->window = std::unique_ptr<Window>(Window::Create(Ullr::WindowProps(
    "UllrEngine",
      1920,
      1080,
      Ullr::WindowMode::WINDOWED,
      false)));

    // Bind the event callback for the window
    this->window->setEventCallback(ULLR_BIND_EVENT_FN(Application::OnEvent));

    // Create managers
    this->renderManager = Ullr::Graphics::RenderManager::Create();
    this->renderManager->Init();

    // Create Imgui layers
    this->imguiLayer = new ImGuiLayer();
    this->PushOverlay(this->imguiLayer);

    this->PushOverlay(new Utils::FpsImGuiLayer());
  }


  Application::~Application()
  {
    this->renderManager->Shutdown();
  }

  void Application::OnEvent(Events::Event& e)
  {
    Events::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Events::WindowClosedEvent>(ULLR_BIND_EVENT_FN(Application::OnWindowClosed));
    dispatcher.Dispatch<Events::KeyPressedEvent>(ULLR_BIND_EVENT_FN(Application::OnKeyPressed));

    for (auto it = this->layerStack.end(); it != this->layerStack.begin(); ) {
      (*--it)->OnEvent(e);
      if (e.Handled) // Prevent layers underneath from receiving handled events
        break;
    }
  }

  void Application::Run()
  {
    this->renderManager->SetClearColor(0.1f, 0.1f, 0.1f);

    while (this->running)
    {
      this->renderManager->ClearBuffer();

      // TODO: This can likely all be done by the layer stack (LayerManager) itself, and not expose layers to the Application directly?
      for (Layer* layer : this->layerStack)
        layer->OnUpdate();

      // Submit RenderImGUICommand
      Graphics::Command::RenderImGUI::Dispatch(this);

      this->renderManager->Render();

      // Update the window
      this->window->Update();

      // Update the timer
      this->time = glfwGetTime();
    }

  }

  void Application::PushLayer(Layer* layer) {
    this->layerStack.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* overlay) {
    this->layerStack.PushOverlay(overlay);
    overlay->OnAttach();
  }

  bool Application::OnWindowClosed(Events::WindowClosedEvent& e)
  {
    this->running = false;
    return true;
  }

  bool Application::OnKeyPressed(Events::KeyPressedEvent& e)
  {
    if (e.getKeyCode() == ULLR_KEY_ESCAPE) {
      this->running = false;
      return true;
    }

    return false; // Pass through other key events
  }

}
