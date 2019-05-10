#pragma once

namespace Ullr {

  class Manager
  {
  public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual ~Manager() = default;
  };

}
