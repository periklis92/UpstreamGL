#include <glad/glad.h>

#include <UpstreamGL/Debug/Debug.h>
#include <UpstreamGL/Graphics/Texture2D.h>

namespace upgl {
static inline uint32_t TextureFilterToGL(TextureFilter filter) {
  switch (filter) {
  case TextureFilter::Linear:
    return GL_LINEAR;
  default:
    return GL_NEAREST;
  }
}

static inline uint32_t TextureWrapToGL(TextureWrap wrap) {
  switch (wrap) {
  case TextureWrap::ClampToBorder:
    return GL_CLAMP_TO_BORDER;
  case TextureWrap::ClampToEdge:
    return GL_CLAMP_TO_EDGE;
  case TextureWrap::Repeat:
    return GL_REPEAT;
  default:
    return GL_MIRRORED_REPEAT;
  }
}

static inline uint32_t TextureFormatToGL(TextureFormat format) {
  switch (format) {
  case TextureFormat::Red:
    return GL_RED;
  case TextureFormat::Green:
    return GL_GREEN;
  case TextureFormat::Blue:
    return GL_BLUE;
  case TextureFormat::Alpha:
    return GL_ALPHA;
  case TextureFormat::RGB:
    return GL_RGB;
  default:
    return GL_RGBA;
  }
}

Texture2D::Texture2D() {}

void Texture2D::LoadFromData(const uint8_t *data, int width, int height, Texture2DOptions options) {
  m_HasAlpha = options.HasAlpha;
  if (!data || !width || !height) {
    UPGL_LOG_ERROR("Unable to load texture from data!");
    return;
  }
  glGenTextures(1, &m_Handle);
  glBindTexture(GL_TEXTURE_2D, m_Handle);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapToGL(options.WrapMode));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapToGL(options.WrapMode));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterToGL(options.Filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterToGL(options.Filter));

  glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToGL(options.InternalFormat), width, height, 0,
               TextureFormatToGL(options.Format), GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() { glBindTexture(GL_TEXTURE_2D, m_Handle); }

void Texture2D::Delete() {
  if (m_Handle) {
    glDeleteTextures(1, &m_Handle);
    m_Handle = 0;
  }
}
} // namespace upgl