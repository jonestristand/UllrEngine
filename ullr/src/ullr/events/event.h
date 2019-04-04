#pragma once

#include "ullrpch.h"
#include "ullr/defines.h"

namespace Ullr::Events {

  enum class EventType {
    None = 0,
    WindowResize, WindowClose,
    KeyPress, KeyRelease, KeyType,
    MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
  };

  enum class EventCategory {
    None = 0,
    Application = BIT(0),
    Input       = BIT(1),
    Keyboard    = BIT(2),
    Mouse       = BIT(3)
  };
  inline EventCategory operator&(EventCategory a, EventCategory b)
  { return static_cast<EventCategory>(static_cast<int32>(a) & static_cast<int32>(b)); }
  inline EventCategory operator|(EventCategory a, EventCategory b)
  { return static_cast<EventCategory>(static_cast<int32>(a) | static_cast<int32>(b)); }

  #define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return type; }\
                                virtual EventType getEventType() const override { return getStaticType(); }\
                                virtual const char* getName() const override { return #type; }

  #define EVENT_CLASS_CATEGORY(category) virtual EventCategory getCategoryFlags() const override { return category; }

  class ULLR_API Event
  {
  public:
    bool Handled = false;

    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    virtual EventCategory getCategoryFlags() const = 0;
    virtual std::string ToString() const { return this->getName(); }

    inline bool IsInCategory(EventCategory category)
    {
      return static_cast<bool>(this->getCategoryFlags() & category);
    }

  };

  class ULLR_API EventDispatcher
  {
    template<typename T>
    using EventFn = std::function<bool(T&)>;

  public:
    EventDispatcher(Event& event)
      :event(event)
    {
    }

    template<typename T>
    bool Dispatch(EventFn<T> func)
    {
      if (this->event.getEventType() == T::getStaticType()) {
        this->event.Handled = func(*(T*)&(this->event));
        return true;
      }
      return false;
    }

  private:
    Event& event;
  };

  inline std::ostream& operator<<(std::ostream& os, const Event& e)
  {
    return os << e.ToString();
  }

}
