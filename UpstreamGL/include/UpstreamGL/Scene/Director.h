#pragma once

#include <UpstreamGL/Graphics/MeshRenderer.h>
#include <UpstreamGL/Resources/ShaderManager.h>
#include "Scene.h"

class Director
{
public:
    static Director* GetInstance();
    MeshRenderer& GetMeshRenderer();
    ShaderManager& GetShaderManager();
    Scene* const GetScene() { return &m_Scene; }

private:
    Director() = default;
private:
    MeshRenderer m_MeshRenderer{};
    Scene m_Scene{};
    ShaderManager m_ShaderManager{};
};