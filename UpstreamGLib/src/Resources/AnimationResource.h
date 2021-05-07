#pragma once

#include "Resource.h"

#include <vector>

#include "Graphics/Animation.h"

struct aiScene;
class AnimationClip;

class AnimationResource
    : public Resource
{
public:
    AnimationResource(const std::string& name, const std::filesystem::path& path)
        :Resource(name, path) { Reload(); }

    virtual void Reload() override;
    virtual void Unload() override;
    virtual void Destroy() override;

    virtual bool IsLoaded() const override { return m_Clips.size() > 0; }

    size_t GetNumClips() const { return m_Clips.size(); }
    const std::vector<AnimationClip>& GetClips() const { return m_Clips; }

private:    
    void LoadAnimations(const aiScene* scene);

private:
    std::vector<AnimationClip> m_Clips;
};
