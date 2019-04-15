#pragma once

//#include "spdlog/fmt/ostr.h"

namespace Ullr::Graphics {

  class Shader
  {
  public:
    Shader(const char* filename);
    ~Shader();

    void CompileAndLoad();
    void Bind();

    void SetUniform1f(const std::string& name, float val);
    void SetUniform3f(const std::string& name, glm::vec3 vec);
    void SetUniform4f(const std::string& name, glm::vec4 vec);
    void SetUniform1i(const std::string& name, int32 val);
    void SetUniformMatrix4f(const std::string& name, glm::mat4 mat);

    inline bool IsLoaded() { return this->programLoaded; }

    inline std::string ToString() const { return this->filename; }

  private: // methods
    uint32 getShaderTypeFromString(const std::string& shaderType);

  private: // fields
    std::unordered_map<uint32, std::string>* shaderSources;
    std::string filename;

    uint32 shaderProgramId;

    bool programLoaded;
  };

  inline std::ostream& operator<<(std::ostream& os, const Shader& s)
  {
    return os << s.ToString();
  }

}
