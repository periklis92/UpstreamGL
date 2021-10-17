#include <UpstreamGL/Resources/MeshResource.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <UpstreamGL/Graphics/Bone.h>
#include <UpstreamGL/Graphics/MeshData.h>
#include <UpstreamGL/Math/AssimpGLMHelper.h>
#include <UpstreamGL/Debug/Debug.h>

void MeshResource::Load()
{
	UPGL_LOG("Loading mesh: %s", m_Name.c_str());
    Unload();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_Path.generic_u8string(), aiProcess_Triangulate | aiProcess_CalcTangentSpace 
            | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_LimitBoneWeights | aiProcess_GenBoundingBoxes);
    
    // assert(scene);
	if (!scene) 
	{
		UPGL_LOG_ERROR("Unable to load %s mesh from %ls", m_Name.c_str(), m_Path.c_str());
		return;
	}

    LoadMeshes(scene);
}

// MeshResource::MeshResource(MeshResource&& other)
// 	:Resource(std::move(other))
// {
// 	*this = std::move(other);
// }

// MeshResource& MeshResource::operator=(MeshResource&& other)
// {
// 	m_Name = other.m_Name;
// 	m_Path = other.m_Path;

// 	m_ResourceData = other.m_ResourceData;
// 	m_IndexInScene = other.m_IndexInScene;
// 	return *this;
// }

MeshResource::~MeshResource()
{
}

void MeshResource::Unload()
{
	
}

void MeshResource::Destroy()
{
	if (m_ResourceData)
    	delete m_ResourceData;
}

void MeshResource::LoadSimpleMesh(const aiMesh* mesh)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces * 3);
	for (uint32_t v = 0; v < mesh->mNumVertices; ++v)
	{
		Vertex vertex;
		vertex.position = { mesh->mVertices[v].x,
			mesh->mVertices[v].y, mesh->mVertices[v].z };

		vertex.normal = { mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z };
		vertex.uv = { mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y };

		if (mesh->mColors[0])
			vertex.color = { mesh->mColors[0][v].r, mesh->mColors[0][v].g, mesh->mColors[0][v].b, mesh->mColors[0][v].a };
		else
			vertex.color = { 1.f, 1.f, 1.f, 1.f };

		vertex.tangent = { mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z };
		vertex.bitangent = { mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z };

		vertices.push_back(vertex);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
		for (uint32_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
			indices.push_back(mesh->mFaces[i].mIndices[j]);

    m_ResourceData = new MeshData(vertices, indices, mesh->mAABB);
}

void MeshResource::LoadSkinnedMesh(const aiMesh* mesh, Armature* armature)
{
	std::vector<SkinnedVertex> vertices;
	std::vector<uint32_t> indices;
    std::vector<VertexWeight>& vertexWeights = armature->GetVertexWeights();
	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces * 3);
	for (uint32_t v = 0; v < mesh->mNumVertices; ++v)
	{
		SkinnedVertex vertex;
		vertex.position = { mesh->mVertices[v].x,
			mesh->mVertices[v].y, mesh->mVertices[v].z };

		vertex.normal = { mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z };
		vertex.uv = { mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y };

		if (mesh->mColors[0])
			vertex.color = { mesh->mColors[0][v].r, mesh->mColors[0][v].g, mesh->mColors[0][v].b, mesh->mColors[0][v].a };
		else
			vertex.color = { 1.f, 1.f, 1.f, 1.f };

		vertex.tangent = { mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z };
		vertex.bitangent = { mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z };

		VertexWeight& bones = vertexWeights[v];
        

		for (uint32_t b = 0; b < bones.NumBones; ++b)
		{
			vertex.boneIds[b] = bones.BoneIds[b];
			vertex.boneWeights[b] = bones.Weight[b];
		}

		vertices.push_back(vertex);
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
		for (uint32_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
			indices.push_back(mesh->mFaces[i].mIndices[j]);

    m_ResourceData = new MeshData(vertices, indices, mesh->mAABB, armature);
}

void MeshResource::LoadMeshes(const aiScene* scene)
{
	if (scene->mMeshes[m_IndexInScene]->HasBones())
	{
		std::vector<VertexWeight> vertexWeights;
		Armature* armature = new Armature(scene->mRootNode, scene->mMeshes[m_IndexInScene]);
		LoadSkinnedMesh(scene->mMeshes[m_IndexInScene], armature);
	}
	else
		LoadSimpleMesh(scene->mMeshes[m_IndexInScene]);
}