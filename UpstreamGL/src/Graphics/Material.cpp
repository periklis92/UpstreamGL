#include <cstddef>
#include <glad/glad.h>

#include <UpstreamGL/Graphics/Material.h>

namespace upgl {
Material::Material(const Shader &shader, const Texture2D &texture)
    : m_Shader(shader), m_Texture(texture) {}

void Material::Bind() {
  m_Shader.Bind();
  glActiveTexture(GL_TEXTURE0);
  m_Shader.SetInt(UPGL_SHADER_MAIN_TEX_NAME, 0);
  m_Texture.Bind();
}

MaterialInstance *const Material::CreateInstance() {
  return new MaterialInstance(ID::Generate(), *this);
}

void MaterialInstance::Bind() { m_Material.Bind(); }

void MaterialInstance::Apply() {
  for (auto &v : m_MValues) {
    __ApplyValue(v.first, v.second);
  }
}

void MaterialInstance::__ApplyValue(const std::string &n, const MaterialValue &v) {
  switch (v.Type) {
  case VT_FLOAT:
    m_Material.GetShader().SetFloat(n.c_str(), std::get<float *>(v.Value), v.Count);
    break;
  case VT_INT:
    m_Material.GetShader().SetInt(n.c_str(), std::get<int *>(v.Value), v.Count);
    break;
  default:
    break;
  }
}

} // namespace upgl