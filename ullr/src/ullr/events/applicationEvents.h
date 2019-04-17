#pragma once

#include "event.h"
#include "ullr/defines.h"

#include <sstream>

namespace Ullr::Events {

  /// === Window Resized Event ===
  class WindowResizedEvent : public Event
  {
  public:
    WindowResizedEvent(uint32 width, uint32 height)
      : width(width), height(height) {}

    inline uint32 getWidth() const  { return this->width; }
    inline uint32 getHeight() const { return this->height; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "WindowResizeEvent: [" << this->width << ", " << this->height << "]";
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::WindowResize)
    EVENT_CLASS_CATEGORY(EventCategory::Application)

  private:
    uint32 width, height;
  };

  /// === Window Closed Event ===
  class WindowClosedEvent : public Event
  {
  public:

    std::string ToString() const override
    {
      return "WindowClosedEvent";
    }

    EVENT_CLASS_TYPE(EventType::WindowClose)
    EVENT_CLASS_CATEGORY(EventCategory::Application)
  };

}
