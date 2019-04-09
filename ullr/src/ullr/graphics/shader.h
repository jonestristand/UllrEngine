#pragma once

#include "glad/glad.h"

#include "spdlog/fmt/ostr.h"

namespace Ullr::Graphics {

  class Shader
  {
  public:
    Shader(const char* filename);
    ~Shader();

    void CompileAndLoad();
    void Bind();

    void SetUniform4f(const char* name, glm::vec4 vec);
    void SetUniform1i(const char* name, int32 val);
    void SetUniformMatrix4f(const char* name, glm::mat4 mat);

    inline bool IsLoaded() { return this->programLoaded; }

  private: // methods
    GLenum getShaderTypeFromString(const std::string& shaderType);

  private: // fields
    std::unordered_map<GLenum, std::string>* shaderSources;
    std::string filename;

    GLuint shaderProgramId;

    bool programLoaded;
  };

}
