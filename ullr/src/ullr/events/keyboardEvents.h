#pragma once

#include "event.h"
#include "ullr/defines.h"

namespace Ullr::Events {

  /// === Abstract Key Event ===
  class ULLR_API KeyEvent : public Event
  {
  public:
    inline int getKeyCode() const { return this->keyCode; }

    EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Keyboard)

  protected:
    KeyEvent(int keyCode)
      : keyCode(keyCode) {}

    int keyCode;
  };

  /// === Key Pressed Event ===
  class ULLR_API KeyPressedEvent : public KeyEvent
  {
  public:
    KeyPressedEvent(int keyCode, int repeatCount)
      : KeyEvent(keyCode), repeatCount(repeatCount) {}

    inline unsigned int getRepeatCount() const { return this->repeatCount; }

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "KeyPressedEvent: " << this->keyCode << "(" << this->repeatCount << " repeats)";
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::KeyPress)

  private:
    int repeatCount;
  };

  /// === Key Released Event ===
  class ULLR_API KeyReleasedEvent : public KeyEvent
  {
  public:
    KeyReleasedEvent(int keyCode)
      : KeyEvent(keyCode) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "KeyReleasedEvent: " << this->keyCode;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::KeyRelease)
  };

  /// === Key Typed Event ===
  class ULLR_API KeyTypedEvent : public KeyEvent
  {
  public:
    KeyTypedEvent(int keyCode)
      : KeyEvent(keyCode) {}

    std::string ToString() const override
    {
      std::stringstream ss;
      ss << "KeyTypedEvent: " << this->keyCode;
      return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::KeyType)
  };

}