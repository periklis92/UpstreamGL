#pragma once

#include "Graphics/Material.h"

#include <map>
#include <string>

class MaterialManager
{
public:
    void AddMaterial(const std::string& name, Material* material);
    void CreateMaterial(const std::string name, Shader* shader);
    Material* GetMaterial(const std::string& name);
private:
    std::map<std::string, Material*> m_LoadedMaterials;
};