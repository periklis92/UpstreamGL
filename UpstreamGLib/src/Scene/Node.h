#pragma once

#include <list>
#include <cstdint>
#include <string>
#include <cassert>

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/ext.hpp"

#include "Components/ComponentRegistry.h"

class Node
{
public:
	Node(const std::string& name = "");
	virtual ~Node();

	virtual void OnEnter();
	virtual void OnExit();

	void AddChild(Node* const& child) { assert(child); child->m_Parent = this; m_Children.push_back(child); }
	void RemoveChild(Node* const& child) { m_Children.remove(child); }
	const std::list<Node*>& GetChildren() const { return m_Children; }
	std::list<Node*>& GetChildren() { return m_Children; }
	size_t NumChildren() const { return m_Children.size(); }

	Node* FindChildWithName(const std::string& name);

	void SetName(const std::string& name) { m_Name = name; }
	const std::string& GetName() const { return m_Name; }

	void SetLocalTransform(const glm::mat4& transform);
	void SetWorldTransform(const glm::mat4& transform);

	void SetLocalPosition(glm::vec3 position);
	void SetLocalRotation(glm::quat rotation);
	void SetLocalRotation(glm::vec3 rotation);
	void SetLocalScale(glm::vec3 scale);

	void SetWorldPosition(glm::vec3 position);
	void SetWorldRotation(glm::quat rotation);
	void SetWorldRotation(glm::vec3 rotation);
	void SetWorldScale(glm::vec3 scale);

	void Translate(glm::vec3 translation);
	void Rotate(float angle, glm::vec3 axis);
	void Rotate(glm::vec3 eulerAngles);
	void Scale(glm::vec3 scale);

	const glm::mat4 GetLocalTransform() const;
	const glm::vec3 GetLocalPosition() const;
	const glm::quat GetLocalRotationQuat() const;
	const glm::vec3 GetLocalRotation() const;
	const glm::vec3 GetLocalScale() const;

	const glm::mat4 GetWorldTransform() const;
	const glm::vec3 GetWorldPosition() const;
	const glm::quat GetWorldRotationQuat() const;
	const glm::vec3 GetWorldRotation() const;
	const glm::vec3 GetWorldScale() const;

	const glm::vec3 GetForwardDirection() const;
	const glm::vec3 GetUpDirection() const;
	const glm::vec3 GetRightDirection() const;

	template<class T>
	inline auto AddComponent() { return m_ComponentRegistry.AddComponent<T>(this); }
	
	inline auto AddComponent(const char* name) { return m_ComponentRegistry.AddComponent(name, this); }

	template<class T>
	inline auto GetComponent() { return m_ComponentRegistry.GetComponent<T>(); }

private:
	Node* m_Parent;
	std::list<Node*> m_Children;
	std::string m_Name;
	glm::vec3 m_LocalPosition;
	glm::quat m_LocalRotation;
	glm::vec3 m_LocalScale;
	glm::mat4 m_LocalTransform;

	ComponentRegistry m_ComponentRegistry;
};