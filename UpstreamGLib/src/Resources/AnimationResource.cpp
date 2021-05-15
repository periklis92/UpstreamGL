#include "AnimationResource.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Graphics/Animation.h"
#include "Math/AssimpGLMHelper.h"
#include "Debug/Debug.h"

void AnimationResource::Load()
{
    GLR_LOG("Loading Animation: %s", m_Name.c_str());
    Unload();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_Path.generic_u8string(), 0);
    
    assert(scene);
    
    LoadAnimations(scene);
}

void AnimationResource::Destroy()
{
    delete m_ResourceData;
}


void AnimationResource::Unload()
{
    m_ResourceData->Clear();
}

void AnimationResource::LoadAnimations(const aiScene* scene)
{
    for (uint32_t i = 0; i < scene->mNumAnimations; ++i)
    {
        m_ResourceData->AddClip(scene->mAnimations[i]->mName.C_Str(), AnimationClip(scene->mAnimations[i]->mName.C_Str(), scene->mAnimations[i]));
    }
}