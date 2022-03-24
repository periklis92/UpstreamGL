#pragma once

#include <UpstreamGL/Components/Component.h>
#include <map>
#include <memory>
#include <type_traits>
#include <typeindex>

namespace upgl {
class Node;

class ComponentRegistry {
public:
  ComponentRegistry() = default;
  ComponentRegistry(const ComponentRegistry &) = delete;
  ComponentRegistry &operator=(const ComponentRegistry &) = delete;

  ComponentRegistry(ComponentRegistry &&other) { other.m_Components = std::move(m_Components); }

  ComponentRegistry &operator=(ComponentRegistry &&other) {
    other.m_Components = std::move(m_Components);
    return *this;
  }

  ~ComponentRegistry() { m_Components.clear(); }

  template <class T> auto AddComponent(Node *node) {
    return dynamic_cast<T *>(
        m_Components.emplace(typeid(T), ::std::make_unique<T>(node)).first->second.get());
  }

  // Component* AddComponent(::std::type_index& type, Component* component) {
  // return m_Components.emplace(type,
  // ::std::make_unique(component)).first->second; }

  auto HasComponent(::std::type_index &type) {
    return m_Components.find(type) != m_Components.end();
  }

  /*Component* AddComponent(const char* name, Node* node)
  {
      auto type = m_ComponentTypeInfo.find(name);
      auto ctor = m_ComponentConstructors.find(type->second);
      if (ctor != m_ComponentConstructors.end() && !HasComponent(type->second))
      {
          return AddComponent(type->second, ctor->second(node));
      }

      return nullptr;
  }*/

  template <class T> auto HasComponent() {
    return m_Components.find(typeid(T)) != m_Components.end();
  }

  template <class T> auto GetComponent() {
    if (HasComponent<T>())
      return dynamic_cast<T *>(m_Components.find(typeid(T))->second.get());
    return static_cast<T *>(nullptr);
  }

  auto begin() { return m_Components.begin(); }
  auto end() { return m_Components.end(); }

  template <class T> static void RegisterComponent(const char *name) {
    m_ComponentConstructors.emplace(typeid(T),
                                    [](Node *node) -> Component * { return new T(node); });
    m_ComponentTypeInfo.emplace(name, typeid(T));
  }

private:
  ::std::map<::std::type_index, ::std::unique_ptr<Component>> m_Components;
  static inline ::std::map<::std::string, ::std::type_index> m_ComponentTypeInfo;
  static inline ::std::map<::std::type_index, Component *(*)(Node *)> m_ComponentConstructors;
};
} // namespace upgl