#include "ShaderManager.h"


void ShaderManager::AddShader(const std::string& name, Shader* shader)
{
    m_LoadedShaders.emplace(name, shader);
}