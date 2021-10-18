#include <UpstreamGL/Scene/Scene.h>

#include <algorithm>

#include <UpstreamGL/Components/Camera.h>

void Scene::Unload()
{
    m_Nodes.clear();
}

Node& Scene::CreateNode(const std::string& name)
{
    auto& node = m_Nodes.emplace_back(std::move(Node(name)));
    node.AddComponent<Transform>();
    return node;
}

void Scene::Enter()
{
    for (auto& n: m_Nodes ) n.OnEnter();
}

void Scene::Exit() 
{ 
    for (auto& n: m_Nodes ) n.OnExit(); 
}

NodeContainer::iterator Scene::FindNode(const std::string& name)
{
    return std::find(m_Nodes.begin(), m_Nodes.end(), [name](Node& n){return n.GetName() == name; });
}

void Scene::SetMainCamera(Camera* camera)
{
    m_MainCamera = camera;
}

Camera* Scene::GetMainCamera()
{
    return m_MainCamera;
}