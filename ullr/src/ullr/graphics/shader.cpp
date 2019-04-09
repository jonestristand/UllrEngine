#include "ullrpch.h"

#include "shader.h"

#include "renderManager.h"
#include "renderMacros.h"

#include <regex>

namespace Ullr::Graphics {

  Shader::Shader(const char* filename)
    :programLoaded(false), shaderProgramId(0)
  {
    std::string file;
    std::regex re("(?:#shader\\s+(vertex|fragment|pixel))\\s+([^]+?)(?=#endshader)");

    this->shaderSources = new std::unordered_map<GLenum, std::string>();

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

        GLenum shaderType = this->getShaderTypeFromString(match.str(1));
        if (shaderType)
          this->shaderSources->insert({ shaderType, match.str(2) });

        next++;
      }
    }
  }

  Shader::~Shader()
  {
    if (this->programLoaded)
      glDeleteProgram(this->shaderProgramId);

    delete this->shaderSources;
  }

  void Shader::CompileAndLoad()
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

  void Shader::Bind()
  {
    DISPATCH_RENDER_SELF_FN(BindShader, {
      glUseProgram(self->shaderProgramId);
    });
  }

  void Shader::SetUniform4f(const char* name, glm::vec4 vec)
  {
    DISPATCH_RENDER_SELF_FN2(SetUniform4f, name, vec, {
      uint32 uniformLocation = glGetUniformLocation(self->shaderProgramId, name);
      glUniform4f(uniformLocation, vec[0], vec[1], vec[2], vec[3]);
    });
  }

  void Shader::SetUniform1i(const char* name, int32 val)
  {
    DISPATCH_RENDER_SELF_FN2(SetUniform1i, name, val, {
      uint32 uniformLocation = glGetUniformLocation(self->shaderProgramId, name);
      glUniform1i(uniformLocation, val);
    });

  }

  void Shader::SetUniformMatrix4f(const char* name, glm::mat4 mat)
  {
    DISPATCH_RENDER_SELF_FN2(SetUniform1i, name, mat, {
      uint32 uniformLocation = glGetUniformLocation(self->shaderProgramId, name);
      glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
    });
  }

  GLenum Shader::getShaderTypeFromString(const std::string& shaderType)
  {
    if (shaderType == "vertex")
      return GL_VERTEX_SHADER;
    if (shaderType == "fragment" || shaderType == "pixel")
      return GL_FRAGMENT_SHADER;
    else return NULL;
  }

}
