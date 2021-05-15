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

ShaderManager& Director::GetShaderManager()
{
    return m_ShaderManager;
}