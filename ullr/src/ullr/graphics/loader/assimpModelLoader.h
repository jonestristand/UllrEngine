#pragma once

#include "ullr/defines.h"
#include "modelLoader.h"

#include "ullr/graphics/animation/joint.h"
#include "ullr/graphics/animation/animator.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiString;
template<typename TReal> class aiMatrix4x4t;
typedef aiMatrix4x4t<float> aiMatrix4x4;
enum aiTextureType;

namespace Ullr::Graphics::Loader
{

  class AssimpModelLoader : public ModelLoader
  {
  public:
    AssimpModelLoader();
    virtual ~AssimpModelLoader() = default;

    virtual Model* LoadFromFile(const std::string& path) override;
    virtual Model* LoadFromMemory(const byte* modelBuffer) override;

  private: // methods
    void processMeshNodes(aiNode* node, const aiScene* scene, MeshPtrList& meshList);
    Animation::Joint* buildBoneHierarchy(aiNode* node, const aiScene* scene);
    MeshPtr processMesh(aiMesh* mesh, const aiScene* scene);
    std::unique_ptr<Animation::Animator> loadAnimations(const aiScene* scene);
    TexturePtrList loadTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
    glm::mat4 convertAssimpMat4(const aiMatrix4x4& mat);

  private: // fields
    std::string currDirectory;

    std::unordered_map<std::string, TexturePtr> loadedTextures;

    std::unordered_map<std::string, uint32> boneNameIndices;
    std::vector<glm::mat4> boneMatrices;
  };

}
