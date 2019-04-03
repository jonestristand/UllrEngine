#include "ullr.h"
/*
#include "imgui.h"
*/

class Sandbox : public Ullr::Application
{
public:
  Sandbox() { }
  ~Sandbox() { }
};

Ullr::Application* Ullr::CreateApplication()
{
  return new Sandbox();
}
