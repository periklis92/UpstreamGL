#include <UpstreamGL/Components/Component.h>
#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/Scene/Node.h>

namespace upgl {
Node::Node(Scene *scene, const ID &id, const std::string &name)
    : m_Scene(scene), m_ID(id), m_Name(name) {}

Node::Node(Node &&node) noexcept
    : m_Scene(node.m_Scene), m_ID(node.m_ID), m_Name(node.m_Name),
      m_ComponentRegistry(std::move(node.m_ComponentRegistry)) {}

Node &&Node::operator=(Node &&node) noexcept {
  m_Name = std::move(node.m_Name);
  m_ComponentRegistry = std::move(node.m_ComponentRegistry);
  m_ID = std::move(node.m_ID);
  m_Scene = node.m_Scene;
  return std::move(*this);
}

bool Node::operator==(const Node &node) const { return m_ID == node.m_ID; }

Node::~Node() {}

void Node::OnEnter() {
  for (auto &c : m_ComponentRegistry) {
    c.second->OnEnter();
  }
}

void Node::OnExit() {
  for (auto &c : m_ComponentRegistry) {
    c.second->OnExit();
  }
}

Transform *Node::GetTransform() { return GetComponent<Transform>(); }

Node *Node::GetParent() {
  auto parTransform = GetTransform()->GetParent();
  if (!parTransform)
    return nullptr;
  return parTransform->GetNode();
}
} // namespace upgl