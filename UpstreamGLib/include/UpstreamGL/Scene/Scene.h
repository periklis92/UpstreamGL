#pragma once

#include <map>
#include <string>
#include "Node.h"

class Camera;

using NodeContainer = std::map<std::string, Node*>;

class Scene
{
public:
    void Unload();

    Node& CreateNode(const std::string& name = "");
    Node& GetNode(const std::string& name);
    NodeContainer::iterator FindNode(const std::string& name);
    void SetMainCamera(Camera* camera);
    Camera* GetMainCamera();
    
    void Enter();

    void Exit() 
    { 
        for (auto& n: m_Nodes ) n.second->OnExit(); 
    }

private:
    NodeContainer m_Nodes;
    Camera* m_MainCamera{nullptr};
        
};