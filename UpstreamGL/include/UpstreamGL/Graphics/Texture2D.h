#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace upgl {
enum class TextureFilter { Linear = 0, Nearest };

enum class TextureWrap { Repeat = 0, MirroredRepeat, ClampToEdge, ClampToBorder };

enum class TextureFormat { Red = 0, Green, Blue, Alpha, RGB, RGBA };

struct Texture2DOptions {
  TextureFilter Filter = TextureFilter::Linear;
  TextureWrap WrapMode = TextureWrap::Repeat;
  TextureFormat Format = TextureFormat::RGBA;
  TextureFormat InternalFormat = TextureFormat::RGBA;
  bool HasAlpha = false;

  Texture2DOptions(TextureFilter filter = TextureFilter::Linear,
                   TextureWrap wrapMode = TextureWrap::Repeat,
                   TextureFormat format = TextureFormat::RGBA,
                   TextureFormat internalFormat = TextureFormat::RGBA, bool hasAlpha = false)
      : Filter(filter), WrapMode(wrapMode), Format(format), InternalFormat(internalFormat),
        HasAlpha(hasAlpha) {}
};

class TextureResource;

class Texture2D {
public:
  Texture2D();
  ~Texture2D() = default;

  void LoadFromData(const uint8_t *data, int width, int height, Texture2DOptions options = {});

  bool operator==(const Texture2D &other) const { return m_Handle == other.m_Handle; }
  bool operator<(const Texture2D &other) const { return m_Handle < other.m_Handle; }

  void SetHasAlpha(bool hasAlpha) { m_HasAlpha = hasAlpha; }
  bool HasAlpha() const { return m_HasAlpha; }

  void Bind();
  void Delete();

private:
  uint32_t m_Handle{0};
  bool m_HasAlpha{false};

  friend class Material;
  friend class TextureResource;
  friend struct ::std::hash<Texture2D>;
};
} // namespace upgl

namespace std {
template <> struct hash<upgl::Texture2D> {
  size_t operator()(const upgl::Texture2D &texture) const noexcept {
    return hash<uint32_t>{}(texture.m_Handle);
  }
};
} // namespace std
