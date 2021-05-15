#include "Texture2D.h"
#include "Debug/Debug.h"
#include <glad/glad.h>
#include <stb/stb_image.h>


static inline uint32_t TextureFilterToGL(TextureFilter filter)
{
    switch(filter)
    {
        case TextureFilter::Linear:
            return GL_LINEAR;
        default:
            return GL_NEAREST;
    }
}

static inline uint32_t TextureWrapToGL(TextureWrap wrap)
{
    switch(wrap)
    {
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

static inline uint32_t TextureFormatToGL(TextureFormat format)
{
    switch(format)
    {
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

Texture2D::Texture2D(TextureFilter filter, TextureWrap wrap, TextureFormat format)
    :m_Filter(filter), m_Wrap(wrap), m_Format(format)
{
    
}

Texture2D::~Texture2D()
{
    if (m_Handle)
        glDeleteTextures(1, &m_Handle);
}

void Texture2D::LoadFromFile(const std::string& path)
{
    int width{0}, height{0};
    uint8_t* data = stbi_load(path.c_str(), &width, &height, &m_Channels, 0);
    if (!data)
    {
        GLR_LOG_ERROR("Unable to load texture %s", path.c_str());
        return;
    }
    LoadFromData(data, width, height);
    stbi_image_free(data);
}

void Texture2D::LoadFromData(const uint8_t* data, int width, int height)
{
    if (!data || ! width || !height)
    {
        GLR_LOG_ERROR("Unable to load texture from data!");
        return;
    }
    glGenTextures(1, &m_Handle);
    glBindTexture(GL_TEXTURE_2D, m_Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapToGL(m_Wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapToGL(m_Wrap));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterToGL(m_Filter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterToGL(m_Filter));

    glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToGL(m_Format), m_Width, m_Height, 0, TextureFormatToGL(m_Format), GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind()
{
    glBindTexture(GL_TEXTURE_2D, m_Handle);
}

void Texture2D::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, m_Handle);
}