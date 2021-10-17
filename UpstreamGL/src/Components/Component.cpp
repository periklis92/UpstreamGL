#include <UpstreamGL/Components/Component.h>

#include <cassert>

Component::Component(Node* node)
    :m_Node(node)
{
    assert(node);
}