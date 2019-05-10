#pragma once

#include "manager.h"

#include <chrono>

namespace Ullr {

  class TimeManager : public Manager
  {
  public:
    TimeManager();
    ~TimeManager() = default;

    void Init() override;
    void Shutdown() override;

    void Update();
    void Reset();

    double getTime();
    inline double getFrametime() { return this->lastFrameTime; }

  private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point frameStartTime;
    double lastFrameTime;
  };

}
