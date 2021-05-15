#pragma once

#include "Resource.h"

#include <vector>

#include "Graphics/Animation.h"

struct aiScene;
class AnimationClip;

class AnimationResource
    : public Resource<Animation>
{
public:
    AnimationResource(const std::string& name, const std::filesystem::path& path)
        :Resource(name, path) { Load(); }

    virtual void Load() override;
    virtual void Unload() override;
    virtual void Destroy() override;

    virtual bool IsLoaded() const override { return m_ResourceData; }

private:    
    void LoadAnimations(const aiScene* scene);

};
