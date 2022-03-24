#pragma once

#include <string>

namespace upgl {
class Node;
class Transform;

class Component {
public:
  Component(Node *node);

  virtual ~Component() = default;

  virtual void OnEnter() {}
  virtual void OnExit() {}

  const Node *GetNode() const { return m_Node; }
  Node *GetNode() { return m_Node; }

  virtual const std::string GetComponentName() const = 0;

  /*template<class T>
  inline auto AddComponent() { return m_Node->AddComponent<T>(); }

  template<class T>
  inline auto GetComponent() { return m_Node->GetComponent<T>(); }*/

  // const std::string& GetName() const { return m_Node->GetName(); }
  /*Transform* GetTransform();
  Transform* GetParent();*/

protected:
  Node *m_Node{nullptr};
};
} // namespace upgl