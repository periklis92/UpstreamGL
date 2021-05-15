#include "MaterialManager.h"

void MaterialManager::AddMaterial(const std::string& name, Material* material)
{
    m_LoadedMaterials.emplace(name, material);
}

Material* MaterialManager::GetMaterial(const std::string& name)
{
    auto it = m_LoadedMaterials.find(name);
    if (it == m_LoadedMaterials.end()) return nullptr;
    return it->second;
}