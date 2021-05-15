#pragma once

#include "Resource.h"
#include "Graphics/Texture2D.h"

class TextureResource
    : public Resource<Texture2D>
{
public:
    TextureResource(const std::string& name, const std::filesystem::path& path, Texture2DOptions options = {})
        :Resource(name, path) 
    { 
        m_ResourceData = new Texture2D(options);
        Load(); 
    }

    TextureResource(Texture2DOptions options = {}): Resource() { m_ResourceData = new Texture2D(options); }
    
    virtual void Load() override;
    virtual void Unload() override;
    virtual void Destroy() override;

    void SetTextureOptions(Texture2DOptions options) { if (m_ResourceData) m_ResourceData->SetTextureOptions(options); }
    
    void LoadFromData(const uint8_t* data, int width, int height) { m_ResourceData->LoadFromData(data, width, height); }

    virtual bool IsLoaded() const override { return m_ResourceData != nullptr; }
    
};