#include <UpstreamGL/Resources/TextureResource.h>

#include <UpstreamGL/Debug/Debug.h>
#include <stb/stb_image.h>

void TextureResource::Load()
{
    int width{0}, height{0}, channels{0};

    uint8_t* data = stbi_load(m_Path.string().c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        UPGL_LOG_ERROR("Unable to load texture %s", m_Path.string().c_str());
        return;
    }
        
    m_ResourceData->LoadFromData(data, width, height);
    stbi_image_free(data);
}

void TextureResource::Unload()
{
    m_ResourceData->Delete();
}

void TextureResource::Destroy()
{
    delete m_ResourceData;
}