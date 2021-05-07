#pragma once

#include <typeindex>
#include <type_traits>
#include <map>

#include "Component.h"

class Node;

class ComponentRegistry
{
public:
    ComponentRegistry() = default;
    ComponentRegistry(const ComponentRegistry&) = delete;
    ComponentRegistry(ComponentRegistry&&) = default;
    ComponentRegistry& operator=(const ComponentRegistry&) = delete;
    ComponentRegistry& operator=(ComponentRegistry&&) = default;

    ~ComponentRegistry()
    {
        for (auto it: m_Components)
        {
            if (it.second)
                delete it.second;
        }
    }

    // template<class T, class...Args>
    // inline auto AddComponent(Args&&...args) { return m_Components.emplace(typeid(T), new T(std::forward<Args>(args)...)); }

    template<class T>
    auto AddComponent(Node* node) { return dynamic_cast<T*>(m_Components.emplace(typeid(T), new T(node)).first->second); }

    Component* AddComponent(std::type_index& type, Component* component) { return m_Components.emplace(type, component).first->second; }
    
    auto HasComponent(std::type_index& type) { return m_Components.find(type) != m_Components.end(); }
    
    Component* AddComponent(const char* name, Node* node)
    {
        auto type = m_ComponentTypeInfo.find(name);
        auto ctor = m_ComponentConstructors.find(type->second);
        if (ctor != m_ComponentConstructors.end() && !HasComponent(type->second))
        {
            return AddComponent(type->second, ctor->second(node));
        }

        return nullptr;
    }

    template<class T>
    auto HasComponent() { return m_Components.find(typeid(T)) != m_Components.end(); }
    
    
    template<class T>
    auto GetComponent() { if (HasComponent<T>()) return dynamic_cast<T*>(m_Components.find(typeid(T))->second); return static_cast<T*>(nullptr); }

    auto begin() { return m_Components.begin(); }
    auto end() { return m_Components.end(); }

    template<class T>
    static void RegisterComponent(const char* name) 
    { 
        m_ComponentConstructors.emplace(typeid(T), [](Node* node) -> Component* { return new T(node); });
        m_ComponentTypeInfo.emplace(name, typeid(T));
    }

private:
    std::map<std::type_index, Component*> m_Components;
    static inline std::map<std::string, std::type_index> m_ComponentTypeInfo;
    static inline std::map<std::type_index, Component*(*)(Node*)> m_ComponentConstructors;
};