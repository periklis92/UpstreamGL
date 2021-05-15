#pragma once

#include "Resource.h"

#include <vector>

#include "Graphics/MeshData.h"
struct aiScene;
struct aiMesh;
class Armature;

class MeshResource
    : public Resource<MeshData>
{
public:
    MeshResource(const std::string& name, const std::filesystem::path& path, uint32_t indexInScene = 0)
        :Resource(name, path), m_IndexInScene(indexInScene) { Load(); }
    MeshResource(): Resource() {  }
    // MeshResource(MeshResource&& other);
    // MeshResource& operator=(MeshResource&& other);
    virtual ~MeshResource();

    virtual void Load() override;
    virtual void Unload() override;
    virtual void Destroy() override;
    
    virtual bool IsLoaded() const override { return m_ResourceData != nullptr; }

private:
    void LoadSimpleMesh(const aiMesh* mesh);
    void LoadSkinnedMesh(const aiMesh* mesh, Armature* armature);
    void LoadMeshes(const aiScene* scene);

private:
    uint32_t m_IndexInScene;
};