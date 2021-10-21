#pragma once

#include <list>
#include <cstdint>
#include <string>
#include <cassert>

#include <glm/glm.hpp>
#include "glm/gtx/quaternion.hpp"
#include "glm/ext.hpp"

#include <UpstreamGL/Components/ComponentRegistry.h>
#include <UpstreamGL/Components/Transform.h>

class Node
{
public:
	Node(const std::string& name = "");

	Node(Node&& node);
	Node&& operator=(Node&& node);

	Node(const Node& node) = delete;
	Node& operator=(const Node& node) = delete;
	
	bool operator==(const Node& node) const;

	virtual ~Node();

	virtual void OnEnter();
	virtual void OnExit();

	void SetName(const std::string& name) { m_Name = name; }
	const std::string& GetName() const { return m_Name; }

	Transform* GetTransform() { return GetComponent<Transform>(); }

	template<class T>
	inline auto AddComponent() { return m_ComponentRegistry.AddComponent<T>(this); }
	
	inline auto AddComponent(const char* name) { return m_ComponentRegistry.AddComponent(name, this); }

	template<class T>
	inline auto GetComponent() { return m_ComponentRegistry.GetComponent<T>(); }

private:
	std::string m_Name;
	ComponentRegistry m_ComponentRegistry;
};