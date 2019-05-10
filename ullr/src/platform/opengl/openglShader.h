#pragma once

#include "ullr/graphics/shader.h"

namespace Ullr::Graphics::OpenGL {

  class OpenGLShader : public Shader
  {
  public:
    OpenGLShader(const std::string& filename);
    virtual ~OpenGLShader();

    void CompileAndLoad();
    void Bind();

    void SetUniform(const std::string& name, float val) override;
    void SetUniform(const std::string& name, const glm::vec3& vec) override;
    void SetUniform(const std::string& name, const glm::vec4& vec) override;
    void SetUniform(const std::string& name, int32 val) override;
    void SetUniform(const std::string& name, uint32 val) override;
    void SetUniform(const std::string& name, const glm::mat4& mat) override;

  private: // methods
    GLenum getShaderTypeFromString(const std::string& shaderType);

  private: // fields
    uint32 shaderProgramId;
  };

}
