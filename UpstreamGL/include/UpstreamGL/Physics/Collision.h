#pragma once

#include <UpstreamGL/System/Event.h>
#include <UpstreamGL/System/EventDispatcher.h>

namespace upgl {
class Node;

enum class CollisionType { Enter, Stay, Exit };

struct CollisionEvent : public Event {
  UPGL_IMPL_EVT_BODY(CollisionEvent);

  CollisionEvent(Node *other, CollisionType type) : Other(other), ColType(type) {}

  Node *Other;
  CollisionType ColType;
};

using CollisionEventDelegate = EventDelegate<CollisionEvent>;

} // namespace upgl