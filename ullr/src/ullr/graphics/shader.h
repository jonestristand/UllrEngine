#pragma once

#include "ullr/defines.h"

namespace Ullr::Graphics {

  class Shader
  {
  public:
    inline Shader(const std::string& filename)
    : shaderSources(new std::unordered_map<uint32, std::string>()),
      filename(filename), programLoaded(false)
    {}

    virtual ~Shader() {}

    virtual void CompileAndLoad() = 0;
    virtual void Bind() = 0;

    virtual void SetUniform(const std::string& name, float val) = 0;
    virtual void SetUniform(const std::string& name, const glm::vec3& vec) = 0;
    virtual void SetUniform(const std::string& name, const glm::vec4& vec) = 0;
    virtual void SetUniform(const std::string& name, int32 val) = 0;
    virtual void SetUniform(const std::string& name, uint32 val) = 0;
    virtual void SetUniform(const std::string& name, const glm::mat4& mat) = 0;

    inline bool getIsLoaded() { return this->programLoaded; }
    inline std::string getToString() const { return this->filename; }

  protected: // fields
    std::unordered_map<uint32, std::string>* shaderSources;
    std::string filename;

    bool programLoaded;

  public: // STATIC (implement per-platform)
    static Shader* CreateShader(const std::string& filename);
  };

  // Logger for shaders
  inline std::ostream& operator<<(std::ostream& os, const Shader& s)
  {
    return os << s.getToString();
  }

}
