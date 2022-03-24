#pragma once

#include <cstring>
#include <functional>
#include <map>
#include <variant>

#include <glm/vec4.hpp>

#include <UpstreamGL/Debug/Debug.h>
#include <UpstreamGL/Graphics/Shader.h>
#include <UpstreamGL/Graphics/Texture2D.h>
#include <UpstreamGL/System/ID.h>

namespace upgl {
class MaterialInstance;

class Material {
private:
  Shader m_Shader{};
  Texture2D m_Texture{};

  friend struct std::hash<Material>;

public:
  Material() {}
  Material(const Shader &shader, const Texture2D &texture);

  void Bind();
  const Shader &GetShader() const { return m_Shader; }
  const Texture2D &GetTexture() const { return m_Texture; }
  Shader &GetShader() { return m_Shader; }
  Texture2D &GetTexture() { return m_Texture; }

  void SetShader(const Shader &shader) { m_Shader = shader; }
  void SetTexture(const Texture2D &texture) { m_Texture = texture; }

  bool operator==(const Material &other) const {
    return m_Shader == other.m_Shader && m_Texture == other.m_Texture;
  }

  bool operator<(const Material &other) const {
    return (m_Shader.m_ProgramId + m_Texture.m_Handle) <
           (other.m_Shader.m_ProgramId + other.m_Texture.m_Handle);
  }

  bool HasAlpha() const { return m_Texture.HasAlpha(); }

public:
  MaterialInstance *const CreateInstance();
};

class MaterialInstance {
  enum ValueType { VT_NULL, VT_INT, VT_FLOAT };

  struct MaterialValue {
    std::variant<float *, int *> Value;
    std::size_t Count;
    ValueType Type;

    MaterialValue(const int *v, std::size_t c) : Value(), Count(c), Type(VT_INT) {
      Value = new int[Count];
      memcpy(std::get<int *>(Value), v, sizeof(int) * Count);
    }

    MaterialValue(const float *v, std::size_t c) : Value(), Count(c), Type(VT_FLOAT) {
      Value = new float[Count];
      memcpy(std::get<float *>(Value), v, sizeof(float) * Count);
    }

    MaterialValue(const MaterialValue &) = delete;
    MaterialValue &operator=(const MaterialValue &) = delete;

    MaterialValue(MaterialValue &&other) noexcept { *this = std::move(other); }

    MaterialValue &operator=(MaterialValue &&other) noexcept {
      Value = other.Value;
      Count = other.Count;
      Type = other.Type;
      other.Value = (int *)nullptr;
      other.Type = VT_NULL;
      other.Count = 0;
      return *this;
    }

    ~MaterialValue() {
      if (Type == VT_INT) {
        delete std::get<int *>(Value);
      } else if (Type == VT_FLOAT) {
        delete std::get<float *>(Value);
      }
    }
  };

public:
  MaterialInstance(ID id, Material &material) : m_ID(id), m_Material(material) {}

  void Bind();
  void Apply();

  Material &GetMaterial() const { return m_Material; }

private:
  void __ApplyValue(const std::string &n, const MaterialValue &v);

public:
  template <std::size_t Sz> void SetProperty(const char *name, float (&value)[Sz]);
  template <std::size_t Sz> void SetProperty(const char *name, int (&value)[Sz]);

  const Shader &GetShader() const { return m_Material.GetShader(); }
  const Texture2D &GetTexture() const { return m_Material.GetTexture(); }
  Shader &GetShader() { return m_Material.GetShader(); }
  Texture2D &GetTexture() { return m_Material.GetTexture(); }

  bool HasAlpha() const { return m_Material.HasAlpha(); }

private:
  ID m_ID;
  Material &m_Material;
  std::map<std::string, MaterialValue> m_MValues{};

  friend struct std::hash<MaterialInstance>;
};

template <std::size_t Sz> void MaterialInstance::SetProperty(const char *name, float (&value)[Sz]) {
  m_MValues.emplace(name, MaterialValue{value, Sz});
}

template <std::size_t Sz> void MaterialInstance::SetProperty(const char *name, int (&value)[Sz]) {
  m_MValues.emplace(name, MaterialValue{value, Sz});
}

} // namespace upgl

namespace std {
template <> struct hash<upgl::Material> {
  std::size_t operator()(const upgl::Material &material) const noexcept {
    return hash<upgl::Shader>{}(material.m_Shader) ^
           (hash<upgl::Texture2D>{}(material.m_Texture) << 1);
  }
};

template <> struct hash<upgl::MaterialInstance> {
  std::size_t operator()(const upgl::MaterialInstance &material) const noexcept {
    return hash<upgl::Material>{}(material.m_Material);
  }
};
} // namespace std