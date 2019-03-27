#pragma once

#include "defines.h"
#include "events/event.h"

namespace Ullr {

  class ULLR_API Layer
  {
  public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnEvent(Events::Event& event) {}

    inline const std::string& getName() const { return this->debugName; }

  protected:
    std::string debugName;
  };

}