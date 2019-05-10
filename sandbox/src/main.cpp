#include "ullr.h"

#include "imgui.h"

class MainLayer : public Ullr::Layer
{
public:
  MainLayer();
  ~MainLayer();

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate() override;

  void OnImGuiRender() override;

  void UpdateInput();

private:
  ImGuiIO* io;

  std::unique_ptr<Ullr::Graphics::Shader> modelShader;
  std::unique_ptr<Ullr::Graphics::Shader> boneShader;
  std::unique_ptr<Ullr::Graphics::Shader> lampShader;
  //Ullr::Graphics::Model* nanosuit;
  Ullr::Graphics::Model* mutant;
  Ullr::Graphics::Model* mutant2;
  Ullr::Graphics::Model* lamp;

  glm::vec3 lightPos;
  glm::vec3 lightDiffuse;
  glm::vec3 lightAmbient;
  glm::vec3 lightSpecular;
  float shininess;

  bool showNormal;

  Ullr::Graphics::Camera camera;
  glm::mat4 projection;

  float mouseSens;

};

MainLayer::MainLayer()
  : Layer("Main Layer"), io(nullptr),
  lightPos(0.0f, 10.0f, 0.0f), lightDiffuse(1.0f, 1.0f, 1.0f), lightAmbient(0.5f, 0.5f, 0.5f), lightSpecular(1.0f, 1.0f, 1.0f), shininess(32.0f), showNormal(true),
  camera(glm::vec3(0.0f, 8.0f, 20.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
  /*mutant("assets/models/mutant/mutant.dae"), nanosuit("assets/models/nanosuit/nanosuit.obj"), lamp("assets/models/lamp/lamp.obj"),*/
  mouseSens(0.1f)
{
    auto loader = new Ullr::Graphics::Loader::AssimpModelLoader();

    //this->nanosuit = loader->LoadFromFile("assets/models/nanosuit/nanosuit.obj");
    this->mutant = loader->LoadFromFile("assets/models/mutant/mutant.dae");
    this->mutant2 = loader->LoadFromFile("assets/models/shepherd/shepherd.fbx");
    this->lamp = loader->LoadFromFile("assets/models/lamp/lamp.obj");

    delete loader;

  // TODO: application should manage active camera(s) and update their perspective matrix on window resize events
  this->projection = glm::perspective(glm::radians(60.0f), (float)Ullr::Application::Get().getWindow().getWidth() / (float)Ullr::Application::Get().getWindow().getHeight(), 0.1f, 1000.0f);

  this->boneShader = std::unique_ptr<Ullr::Graphics::Shader>(Ullr::Graphics::Shader::CreateShader("assets/shaders/main_tex_skel.glsl"));
  this->modelShader = std::unique_ptr<Ullr::Graphics::Shader>(Ullr::Graphics::Shader::CreateShader("assets/shaders/main_tex.glsl"));
  this->lampShader = std::unique_ptr<Ullr::Graphics::Shader>(Ullr::Graphics::Shader::CreateShader("assets/shaders/lamp.glsl"));

  this->boneShader->CompileAndLoad();
  this->modelShader->CompileAndLoad();
  this->lampShader->CompileAndLoad();

  this->mutant->setTranslation(glm::vec3(-10.0f, 0.0f, 0.0f));
  //this->mutant->setRotation(glm::vec3(1.0, 0.0, 0.0), glm::radians(-90.0f));
  this->mutant->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
  this->mutant2->setTranslation(glm::vec3(10.0f, 0.0f, 0.0f));
  //this->mutant2->setRotation(glm::vec3(1.0, 0.0, 0.0), glm::radians(-90.0f));
  this->mutant2->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

  //this->nanosuit->setTranslation(glm::vec3(-10.0f, 0.0f, 0.0f));

  this->lamp->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
}

MainLayer::~MainLayer()
{
  delete this->mutant2;
}

void MainLayer::OnAttach()
{
  this->io = &(ImGui::GetIO());
}

void MainLayer::OnDetach()
{
  this->io = nullptr; // Stop pointing to ImGuiIO - ImGui to manage lifetime of object
  delete this->mutant2;
}

void MainLayer::OnUpdate()
{
  // Update the input
  this->UpdateInput();

  auto view = this->camera.getViewMatrix();

  // Update models
  //this->nanosuit->setRotation(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(10.0f * (float)Ullr::Application::Get().getTime()));
  this->lightPos = glm::vec3(0.0f, 10.0f, 15.0f * glm::sin(1.5f * Ullr::Application::Get().getTime()));
  this->lamp->setTranslation(this->lightPos);

  this->modelShader->SetUniform("viewPos", this->camera.getPosition());
  this->boneShader->SetUniform("viewPos", this->camera.getPosition());

  // Set light uniforms
  this->modelShader->SetUniform("light.position", this->lightPos);
  this->modelShader->SetUniform("light.ambient", this->lightAmbient);
  this->modelShader->SetUniform("light.diffuse", this->lightDiffuse);
  this->modelShader->SetUniform("light.specular", this->lightSpecular);

  this->boneShader->SetUniform("light.position", this->lightPos);
  this->boneShader->SetUniform("light.ambient", this->lightAmbient);
  this->boneShader->SetUniform("light.diffuse", this->lightDiffuse);
  this->boneShader->SetUniform("light.specular", this->lightSpecular);

  // material properties
  this->modelShader->SetUniform("material.shininess", this->shininess);
  this->boneShader->SetUniform("material.shininess", this->shininess);

  // Set mvp uniforms
//  this->modelShader->SetUniform("model", this->nanosuit->getModelMatrix());
//  this->modelShader->SetUniform("mvp", (projection * view * this->nanosuit->getModelMatrix()));
//  this->modelShader->SetUniform("isSpecular", true);
//  this->modelShader->SetUniform("useNormal", this->showNormal);
  // Render the model
//  this->nanosuit->Render(*this->modelShader);

  // Set mvp uniforms
  //this->modelShader->SetUniform("model", this->mutant->getModelMatrix());
  //this->modelShader->SetUniform("mvp", (projection * view * this->mutant->getModelMatrix()));
  //this->modelShader->SetUniform("isSpecular", true);
  //this->modelShader->SetUniform("useNormal", false/*this->showNormal*/);
  // Render the model
  //this->mutant->Render(*this->modelShader);

  this->boneShader->SetUniform("model", this->mutant2->getModelMatrix());
  this->boneShader->SetUniform("mvp", (projection * view * this->mutant2->getModelMatrix()));
  this->boneShader->SetUniform("isSpecular", true);
  this->boneShader->SetUniform("useNormal", this->showNormal);
  //this->boneShader->SetUniform("bone", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)));
  // Render the model
  this->mutant2->Update(Ullr::Application::Get().getTime());
  this->mutant2->Render(*this->boneShader);

  this->boneShader->SetUniform("model", this->mutant->getModelMatrix());
  this->boneShader->SetUniform("mvp", (projection * view * this->mutant->getModelMatrix()));
  this->boneShader->SetUniform("isSpecular", false);
  this->boneShader->SetUniform("useNormal", this->showNormal);
  this->mutant->Update(Ullr::Application::Get().getTime());
  this->mutant->Render(*this->boneShader);

  //this->lampShader.Bind();
  this->lampShader->SetUniform("mvp", (projection * view * this->lamp->getModelMatrix()));
  this->lampShader->SetUniform("color", lightSpecular);
  // Render the lamp
  this->lamp->Render(*this->lampShader);
}

void MainLayer::OnImGuiRender()
{
  if (this->io) {
    ImGui::Begin("Main Editor");

//    ImGui::Text("Nanosuit tris: %d", this->nanosuit->getFaceCount());
    ImGui::Text("Mutant tris: %d", this->mutant2->getFaceCount());
    ImGui::Text("Knight tris: %d", this->mutant->getFaceCount());
    ImGui::Text("Lamp tris: %d", this->lamp->getFaceCount());
    ImGui::Spacing();
    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Light Properties")) {
      ImGui::ColorEdit3("Specular Color", glm::value_ptr(this->lightSpecular));
      ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(this->lightDiffuse));
      ImGui::ColorEdit3("Ambient Color", glm::value_ptr(this->lightAmbient));

      ImGui::SliderFloat3("Position", glm::value_ptr(this->lightPos), -20.0f, 20.0f);
    }

    if (ImGui::CollapsingHeader("Material Properties")) {
      ImGui::SliderFloat("Shininess", &this->shininess, 0.0f, 128.0f);

      if (ImGui::Button("Toggle Normal Map"))
        this->showNormal = !this->showNormal;
      ImGui::Text("Normal mapping: %s", this->showNormal ? "on" : "off");
    }

    if (ImGui::CollapsingHeader("Input Properties")) {
      ImGui::SliderFloat("Mouse Sensitivity", &this->mouseSens, 0.0f, 1.0f);
    }

    ImGui::End();
  }
}

void MainLayer::UpdateInput() {
  static bool lastMousePressed = false;
  static std::pair<float, float> lastMousePos = Ullr::Input::InputManager::GetMousePos();

  float speed = 10.0f * Ullr::Application::Get().getFrameTime();

  if (Ullr::Input::InputManager::IsKeyPressed(ULLR_KEY_LEFT_SHIFT)) {
    speed *= 2.0;
  }

  if (Ullr::Input::InputManager::IsKeyPressed(ULLR_KEY_W)) {
    this->camera.Translate(speed, this->camera.getFront());
  }
  if (Ullr::Input::InputManager::IsKeyPressed(ULLR_KEY_S)) {
    this->camera.Translate(speed, this->camera.getBack());
  }
  if (Ullr::Input::InputManager::IsKeyPressed(ULLR_KEY_A)) {
    this->camera.Translate(speed, this->camera.getLeft());
  }
  if (Ullr::Input::InputManager::IsKeyPressed(ULLR_KEY_D)) {
    this->camera.Translate(speed, this->camera.getRight());
  }
  if (Ullr::Input::InputManager::IsKeyPressed(ULLR_KEY_Q)) {
    this->camera.Translate(speed, this->camera.getUp());
  }
  if (Ullr::Input::InputManager::IsKeyPressed(ULLR_KEY_E)) {
    this->camera.Translate(speed, this->camera.getDown());
  }

  auto currentMousePos = Ullr::Input::InputManager::GetMousePos();
  float dx = currentMousePos.first - lastMousePos.first;
  float dy = currentMousePos.second - lastMousePos.second;
  lastMousePos = currentMousePos;

  if (!this->io->WantCaptureMouse) {
    // Capture/uncapture mouse
    bool isPressed = Ullr::Input::InputManager::IsMouseButtonPressed(ULLR_MOUSE_BUTTON_1);

    if (isPressed != lastMousePressed) { // State change
      Ullr::Input::InputManager::CaptureMouse(isPressed);
      lastMousePressed = !lastMousePressed;
    }

    if (isPressed) {
      this->camera.Yaw(-dx * this->mouseSens);
      this->camera.Pitch(-dy * this->mouseSens); // y-axis for mouse is reversed
    }
  }
}



class Sandbox : public Ullr::Application
{
public:
  Sandbox() {
    this->PushLayer(new MainLayer());
  }
  ~Sandbox() { }
};

Ullr::Application* Ullr::CreateApplication()
{
  return new Sandbox();
}
