#pragma once

#include "Resource.h"

#include <vector>

#include "Graphics/MeshData.h"
struct aiScene;
struct aiMesh;
class Armature;

class MeshResource
    : public Resource
{
public:
    MeshResource(const std::string& name, const std::filesystem::path& path, uint32_t indexInScene = 0)
        :Resource(name, path), m_Mesh(nullptr), m_IndexInScene(indexInScene) { Reload(); }

    MeshResource(MeshResource&& other);
    MeshResource& operator=(MeshResource&& other);
    virtual ~MeshResource();

    virtual void Reload() override;
    virtual void Unload() override;
    virtual void Destroy() override;
    
    virtual bool IsLoaded() const override { return m_Mesh != nullptr; }

    const MeshData* GetMesh() const { return m_Mesh; }
    MeshData* GetMesh() { return m_Mesh; }

private:
    void LoadSimpleMesh(const aiMesh* mesh);
    void LoadSkinnedMesh(const aiMesh* mesh, Armature* armature);
    void LoadMeshes(const aiScene* scene);

private:
    MeshData* m_Mesh;
    uint32_t m_IndexInScene;
};