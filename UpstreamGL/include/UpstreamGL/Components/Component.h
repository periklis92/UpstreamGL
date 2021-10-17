#pragma once

#include <string>

class Node;

class Component
{
public:
    Component(Node* node);
    virtual ~Component() = default;
    
    virtual void OnEnter() {}
    virtual void OnExit() {}

    const Node* GetNode() const { return m_Node; }
    Node* GetNode() { return m_Node; }

    virtual const std::string GetComponentName() const = 0;

protected:
    Node* m_Node{nullptr};
};