#include "ullrpch.h"
#include "layerStack.h"

namespace Ullr {

  LayerStack::LayerStack()
  {}

  LayerStack::~LayerStack()
  {
    for (Layer* layer : this->layers)
      delete layer;
  }

  void LayerStack::PushLayer(Layer* layer)
  {
    layers.emplace(this->layers.begin() + this->layerInsertIndex, layer);
    this->layerInsertIndex++;
  }

  void LayerStack::PushOverlay(Layer* overlay)
  {
    this->layers.emplace_back(overlay);
  }

  // TODO: concern that this function actually releases responsibility for the raw pointer without indicating this is the case
  void LayerStack::PopLayer(Layer* layer)
  {
    // Location of layer inside layer stack vector
    auto it = std::find(this->layers.begin(), this->layers.end(), layer);
    if (it != this->layers.end()) {
      this->layers.erase(it);
      this->layerInsertIndex--;
    }
  }

  // TODO: concern that this function actually releases responsibility for the raw pointer without indicating this is the case
  // Consider just enable/disable of layers but layers created only in layerstack by layerstack (force ownership)
  void LayerStack::PopOverlay(Layer* overlay)
  {
    auto it = std::find(this->layers.begin(), this->layers.end(), overlay);
    if (it != this->layers.end()) {
      this->layers.erase(it);
    }
  }

}
