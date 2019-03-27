#pragma once

#include "defines.h"
#include "layer.h"

namespace Ullr {

  class LayerStack
  {
  public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    inline std::vector<Layer*>::iterator begin() { return layers.begin(); }
    inline std::vector<Layer*>::iterator end() { return layers.end(); }

  private:
    std::vector<Layer*> layers;
    std::vector<Layer*>::iterator layerInsert;
  };

}