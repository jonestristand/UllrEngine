#include "ullrpch.h"

#include "application.h"
#include "window.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/vertexBuffer.h"
#include "graphics/indexBuffer.h"
#include "graphics/mesh.h"
#include "graphics/renderMacros.h"

#include "utils/fpsImGuiLayer.h"


// TODO: THIS IS TEMPORARY
#include "input/keyCodes.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "graphics/renderManager.h"

namespace Ullr {

  Application* Application::instance = nullptr;

  Application::Application()
  {
    UL_CORE_ASSERT(!Application::instance, "Application already exists!")
    Application::instance = this;

    this->window = std::unique_ptr<Window>(Window::Create());
    this->window->setVSync(false);

    // Bind the event callback for the window
    this->window->setEventCallback(ULLR_BIND_EVENT_FN(Application::OnEvent));

    // Create managers
    this->renderManager = Ullr::Graphics::RenderManager::Create();
    this->renderManager->Init();

    this->imguiLayer = new ImGuiLayer();
    this->PushOverlay(this->imguiLayer);

    this->PushOverlay(new Utils::FpsImGuiLayer());
    
  }


  Application::~Application()
  {
    this->renderManager->Shutdown();
//    delete this->renderManager;
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
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // TODO: Temp
    // Create triangle
//    static float vertices[] = {
//      // positions          // colors           // texture coords
//       0.5f,  0.5f, 0.0f,  /* 1.0f, 1.0f, 1.0f, */  1.0f, 1.0f, // top right
//       0.5f, -0.5f, 0.0f,  /* 1.0f, 1.0f, 1.0f, */  1.0f, 0.0f, // bottom right
//      -0.5f, -0.5f, 0.0f,  /* 1.0f, 1.0f, 1.0f, */  0.0f, 0.0f, // bottom left
//      -0.5f,  0.5f, 0.0f,  /* 1.0f, 1.0f, 1.0f, */  0.0f, 1.0f  // top left 
//    };

    float vertices[] = {
      -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, // bottom front left
       0.5f, -0.5f,  0.5f,    0.0f, 1.0f, // bottom front right
       0.5f,  0.5f,  0.5f,    0.0f, 0.0f, // top front right
      -0.5f,  0.5f,  0.5f,    1.0f, 0.0f, // top front left

      -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom front left
       0.5f, -0.5f, -0.5f,    1.0f, 0.0f, // bottom front right
       0.5f,  0.5f, -0.5f,    1.0f, 1.0f, // top front right
      -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // top front left
    };

    static unsigned int indices[] = {
      // front face
      0, 2, 1,
      0, 3, 2,

      // right face
      1, 6, 5,
      1, 2, 6,

      // left face
      0, 4, 7,
      0, 7, 3,

      // top face
      3, 6, 2,
      3, 7, 6,

      // bottom face
      0, 1, 5,
      0, 5, 4,

      // back face
      4, 5, 6,
      4, 6, 7
    };

    glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),
      glm::vec3(1.5f,  2.0f, -2.5f),
      glm::vec3(1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    Graphics::Shader* shader = new Graphics::Shader("assets/shaders/main.glsl");
    Graphics::Texture* texture1 = new Graphics::Texture("assets/textures/container.jpg");
    Graphics::Texture* texture2 = new Graphics::Texture("assets/textures/awesomeface.png");
    Graphics::Mesh* mesh1 = new Graphics::Mesh(vertices, sizeof(vertices), indices, sizeof(indices), sizeof(indices)/sizeof(indices[0]));
    shader->CompileAndLoad();

    this->renderManager->SetClearColor(0.0f, 0.0f, 0.0f);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)this->getWindow().getWidth() / (float)this->getWindow().getHeight(), 0.1f, 100.0f);

    while (this->running)
    {
      this->renderManager->ClearBuffer();

      shader->Bind();
      texture1->Bind(0);
      shader->SetUniform1i("u_Texture1", 0);
      texture2->Bind(1);
      shader->SetUniform1i("u_Texture2", 1);

      shader->SetUniform4f("u_Color", color);

//      model = glm::rotate(glm::mat4(1.0f), (float)this->time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

//      shader->SetUniformMatrix4f("model", model);
      shader->SetUniformMatrix4f("view", view);
      shader->SetUniformMatrix4f("projection", projection);

      for (int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);

        if (i % 3 == 0) {
          model = glm::rotate(model, (float)this->time * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        }
        else {
          model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
        }
        shader->SetUniformMatrix4f("model", model);

        mesh1->Render();
      }

      // TODO: This can likely all be done by the layer stack (LayerManager) itself, and not expose layers to the Application directly?
      for (Layer* layer : this->layerStack)
        layer->OnUpdate();

      DISPATCH_RENDER_SELF_FN(RenderImGUI, {
        // Render ImGui interface
        self->imguiLayer->Begin();

        for (Layer* layer : self->layerStack)
          layer->OnImGuiRender();

        self->imguiLayer->End();
      });

      this->renderManager->Render();

      // Update the window
      this->window->Update();

      // Update the timer
      this->time = glfwGetTime();
    }

    delete mesh1;
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
