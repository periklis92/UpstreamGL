#include <UpstreamGL/Scene/Scene.h>

#include <algorithm>

#include <UpstreamGL/Components/Camera.h>

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
    auto& node = *m_Nodes.emplace(name, new Node(name)).first->second;
    node.AddComponent<Transform>();
    return node;
}

void Scene::Enter()
{
    for (auto& n: m_Nodes ) n.second->OnEnter();
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