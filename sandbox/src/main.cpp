#include "ullr.h"

class TristanLayer : public Ullr::Layer
{

public:
  TristanLayer()
    :Layer("Tristan Layer") {}

  void OnUpdate() override
  {
  }

  void OnEvent(Ullr::Events::Event& e) override {
    //UL_TRACE("{0}", e);
  }

};

class Sandbox : public Ullr::Application
{
public:
  Sandbox()
  {
    this->PushLayer(new TristanLayer());
    this->PushOverlay(new Ullr::ImGuiLayer());
  }
  ~Sandbox() { }
};

Ullr::Application* Ullr::CreateApplication()
{
  return new Sandbox();
}
