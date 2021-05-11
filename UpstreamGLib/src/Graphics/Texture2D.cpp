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

void Texture2D::LoadFromFile(const std::string& path)
{
    uint8_t* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
    if (!data)
    {
        GLR_LOG_ERROR("Unable to load texture: %s", path.c_str());
        return;
    }
    glGenTextures(1, &m_Handle);
    glBindTexture(GL_TEXTURE_2D, m_Handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}