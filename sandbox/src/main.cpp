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

private:
  ImGuiIO* io;

  Ullr::Graphics::Shader modelShader;
  Ullr::Graphics::Shader lampShader;
  Ullr::Graphics::Model nanosuit;
  Ullr::Graphics::Model lamp;

  glm::vec3 lightPos;
  glm::vec3 lightDiffuse;
  glm::vec3 lightAmbient;
  glm::vec3 lightSpecular;
  float shininess;

  glm::vec3 cameraPos = glm::vec3(0.0f, -8.0f, -20.0f);
};

MainLayer::MainLayer()
  : Layer("Main Layer"), io(nullptr),
  lightPos(8.0f, 12.0f, 8.0f), lightDiffuse(1.0f, 1.0f, 1.0f), lightAmbient(0.5f, 0.5f, 0.5f), lightSpecular(1.0f, 1.0f, 1.0f), shininess(8.0f),
  modelShader("assets/shaders/main_tex.glsl"), lampShader("assets/shaders/lamp.glsl"),
  nanosuit("assets/models/nanosuit/nanosuit.obj"), lamp("assets/models/lamp/lamp.obj")
{
  this->modelShader.CompileAndLoad();
  this->lampShader.CompileAndLoad();
}

MainLayer::~MainLayer()
{ }

void MainLayer::OnAttach()
{
  this->io = &(ImGui::GetIO());
}

void MainLayer::OnDetach()
{
  this->io = nullptr; // Stop pointing to ImGuiIO - ImGui to manage lifetime of object
}

void MainLayer::OnUpdate()
{
  glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)Ullr::Application::Get().getWindow().getWidth() / (float)Ullr::Application::Get().getWindow().getHeight(), 0.1f, 1000.0f);

  glm::mat4 modelModel = glm::mat4(1.0f);
  modelModel = glm::rotate(modelModel, glm::radians(45.0f * (float)Ullr::Application::Get().getTime()), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 viewModel = glm::mat4(1.0f);
  viewModel = glm::translate(viewModel, this->cameraPos);

  glm::mat4 modelLamp = glm::mat4(1.0f);
  modelLamp = glm::scale(modelLamp, glm::vec3(0.5f, 0.5f, 0.5f));
  glm::mat4 viewLamp = glm::mat4(1.0f);
  viewLamp = glm::translate(viewLamp, cameraPos);
  viewLamp = glm::translate(viewLamp, lightPos);

  //this->modelShader.Bind();

  // Set mvp uniforms
  this->modelShader.SetUniform("model", modelModel);
  this->modelShader.SetUniform("mvp", (projection * viewModel * modelModel));
  this->modelShader.SetUniform("viewPos", -this->cameraPos);

  // Set light uniforms
  this->modelShader.SetUniform("light.position", this->lightPos);
  this->modelShader.SetUniform("light.ambient", this->lightAmbient);
  this->modelShader.SetUniform("light.diffuse", this->lightDiffuse);
  this->modelShader.SetUniform("light.specular", this->lightSpecular);

  // material properties
  this->modelShader.SetUniform("material.shininess", this->shininess);
  
  // Render the model
  this->nanosuit.Render(this->modelShader);

  //this->lampShader.Bind();
  this->lampShader.SetUniform("mvp", (projection * viewLamp * modelLamp));
  this->lampShader.SetUniform("color", lightSpecular);
  // Render the lamp
  this->lamp.Render(lampShader);
}

void MainLayer::OnImGuiRender()
{
  if (this->io) {
    ImGui::Begin("Main Editor");

    ImGui::Text("Light Properties");
    ImGui::ColorEdit3("Specular Color", glm::value_ptr(this->lightSpecular));
    ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(this->lightDiffuse));
    ImGui::ColorEdit3("Ambient Color", glm::value_ptr(this->lightAmbient));

    ImGui::SliderFloat3("Light Height", glm::value_ptr(this->lightPos), -20.0f, 20.0f);

    ImGui::Text("Material Properties");
    ImGui::SliderFloat("Shininess", &this->shininess, 0.0f, 128.0f);

    ImGui::End();
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
