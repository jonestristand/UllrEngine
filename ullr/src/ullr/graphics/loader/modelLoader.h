#pragma once

#include "ullr/defines.h"
#include "ullr/graphics/model.h"

namespace Ullr::Graphics::Loader
{

  class ModelLoader
  {
  public:
    ModelLoader();
    virtual ~ModelLoader() = default;

    virtual Model* LoadFromFile(const std::string& path) = 0;
    virtual Model* LoadFromMemory(const byte* modelBuffer) = 0;
  };

}
