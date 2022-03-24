#include <cassert>

#include <UpstreamGL/Components/Component.h>
#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/Debug/Debug.h>

namespace upgl {
Component::Component(Node *node) : m_Node(node) { UPGL_ASSERT(node); }

/*Transform* Component::GetTransform() { return GetComponent<Transform>(); }
Transform* Component::GetParent() { return
GetComponent<Transform>()->GetParent(); }*/
} // namespace upgl