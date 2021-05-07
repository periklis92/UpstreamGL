#pragma once

#include <map>
#include <string>
#include "Node.h"

class Camera;

class Scene
{
    using NodeContainer = std::map<std::string, Node*>;
public:
    Node& CreateNode(const std::string& name = "");
    Node& GetNode(const std::string& name);
    NodeContainer::iterator FindNode(const std::string& name);
    void SetMainCamera(Camera* camera);
    Camera* GetMainCamera();
    void Enter() { for (auto& n: m_Nodes ) { n.second->OnEnter();} }
private:
    NodeContainer m_Nodes;
    Camera* m_MainCamera{nullptr};
};