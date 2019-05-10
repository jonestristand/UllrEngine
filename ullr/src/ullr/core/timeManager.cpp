#include "ullrpch.h"
#include "timeManager.h"

#include <chrono>

namespace Ullr
{

  TimeManager::TimeManager()
    : lastFrameTime(0.0)
  { }


  void TimeManager::Init()
  {
    this->Reset();
  }

  void TimeManager::Shutdown()
  { }

  void TimeManager::Reset()
  {
    this->startTime = std::chrono::high_resolution_clock::now();
    this->frameStartTime = this->startTime;
  }

  void TimeManager::Update()
  {
    auto currTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = currTime - this->frameStartTime;

    this->frameStartTime = currTime;

    this->lastFrameTime = duration.count();
  }

  double TimeManager::getTime()
  {
    auto currTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = currTime - this->startTime;

    return duration.count();
  }

}
