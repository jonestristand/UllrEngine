#pragma once

namespace Ullr::Graphics::Command {

  class RenderCommand
  {
  public:
    virtual ~RenderCommand() {}

    virtual void Execute() = 0;
  };

}
