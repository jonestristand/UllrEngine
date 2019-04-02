#pragma once

#include "event.h"
#include "ullr/defines.h"

namespace Ullr::Events {

  /// === Mouse Moved Event ===
  class ULLR_API MouseMovedEvent : public Event
  {
  public:
    MouseMovedEvent(float x, float y)
      : x(x), y(y) {}

    inline float getX() const { return this->x; }
    inline float getY() const { return this->y; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseMovedEvent: [" << this->x << ", " << this->y << "]";
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseMove)
    EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

  private:
    float x, y;
  };

  /// === Mouse Scrolled Event ===
  class ULLR_API MouseScrolledEvent : public Event
  {
  public:
    MouseScrolledEvent(float offsetX, float offsetY)
      : offsetX(offsetX), offsetY(offsetY) {}

    inline float getOffsetX() const { return this->offsetX; }
    inline float getOffsetY() const { return this->offsetY; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseScrolledEvent: [" << this->offsetX << ", " << this->offsetY << "]";
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseScroll)
    EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

  private:
    float offsetX, offsetY;
  };

  /// === Mouse Button Event (abstract) ===
  class ULLR_API MouseButtonEvent : public Event
  {
  public:
    inline int32 getButton() { return this->button; }

    EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

  protected:
    MouseButtonEvent(int32 button)
      : button(button) {}

    int32 button;
  };

  /// === Mouse Button Pressed Event ===
  class ULLR_API MouseButtonPressedEvent : public MouseButtonEvent
  {
  public:
    MouseButtonPressedEvent(int32 button)
      : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseButtonPressedEvent: " << this->button;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseButtonPress)
  };

  /// === Mouse Button Released Event ===
  class ULLR_API MouseButtonReleasedEvent : public MouseButtonEvent
  {
  public:
    MouseButtonReleasedEvent(int32 button)
      : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "MouseButtonReleasedEvent: " << this->button;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseButtonRelease)
  };

}
