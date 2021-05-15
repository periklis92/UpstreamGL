#pragma once

#include <filesystem>
#include <string>

template<class ResourceType>
class Resource
{
public:
    Resource(const std::string& name, const std::filesystem::path& path)
        :m_Name(name), m_Path(path), m_ResourceData(nullptr) {}

    Resource() = default;

    void LoadFromFile(const std::string& name, const std::filesystem::path& path) 
    {
        m_Name = name;
        m_Path = path;
        Load();
    }

    Resource(const Resource& other)
    {
        *this = other;
    }

    Resource& operator=(const Resource& other)
    {
        m_ResourceData = other.m_ResourceData;
        m_Name = other.m_Name;
        m_Path = other.m_Path;

        return *this;
    }

    Resource(Resource&&) = default;
    Resource& operator=(Resource&&) = default;
    virtual ~Resource() = default;
    
    virtual void Load() = 0;
    virtual void Destroy() = 0;
    virtual void Unload() = 0;
    virtual bool IsLoaded() const = 0;

    const std::string& GetName() { return m_Name; }
    const std::filesystem::path GetPath() { return m_Path; }

    ResourceType* operator->() { return m_ResourceData; }
    ResourceType& operator*() { return *m_ResourceData; }
    bool operator!() { return !IsLoaded(); }
    bool operator==(const Resource& other) { return m_ResourceData == other.m_ResourceData; }
    
protected:
    std::string m_Name;
    std::filesystem::path m_Path;
    ResourceType* m_ResourceData;
};