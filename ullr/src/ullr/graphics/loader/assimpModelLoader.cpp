#include "ullrpch.h"

#include "assimpModelLoader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "stb_image.h"
#include <iostream>

#include "ullr/graphics/animation/animatedModel.h"
#include "ullr/graphics/animation/jointTransform.h"

namespace Ullr::Graphics::Loader
{

  AssimpModelLoader::AssimpModelLoader()
    : ModelLoader(), currDirectory(), loadedTextures(), boneNameIndices(), boneMatrices()
  {}

  Model* AssimpModelLoader::LoadFromFile(const std::string& path)
  {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights);

    // Ensure scene loaded succesfully
    if ((!scene) || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || (!scene->mRootNode)) {
      UL_CORE_ERROR("[ASSIMP] Unable to load model '{0}'.", path);
      return new Model(MeshPtrList()); // Return an empty model
    }

    // Store the directory of the model root for finding textures later
    this->currDirectory = path.substr(0, path.find_last_of('/') + 1);

    MeshPtrList meshList; // stores the list of meshes
    meshList.reserve(scene->mNumMeshes);

    this->boneNameIndices.clear();

    if (!scene->HasAnimations()) {
      this->processMeshNodes(scene->mRootNode, scene, meshList);

      return new Model(meshList); // Create and return the new model
    }
    else { // build and return an AnimatedModel instead
      this->processMeshNodes(scene->mRootNode, scene, meshList);
      auto skeleton = this->buildBoneHierarchy(scene->mRootNode, scene);
      auto animator = this->loadAnimations(scene);

      return new Animation::AnimatedModel(meshList, skeleton, this->boneNameIndices.size(), animator); // Create and return the new model
    }
  }

  Model* AssimpModelLoader::LoadFromMemory(const byte* modelBuffer)
  {
    UL_CORE_ASSERT(false, "Loading model from memory buffer not yet supported");
    return nullptr;
  }

  void AssimpModelLoader::processMeshNodes(aiNode* node, const aiScene* scene, MeshPtrList& meshList)
  {
    // Process any meshes in this node
    for (uint32 i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshList.emplace_back(this->processMesh(mesh, scene));
    }

    // Process any child nodes of this node
    for (uint32 i = 0; i < node->mNumChildren; i++) {
      this->processMeshNodes(node->mChildren[i], scene, meshList);
    }
  }

  Animation::Joint* AssimpModelLoader::buildBoneHierarchy(aiNode* node, const aiScene* scene)
  {
    auto boneEntry = this->boneNameIndices.find(node->mName.C_Str());
    Animation::Joint* joint = nullptr;

    if (boneEntry != this->boneNameIndices.end()) {
      // Is a bone
      joint = new Animation::Joint(boneEntry->second, boneEntry->first, this->boneMatrices[boneEntry->second]);
    }

    // Loop through children
    for (uint32 i = 0; i < node->mNumChildren; i++) {
      auto child = this->buildBoneHierarchy(node->mChildren[i], scene);
      if (child != nullptr) {
        if (joint != nullptr) // This is a joint/bone node - add any children
          joint->AddChild(child); // Add if any child found further down hierarchy
        else // This is not a bone node, return the first child
          return child; // TODO: fix this to be more flexible than just returning first child
      }
    }

    return joint;
  }

  MeshPtr AssimpModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
  {
    std::vector<Mesh::Vertex> vertices;
    std::vector<uint32> vertexBoneWeightCount;
    std::vector<uint32> indices;
    TexturePtrList textures;

    // Keep track of how many bone weights each vertex has been assigned so far
    vertexBoneWeightCount.resize(mesh->mNumVertices, 0);

    // Pre-size the vertex buffer to hold all of the vertices
    vertices.reserve(mesh->mNumVertices);

    // == Process vertices
    for (uint32 i = 0; i < mesh->mNumVertices; i++) {
      Mesh::Vertex vert;

      // Position
      glm::vec3 pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
      vert.Position = pos;

      // Normal
      glm::vec3 norm(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
      vert.Normal = norm;

      // TexCoords
      if (mesh->mTextureCoords[0]) {// Does it have any texture coordinates?
        vert.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
      }
      else {
        vert.TexCoords = glm::vec2(0.0f, 0.0f);
      }

      // Tangent
      if (mesh->HasTangentsAndBitangents()) { // Does it have a calculated tangent?
        vert.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
      }
      else {
        vert.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
      }

      // Clear the boneids and bone weights vector
      vert.BoneIds = glm::ivec4(0, 0, 0, 0);
      vert.BoneWeights = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

      // Push into vertex array
      vertices.push_back(vert);
    }

    // == Process indices
    for (uint32 i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (uint32 j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    // == Process materials
    if (mesh->mMaterialIndex >= 0)
    {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      TexturePtrList diffuseMaps = this->loadTextures(material, aiTextureType_DIFFUSE, "diffuse", scene);
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

      TexturePtrList specularMaps = this->loadTextures(material, aiTextureType_SPECULAR, "specular", scene);
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

      // TODO: Hack to support different model formats
      TexturePtrList normalMaps1 = this->loadTextures(material, aiTextureType_NORMALS, "normal", scene);
      textures.insert(textures.end(), normalMaps1.begin(), normalMaps1.end());
      TexturePtrList normalMaps2 = this->loadTextures(material, aiTextureType_HEIGHT, "normal", scene);
      textures.insert(textures.end(), normalMaps2.begin(), normalMaps2.end());
    }

    // == Process bones
    if (mesh->HasBones()) {
      // If the mesh has bones, iterate through them and record the name of each bone in this->boneNames
      this->boneNameIndices.reserve(mesh->mNumBones);
      this->boneMatrices.resize(mesh->mNumBones);

      for (uint32 i = 0; i < mesh->mNumBones; i++) {
        auto bone = mesh->mBones[i];
        this->boneNameIndices.emplace(bone->mName.C_Str(), i);
        this->boneMatrices[i] = this->convertAssimpMat4(bone->mOffsetMatrix);

        // Now iterate all weights for this bone
        for (uint32 j = 0; j < bone->mNumWeights; j++) {
          // Find an empty slot in the vector and record the bone index and weight
          auto vertexId = bone->mWeights[j].mVertexId;

          // TODO: move max bones to a constant somewhere
          if (vertexBoneWeightCount[vertexId] < 4) {
            vertices[vertexId].BoneIds[vertexBoneWeightCount[vertexId]] = i;
            vertices[vertexId].BoneWeights[vertexBoneWeightCount[vertexId]] = bone->mWeights[j].mWeight;

            vertexBoneWeightCount[vertexId]++;
          }
        }

      }

    }
    
    // TODO: probably have model with function to load all meshes in to GPU at once
    auto newMesh = Mesh::CreateMesh(vertices, indices, textures);
    newMesh->UploadMesh();
    return newMesh;
  }

  std::unique_ptr<Animation::Animator> AssimpModelLoader::loadAnimations(const aiScene * scene)
  {
    // TODO: Rewrite all this to load multiple animations
    auto animation = scene->mAnimations[0];

    std::vector<Animation::KeyFrameList> keyFrames;
    keyFrames.resize(this->boneNameIndices.size());

    // Loop through all channels in the animation
    for (int j = 0; j < animation->mNumChannels; j++) {
      auto jointIndexIterator = this->boneNameIndices.find(animation->mChannels[j]->mNodeName.C_Str());
      if (jointIndexIterator != this->boneNameIndices.end())
      {
        auto jointIndex = jointIndexIterator->second;

        for (int i = 0; i < animation->mChannels[j]->mNumPositionKeys; i++) {
          auto translation = animation->mChannels[j]->mPositionKeys[i].mValue;
          auto rotation = animation->mChannels[j]->mRotationKeys[i].mValue;
          auto keyFrameTime = animation->mChannels[j]->mPositionKeys[i].mTime;
          Animation::JointTransform transform;
          transform.Translation = glm::vec3(translation.x, translation.y, translation.z);
          transform.Rotation = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);

          keyFrames[jointIndex].AddKeyframe(keyFrameTime / animation->mTicksPerSecond, transform);
        }
      }
    }

    auto animator = std::make_unique<Animation::Animator>(animation->mDuration / animation->mTicksPerSecond, keyFrames);
    return std::move(animator);
  }

  TexturePtrList AssimpModelLoader::loadTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
  {
    TexturePtrList textures;
    for (uint32 i = 0; i < mat->GetTextureCount(type); i++) {
      aiString filename;
      mat->GetTexture(type, i, &filename);

      TexturePtr texture;

      // First check if it exists already in our list of textures (339.8MB)
      auto cachedTexture = this->loadedTextures.find(filename.C_Str());
      if (cachedTexture == this->loadedTextures.end()) {

        if (auto tex = scene->GetEmbeddedTexture(filename.C_Str()))
        {
          auto hint = tex->achFormatHint; // TODO: need to check if compressed or uncompressed and address that apropriately with stbi or direct load
          int32 w, h, c;
          
          byte* buffer = stbi_load_from_memory((byte*)tex->pcData, tex->mWidth, &w, &h, &c, 4); // this->filePath.c_str(), & this->width, & this->height, & this->numChannels, 4)

          texture = std::shared_ptr<Texture>(Texture::CreateTexture(buffer, w, h, typeName));
        }
        else {

          std::string fullpath = this->currDirectory + std::string(filename.C_Str());

          // TODO: stbi loading should be done in the model loader, not the texture class
          texture = std::shared_ptr<Texture>(Texture::CreateTexture(fullpath, typeName));
          texture->ReadFile();
        }

        // Send command the renderer to load texture data to the GPU
        texture->LoadTexture();

        this->loadedTextures.emplace(filename.C_Str(), texture);
      }
      else { // Texture is in repository
        texture = cachedTexture->second;
      }

      textures.push_back(texture);
    }

    return textures;
  }

  glm::mat4 AssimpModelLoader::convertAssimpMat4(const aiMatrix4x4& mat)
  {
    glm::mat4 result;
    result[0].x = mat.a1; result[0].y = mat.b1; result[0].z = mat.c1; result[0].w = mat.d1;
    result[1].x = mat.a2; result[1].y = mat.b2; result[1].z = mat.c2; result[1].w = mat.d2;
    result[2].x = mat.a3; result[2].y = mat.b3; result[2].z = mat.c3; result[2].w = mat.d3;
    result[3].x = mat.a4; result[3].y = mat.b4; result[3].z = mat.c4; result[3].w = mat.d4;

    return result;
  }

}
