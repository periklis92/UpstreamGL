#pragma once

#include "../Graphics/MeshRenderer.h"
#include "Scene.h"

class Director
{
public:
    static Director* GetInstance();
    MeshRenderer& GetMeshRenderer();
    Scene* const GetScene() { return &m_Scene; }

private:
    Director() = default;
private:
    MeshRenderer m_MeshRenderer;
    Scene m_Scene;
};