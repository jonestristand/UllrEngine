#include "ullrpch.h"
#include "ullr/defines.h"

#include "imguiLayer.h"

#define IMGUI_IMPL_API
#include "imgui.h"
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

#include "ullr/application.h"

// TODO: need to abstract this to platform-specific
#include <GLFW/glfw3.h>

namespace Ullr {

  ImGuiLayer::ImGuiLayer()
    :Layer("ImGUILayer"), io(nullptr)
  { }

  void ImGuiLayer::OnAttach()
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Enable docking, viewports, keyboard navigation
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
    // this->io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable multi-viewport
    // this->io->ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    // this->io->ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    // Set visual style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();
    
    // Tweak visual style to look good in native windows
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    Application& app = Application::Get();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().GetNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

    this->io = &io;
  }

  void ImGuiLayer::OnDetach()
  {
    this->io = nullptr; // Release pointer to ImGuiIO object, ImGui to manage lifecycle
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiLayer::OnImGuiRender()
  {
    //static bool show = true;
    //ImGui::ShowDemoWindow(&show);
  }

  void ImGuiLayer::Begin()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void ImGuiLayer::End()
  {
    // Update window size
    Application& app = Application::Get();
    io->DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());
    // Do actual render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (this->io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      auto backupCurrentContext = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backupCurrentContext);
    }
  }

}
