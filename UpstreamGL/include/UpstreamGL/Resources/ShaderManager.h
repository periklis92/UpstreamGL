#pragma once

#include <map>
#include <string>

#include <UpstreamGL/Graphics/Shader.h>

#define UPGL_MESH_SHADER_NAME "simpleLit"
#define UPGL_SKINNED_MESH_SHADER_NAME "simpleLitSkinned"
#define UPGL_SPRITE_SHADER_NAME "spriteUnlit"
#define UPGL_SPRITE_LIT_SHADER_NAME "spriteLit"

class ShaderManager
{
public:
    void AddShader(const std::string& name, Shader* shader);

private:
    std::map<std::string, Shader*> m_LoadedShaders;
    
};