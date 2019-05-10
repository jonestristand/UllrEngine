#pragma once

namespace Ullr::Graphics {

  class RenderCommand
  {
  public:
    virtual ~RenderCommand() {}

    virtual void Execute() = 0;
  };

}
