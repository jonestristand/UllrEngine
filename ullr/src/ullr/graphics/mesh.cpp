#include "ullrpch.h"
#include "mesh.h"

#include "renderManager.h"
#include "renderMacros.h"

#include "glad/glad.h"

namespace Ullr::Graphics {
  
  Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const std::vector<Texture>& textures)
    : vao(0)
  {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);


    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Mesh::Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(uint32), &this->indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
  }
  
  Mesh::~Mesh()
  { }

  void Mesh::Render(Ullr::Graphics::Shader& shader)
  {
    uint32 diffuseN = 1, specularN = 1;
    for (uint32 i = 0; i < this->textures.size(); i++) {

      std::string num, name = this->textures[i].getType();

      if (name == "diffuse")
        num = std::to_string(diffuseN++);
      else if (name == "specular")
        num = std::to_string(specularN++);

      this->textures[i].Bind(i); // Bind the texture to slot i
      shader.SetUniform1i("material." + name + num, i); // Pass slot i to the shader material struct
    }

    // Add actual render command
    DISPATCH_RENDER_SELF_FN(RenderMesh, {
      glBindVertexArray(self->vao);
      glDrawElements(GL_TRIANGLES, self->indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    });
  }

}
