#include "Director.h"

Director* Director::GetInstance()
{
    static Director __director;
    return &__director;
}

MeshRenderer& Director::GetMeshRenderer()
{
    return m_MeshRenderer;
}

GUIRenderer& Director::GetGuiRenderer()
{
    return m_GUIRenderer;
}

ShaderManager& Director::GetShaderManager()
{
    return m_ShaderManager;
}