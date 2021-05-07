#include "Scene.h"
#include <algorithm>

Node& Scene::CreateNode(const std::string& name)
{
    return *m_Nodes.emplace(name, new Node(name)).first->second;
}

Node& Scene::GetNode(const std::string& name)
{
    // return *std::find(m_Nodes.begin(), m_Nodes.end(), [name](Node& node) -> bool{ return node.GetName() == name; });
    return *m_Nodes[name];
}

void Scene::SetMainCamera(Camera* camera)
{
    m_MainCamera = camera;
}

Camera* Scene::GetMainCamera()
{
    return m_MainCamera;
}