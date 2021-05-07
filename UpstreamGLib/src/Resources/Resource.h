#pragma once

#include <filesystem>
#include <string>

class Resource
{
public:
    Resource(const std::string& name, const std::filesystem::path& path)
        :m_Name(name), m_Path(path) {}

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource(Resource&&) = default;
    Resource& operator=(Resource&&) = default;
    virtual ~Resource() = default;
    
    virtual void Reload() = 0;
    virtual void Unload() = 0;
    virtual void Destroy() = 0;
    virtual bool IsLoaded() const = 0;

    const std::string& GetName() { return m_Name; }
    const std::filesystem::path GetPath() { return m_Path; }

protected:
    std::string m_Name;
    std::filesystem::path m_Path;
};