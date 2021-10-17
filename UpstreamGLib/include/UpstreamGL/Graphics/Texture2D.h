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

struct Texture2DOptions
{
    TextureFilter Filter = TextureFilter::Linear;
    TextureWrap WrapMode = TextureWrap::Repeat;
    TextureFormat Format = TextureFormat::RGBA;
};

class Texture2D
{
public:
    Texture2D(Texture2DOptions options = {});
    ~Texture2D();

    void LoadFromData(const uint8_t* data, int width, int height);

    void SetTextureFilter(TextureFilter filter) { m_Options.Filter = filter; }
    void SetTextureFormat(TextureFormat format) { m_Options.Format = format; }
    void SetTextureWrap(TextureWrap wrap) { m_Options.WrapMode = wrap; }
    void SetTextureOptions(Texture2DOptions options) { m_Options = options; }

    void Bind();
    void Unbind();
    void Delete();
private:
    int m_Width{0}, m_Height{0}, m_Channels{0};
    uint32_t m_Handle{0};
    Texture2DOptions m_Options;
};