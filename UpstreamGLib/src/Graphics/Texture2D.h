#pragma once

#include <cstdint>
#include <string>

enum class TextureFilter
{
	Linear, Nearest
};

enum class TextureWrap
{
	Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
};

enum class TextureFormat
{
	Red, Green, Blue, Alpha, RGB, RGBA
};

class Texture2D
{
public:
    Texture2D() = default;
    
    void LoadFromFile(const std::string& path);

private:
    int m_Width{0}, m_Height{0}, m_Channels{0};
    uint32_t m_Handle{0};
};