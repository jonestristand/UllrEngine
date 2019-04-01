#include "ullrpch.h"
#include "ullr/defines.h"

#include "imguiLayer.h"
#include "ullr/application.h"

#include "platform/opengl/imGuiOpenGLRenderer.h"
#include <GLFW/glfw3.h> // TODO: remove this
#include <glad/glad.h> // TODO: remove this

namespace Ullr {

  ImGuiLayer::ImGuiLayer()
    :Layer("ImGUILayer"), io(nullptr)
  { 
    //this->io = ImGui::GetIO();
  }


  ImGuiLayer::~ImGuiLayer()
  {
  }

  void ImGuiLayer::OnAttach()
  {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    this->io = &(ImGui::GetIO());
    this->io->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    this->io->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // TODO: Replace with own key codes
    // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
    this->io->KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    this->io->KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    this->io->KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    this->io->KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    this->io->KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    this->io->KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    this->io->KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    this->io->KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    this->io->KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    this->io->KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    this->io->KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    this->io->KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    this->io->KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    this->io->KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    this->io->KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    this->io->KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    this->io->KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    this->io->KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    this->io->KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    this->io->KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    this->io->KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

    ImGui_ImplOpenGL3_Init("#version 410");
  }

  void ImGuiLayer::OnDetach()
  {
    this->io = nullptr;
  }

  void ImGuiLayer::OnUpdate()
  {
    Application& app = Application::Get();

    this->io->DisplaySize = ImVec2(app.GetWindow().getWidth(), app.GetWindow().getHeight());

    float time = (float)glfwGetTime();
    this->io->DeltaTime = this->time > 0.0 ? (time - this->time) : (1.0f / 60.0f);
    this->time = time;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("FPS");
    if (showFPS)
      ImGui::Text("%.0f fps", this->io->Framerate);
    else
      ImGui::Text("%.2f ms/fr", 1000.0 /this->io->Framerate);
    if (ImGui::Button("Change")) {
      showFPS = !showFPS;
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void ImGuiLayer::OnEvent(Events::Event& event)
  {
    Events::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Events::MouseMovedEvent>(ULLR_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
    dispatcher.Dispatch<Events::MouseScrolledEvent>(ULLR_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
    dispatcher.Dispatch<Events::MouseButtonPressedEvent>(ULLR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
    dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(ULLR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
    dispatcher.Dispatch<Events::KeyTypedEvent>(ULLR_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
    dispatcher.Dispatch<Events::KeyPressedEvent>(ULLR_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<Events::KeyReleasedEvent>(ULLR_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
    dispatcher.Dispatch<Events::WindowResizedEvent>(ULLR_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
  }

  bool ImGuiLayer::OnMouseButtonPressedEvent(Events::MouseButtonPressedEvent& e)
  {
    this->io->MouseDown[e.getButton()] = true;
    return false;
  }

  bool ImGuiLayer::OnMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& e)
  {
    this->io->MouseDown[e.getButton()] = false;
    return false;
  }

  bool ImGuiLayer::OnMouseMovedEvent(Events::MouseMovedEvent& e)
  {
    this->io->MousePos = ImVec2(e.getX(), e.getY());
    return false;
  }

  bool ImGuiLayer::OnMouseScrolledEvent(Events::MouseScrolledEvent& e)
  {
    this->io->MouseWheel = e.getOffsetY();
    this->io->MouseWheelH = e.getOffsetX();
    return false;
  }

  bool ImGuiLayer::OnKeyTypedEvent(Events::KeyTypedEvent& e)
  {
    uint32 c = e.getKeyCode();
    if (c > 0 && c < 0x10000)
      this->io->AddInputCharacter((ImWchar)c);
    return false;
  }

  bool ImGuiLayer::OnKeyPressedEvent(Events::KeyPressedEvent& e)
  {
    this->io->KeysDown[e.getKeyCode()] = true;

    this->io->KeyCtrl = this->io->KeysDown[GLFW_KEY_LEFT_CONTROL] || this->io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
    this->io->KeyShift = this->io->KeysDown[GLFW_KEY_LEFT_SHIFT] || this->io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
    this->io->KeyAlt = this->io->KeysDown[GLFW_KEY_LEFT_ALT] || this->io->KeysDown[GLFW_KEY_RIGHT_ALT];
    this->io->KeySuper = this->io->KeysDown[GLFW_KEY_LEFT_SUPER] || this->io->KeysDown[GLFW_KEY_RIGHT_SUPER];

    return true;
  }

  bool ImGuiLayer::OnKeyReleasedEvent(Events::KeyReleasedEvent& e)
  {
    this->io->KeysDown[e.getKeyCode()] = false;
    return false;
  }

  bool ImGuiLayer::OnWindowResizedEvent(Events::WindowResizedEvent& e)
  {
    this->io->DisplaySize = ImVec2(e.getWidth(), e.getHeight());
    this->io->DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
    glViewport(0, 0, e.getWidth(), e.getHeight());
    return false;
  }

}