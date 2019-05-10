#include "ullrpch.h"

#include "openglShader.h"
#include "command/shaderCommands.hpp"

#include <regex>

namespace Ullr::Graphics::OpenGL {

  OpenGLShader::OpenGLShader(const std::string& filename)
    :Shader(filename), shaderProgramId(0)
  {
    std::string file;
    std::regex re("(?:#shader\\s+(vertex|fragment|pixel))\\s+([^]+?)(?=#endshader)");

    //this->shaderSources = new std::unordered_map<GLenum, std::string>();

    std::ifstream is(filename, std::ifstream::binary);
    if (is) {
      // Save filename
      this->filename = filename;

      is.seekg(0, std::ios::end);
      file.resize(is.tellg());
      is.seekg(0, std::ios::beg);

      is.read(&file[0], file.size());

      is.close();      
      
      std::sregex_iterator next(file.begin(), file.end(), re, std::regex_constants::match_any);
      std::sregex_iterator ender;
      while (next != ender) {
        std::smatch match = *next;

        uint32 shaderType = this->getShaderTypeFromString(match.str(1));
        if (shaderType)
          this->shaderSources->insert({ shaderType, match.str(2) });

        next++;
      }
    }
  }

  OpenGLShader::~OpenGLShader()
  {
    if (this->programLoaded)
      glDeleteProgram(this->shaderProgramId);

    delete this->shaderSources;
  }

  void OpenGLShader::CompileAndLoad()
  {
    GLint success;
    char infoLog[1024];

    std::vector<GLuint> shaderIds;

    // Set to default status, and false on error
    programLoaded = true;

    this->shaderProgramId = glCreateProgram();

    for (auto& kv : *(this->shaderSources)) {
      GLint shaderId = glCreateShader(kv.first);
      const GLchar* source = kv.second.c_str();

      glShaderSource(shaderId, 1, &source, NULL);
      glCompileShader(shaderId);

      // Error check
      glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
        UL_CORE_WARN("Unable to compile shader {0}: {1}", this->filename, infoLog);
        glDeleteShader(shaderId);
      }
      else {
        glAttachShader(this->shaderProgramId, shaderId);
        shaderIds.push_back(shaderId);
      }
    }

    glLinkProgram(this->shaderProgramId);

    // Check if successful
    glGetProgramiv(this->shaderProgramId, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(this->shaderProgramId, 1024, NULL, infoLog);
      UL_CORE_WARN("Unable to link shader program {0}: {1}", this->filename, infoLog);
      glDeleteProgram(this->shaderProgramId);

      programLoaded = false;
      this->shaderProgramId = 0;
    }

    // Finally, clean up shader programs
    for (auto shaderId : shaderIds) {
      glDetachShader(this->shaderProgramId, shaderId);
      glDeleteShader(shaderId);
    }
  }

  void OpenGLShader::Bind()
  {
    Command::BindShader::Dispatch(this->shaderProgramId);
  }

  void OpenGLShader::SetUniform(const std::string& name, float val)
  {
    Command::SetUniform1f::Dispatch(this->shaderProgramId, name, val);
  }

  void OpenGLShader::SetUniform(const std::string& name, const glm::vec3& vec)
  {
    Command::SetUniform3f::Dispatch(this->shaderProgramId, name, vec);
  }

  void OpenGLShader::SetUniform(const std::string& name, const glm::vec4& vec)
  {
    Command::SetUniform4f(this->shaderProgramId, name, vec);
  }

  void OpenGLShader::SetUniform(const std::string& name, int32 val)
  {
    Command::SetUniform1i::Dispatch(this->shaderProgramId, name, val);
  }

  void OpenGLShader::SetUniform(const std::string& name, uint32 val)
  {
    Command::SetUniform1ui::Dispatch(this->shaderProgramId, name, val);
  }

  void OpenGLShader::SetUniform(const std::string& name, const glm::mat4& mat)
  {
    Command::SetUniformMatrix4f::Dispatch(this->shaderProgramId, name, mat);
  }

  GLenum OpenGLShader::getShaderTypeFromString(const std::string& shaderType)
  {
    if (shaderType == "vertex")
      return GL_VERTEX_SHADER;
    if (shaderType == "fragment" || shaderType == "pixel")
      return GL_FRAGMENT_SHADER;
    else return NULL;
  }

}
