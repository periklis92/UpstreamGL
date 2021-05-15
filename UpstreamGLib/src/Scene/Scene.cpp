#include "Scene.h"
#include <algorithm>

void Scene::Unload()
{
    for (auto node: m_Nodes)
    {
        delete node.second;
    }
    m_Nodes.clear();
}

Node& Scene::CreateNode(const std::string& name)
{
    return *m_Nodes.emplace(name, new Node(name)).first->second;
}

Node& Scene::GetNode(const std::string& name)
{
    return *m_Nodes[name];
}

NodeContainer::iterator Scene::FindNode(const std::string& name)
{
    return m_Nodes.find(name);
}

void Scene::SetMainCamera(Camera* camera)
{
    m_MainCamera = camera;
}

Camera* Scene::GetMainCamera()
{
    return m_MainCamera;
}