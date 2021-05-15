#pragma once

#include <cstdint>
#include <string>

enum class TextureFilter
{
	Linear = 0, Nearest
};

enum class TextureWrap
{
	Repeat = 0, MirroredRepeat, ClampToEdge, ClampToBorder
};

enum class TextureFormat
{
	Red = 0, Green, Blue, Alpha, RGB, RGBA
};

class Texture2D
{
public:
    Texture2D(TextureFilter filter = TextureFilter::Linear, 
        TextureWrap wrap = TextureWrap::Repeat, TextureFormat format = TextureFormat::RGBA);
    ~Texture2D();

    void LoadFromFile(const std::string& path);
    void LoadFromData(const uint8_t* data, int width, int height);

    void SetTextureFilter(TextureFilter filter) { m_Filter = filter; }
    void SetTextureFormat(TextureFormat format) { m_Format = format; }
    void SetTextureWrap(TextureWrap wrap) { m_Wrap = wrap; }

    void Bind();
    void Unbind();

private:
    int m_Width{0}, m_Height{0}, m_Channels{0};
    uint32_t m_Handle{0};
    TextureFilter m_Filter{TextureFilter::Linear};
    TextureWrap m_Wrap{TextureWrap::Repeat};
    TextureFormat m_Format{TextureFormat::RGBA};
};