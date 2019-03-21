#pragma once

#include "event.h"
#include "ullr/defines.h"

namespace Ullr::Events {

  /// === Window Resized Event ===
  class ULLR_API WindowResizedEvent : public Event
  {
  public:
    WindowResizedEvent(unsigned int width, unsigned int height)
      : width(width), height(height) {}

    inline unsigned int getWidth() const  { return this->width; }
    inline unsigned int getHeight() const { return this->height; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "WindowResizeEvent: [" << this->width << ", " << this->height << "]";
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::WindowResize)
    EVENT_CLASS_CATEGORY(EventCategory::Application)

  private:
    unsigned int width, height;
  };

  /// === Window Closed Event ===
  class ULLR_API WindowClosedEvent : public Event
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