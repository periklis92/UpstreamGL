#pragma once

#include <string>

namespace upgl {
struct Event {
  Event() = default;
  virtual ~Event() = default;
  virtual std::string GetEventName() const = 0;
  template <typename EventType> inline const std::decay_t<EventType> *CastTo() const {
    return dynamic_cast<const std::decay_t<EventType> *>((this));
  }
};

#define UPGL_IMPL_EVT_BODY(EventType)                                                              \
public:                                                                                            \
  virtual std::string GetEventName() const override { return #EventType; }                         \
  static std::string GetStaticEventName() { return #EventType; }

struct WindowCloseEvent : public Event {
  UPGL_IMPL_EVT_BODY(WindowCloseEvent)
};

struct WindowResizeEvent : public Event {
  UPGL_IMPL_EVT_BODY(WindowResizeEvent)
  int Width = 0, Height = 0;
  WindowResizeEvent(int w, int h) : Width(w), Height(h) {}
};
} // namespace upgl