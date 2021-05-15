#pragma once

#include "Graphics/MeshRenderer.h"
#include "Graphics/GUIRenderer.h"
#include "Scene.h"

class Director
{
public:
    static Director* GetInstance();
    MeshRenderer& GetMeshRenderer();
    GUIRenderer& GetGuiRenderer();
    Scene* const GetScene() { return &m_Scene; }

private:
    Director() = default;
private:
    MeshRenderer m_MeshRenderer;
    GUIRenderer m_GUIRenderer;
    Scene m_Scene;
};