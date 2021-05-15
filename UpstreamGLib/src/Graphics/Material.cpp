#include "Material.h"
#include "Shader.h"

void Material::Bind()
{
    m_Shader->Bind();
}