#include "ullrpch.h"

#include "application.h"

#include "utils/fpsImGuiLayer.h"

// TODO: THIS IS TEMPORARY
#include "input/keyCodes.h"
#include "glad/glad.h"

namespace Ullr {

  Application* Application::instance = nullptr;

  Application::Application()
  {
    UL_CORE_ASSERT(!Application::instance, "Application already exists!")
    Application::instance = this;

    this->window = std::unique_ptr<Window>(Window::Create());

    // Bind the event callback for the window
    this->window->setEventCallback(ULLR_BIND_EVENT_FN(Application::OnEvent));

    this->imguiLayer = new ImGuiLayer();
    this->PushOverlay(this->imguiLayer);

    this->PushOverlay(new Utils::FpsImGuiLayer());
  }


  Application::~Application()
  {
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
    while (this->running)
    {
      glClearColor(0.12f, 0.57f, 1.0f, 1);
      glClear(GL_COLOR_BUFFER_BIT);

      // TODO: This can likely all be done by the layer stack (LayerManager) itself, and not expose layers to the Application directly?
      for (Layer* layer : this->layerStack)
        layer->OnUpdate();

      // Render ImGui interface
      this->imguiLayer->Begin();

      for (Layer* layer : this->layerStack)
        layer->OnImGuiRender();

      this->imguiLayer->End();

      // Update the window
      this->window->Update();
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
    if (e.getKeyCode() == ULLR_KEY_ESCAPE) // Hack for ESCAPE key
      this->running = false;

    return true;
  }

}
