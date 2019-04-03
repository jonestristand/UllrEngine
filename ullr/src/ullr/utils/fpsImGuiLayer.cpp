#include "ullrpch.h"

#include "fpsImGuiLayer.h"

namespace Ullr::Utils {

  FpsImGuiLayer::FpsImGuiLayer()
    : Layer("FPS Layer"), showFPS(true), io(nullptr) {}

  void FpsImGuiLayer::OnAttach()
  {
    this->io = &(ImGui::GetIO());
  }

  void FpsImGuiLayer::OnDetach()
  {
    this->io = nullptr; // Stop pointing to ImGuiIO - ImGui to manage lifetime of object
  }

  void FpsImGuiLayer::OnImGuiRender()
  {
    if (this->io) {
      ImGui::Begin("FPS");

      // Show FPS or Frame time counter
      if (this->showFPS)
        ImGui::Text("%.1f fps", this->io->Framerate);
      else
        ImGui::Text("%.2f ms/fr", 1000.0f / this->io->Framerate);

      // Button to toggle view
      const char* ButtonText = this->showFPS ? "Show ms/fr" : "Show FPS";
      if (ImGui::Button(ButtonText))
        this->showFPS = !this->showFPS;

      ImGui::End();
    }
  }

}
