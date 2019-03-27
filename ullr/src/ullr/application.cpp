#include "ullrpch.h"
#include "application.h" 

#include "glad/glad.h"

namespace Ullr {

  Application::Application()
  {
    this->window = std::unique_ptr<Window>(Window::Create());

    // Bind the event callback for the window
    this->window->setEventCallback(ULLR_BIND_EVENT_FN(Application::OnEvent));
  }
  

  Application::~Application()
  {
  }

  void Application::OnEvent(Events::Event& e)
  {
    Events::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Events::WindowClosedEvent>(ULLR_BIND_EVENT_FN(Application::OnWindowClosed));
    dispatcher.Dispatch<Events::KeyPressedEvent>(ULLR_BIND_EVENT_FN(Application::OnKeyPressed));

    //UL_CORE_TRACE("Received event: {0}", e);

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

      for (Layer* layer : this->layerStack)
        layer->OnUpdate();

      this->window->Update();
    }
  }

  void Application::PushLayer(Layer* layer) {
    this->layerStack.PushLayer(layer);
  }

  void Application::PushOverlay(Layer* overlay) {
    this->layerStack.PushOverlay(overlay);
  }

  bool Application::OnWindowClosed(Events::WindowClosedEvent& e)
  {
    this->running = false;
    return true;
  }

  bool Application::OnKeyPressed(Events::KeyPressedEvent& e)
  {
    if (e.getKeyCode() == 256) // Hack for ESCAPE key
      this->running = false;

    if (e.getKeyCode() == 65) // A
      this->window->setWindowMode(WindowMode::WINDOWED);
    if (e.getKeyCode() == 66) // B
      this->window->setWindowMode(WindowMode::BORDERLESS);
    if (e.getKeyCode() == 67) // C
      this->window->setWindowMode(WindowMode::FULL_SCREEN, 1920, 1080);
    if (e.getKeyCode() == 68) // A
      this->window->setWindowMode(WindowMode::WINDOWED, 320, 240);

    return true;
  }

}
