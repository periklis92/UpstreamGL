#pragma once

#include <cassert>
#include <cstdint>
#include <list>
#include <string>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <UpstreamGL/Components/ComponentRegistry.h>
#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/System/Delegate.h>
#include <UpstreamGL/System/Event.h>
#include <UpstreamGL/System/EventDispatcher.h>
#include <UpstreamGL/System/ID.h>

namespace upgl {
class Node;
class Scene;
class Transform;

class Node {
public:
  Node(Scene *scene, const ID &id, const ::std::string &name = "");

  Node(Node &&node) noexcept;
  Node &&operator=(Node &&node) noexcept;

  Node(const Node &node) = delete;
  Node &operator=(const Node &node) = delete;

  bool operator==(const Node &node) const;

  virtual ~Node();

  virtual void OnEnter();
  virtual void OnExit();

  void SetName(const ::std::string &name) { m_Name = name; }

  template <class T> inline auto AddComponent() {
    return m_ComponentRegistry.AddComponent<T>(this);
  }
  // inline auto AddComponent(const char* name) { return
  // m_ComponentRegistry.AddComponent(name, this); }

  template <class T> inline bool HasComponent() { return m_ComponentRegistry.HasComponent<T>(); }

  template <class T> inline auto GetComponent() { return m_ComponentRegistry.GetComponent<T>(); }

  template <class T> inline T *FindComponentInParent() {
    auto parent = GetParent();
    T *component = nullptr;
    while (parent && !component) {
      if (parent->HasComponent<T>())
        component = parent->GetComponent<T>();
    }
    return component;
  }

  const ::std::string &GetName() const { return m_Name; }
  Transform *GetTransform();
  Node *GetParent();
  Scene *GetScene() { return m_Scene; }
  const ID &GetID() const { return m_ID; }

private:
  Scene *m_Scene;
  ID m_ID;
  ::std::string m_Name;
  ComponentRegistry m_ComponentRegistry{};
};
} // namespace upgl