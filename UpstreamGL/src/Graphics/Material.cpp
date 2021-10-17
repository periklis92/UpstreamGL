#include <UpstreamGL/Graphics/Material.h>
#include <UpstreamGL/Graphics/Shader.h>

void Material::Bind()
{
    m_Shader->Bind();
}