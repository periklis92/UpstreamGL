#pragma once

#include <map>
#include <string>

#include "Graphics/Shader.h"

#define GLR_MESH_SHADER_NAME "simpleLit"
#define GLR_SKINNED_MESH_SHADER_NAME "simpleLitSkinned"
#define GLR_SPRITE_SHADER_NAME "spriteUnlit"
#define GLR_SPRITE_LIT_SHADER_NAME "spriteLit"

class ShaderManager
{
public:
    void AddShader(const std::string& name, Shader* shader);

private:
    std::map<std::string, Shader*> m_LoadedShaders;
    
};