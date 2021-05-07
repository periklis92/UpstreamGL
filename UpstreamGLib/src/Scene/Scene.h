#pragma once

#include <map>
#include <string>
#include "Node.h"

class Camera;

class Scene
{
public:
    Node& CreateNode(const std::string& name = "");
    Node& GetNode(const std::string& name);
    void SetMainCamera(Camera* camera);
    Camera* GetMainCamera();
    void Enter() { for (auto& n: m_Nodes ) { n.second->OnEnter();} }
private:
    std::map<std::string, Node*> m_Nodes;
    Camera* m_MainCamera{nullptr};
};