#pragma once

#include <list>
#include <cstdint>
#include <string>
#include <cassert>

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/ext.hpp"

#include "Component.h"

class Transform 
    : public Component
{
public:
    Transform(Node* node)
    :Component(node) {}

    virtual const std::string GetComponentName() const override { return "Transform"; }


    void AddChild(Transform* const& child) { assert(child); child->m_Parent = this; m_Children.push_back(child); }
	void RemoveChild(Transform* const& child) { m_Children.remove(child); }
	const std::list<Transform*>& GetChildren() const { return m_Children; }
	std::list<Transform*>& GetChildren() { return m_Children; }
	size_t NumChildren() const { return m_Children.size(); }

	Transform* GetChildWithName(const std::string& name);

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

private:    
    Transform* m_Parent{nullptr};
	std::list<Transform*> m_Children{};
    
	glm::vec3 m_LocalPosition{0, 0, 0};
	glm::quat m_LocalRotation{glm::identity<glm::quat>()};
	glm::vec3 m_LocalScale{1, 1, 1};
};