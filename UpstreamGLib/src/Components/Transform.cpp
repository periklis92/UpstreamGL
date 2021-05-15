#include "Transform.h"

#include "Math/Math.h"
#include <glm/gtx/euler_angles.hpp>
#include <algorithm>


Transform* Transform::GetChildWithName(const std::string& name)
{
	for(auto child: m_Children)
	{
		if (auto c = child->GetChildWithName(name))
		{
			return c;
		}
	}
	return nullptr;
}

void Transform::SetLocalTransform(const glm::mat4& transform) 
{ 
	m_LocalPosition = Math::GetTranslation(transform);
	m_LocalRotation = Math::GetRotationQuat(transform);
	m_LocalScale = Math::GetScale(transform);
}

void Transform::SetWorldTransform(const glm::mat4& transform) 
{ 
	SetWorldPosition(Math::GetTranslation(transform));
	SetWorldRotation(Math::GetRotationQuat(transform));
	SetWorldScale(Math::GetScale(transform));
}

void Transform::SetLocalPosition(glm::vec3 position)
{
	m_LocalPosition = position;
}

void Transform::SetLocalRotation(glm::quat rotation)
{
	m_LocalRotation = rotation;
}

void Transform::SetLocalRotation(glm::vec3 rotation)
{
	m_LocalRotation = glm::identity<glm::quat>();
	m_LocalRotation = glm::rotate(m_LocalRotation, rotation.x, glm::vec3(1, 0, 0));
	m_LocalRotation = glm::rotate(m_LocalRotation, rotation.y, glm::vec3(0, 1, 0));
	m_LocalRotation = glm::rotate(m_LocalRotation, rotation.z, glm::vec3(0, 0, 1));
}

void Transform::SetLocalScale(glm::vec3 scale)
{
	m_LocalScale = scale;
}

void Transform::SetWorldPosition(glm::vec3 position)
{
	m_LocalPosition = position - (GetWorldPosition() - m_LocalPosition);
}

void Transform::SetWorldRotation(glm::quat rotation)
{
	m_LocalRotation = rotation * (rotation * glm::inverse(GetWorldRotationQuat() * glm::inverse(m_LocalRotation)));
}

void Transform::SetWorldRotation(glm::vec3 rotation)
{
	auto rotQ = glm::identity<glm::quat>();
	rotQ = glm::rotate(rotQ, rotation.x, glm::vec3(1, 0, 0));
	rotQ = glm::rotate(rotQ, rotation.y, glm::vec3(0, 1, 0));
	rotQ = glm::rotate(rotQ, rotation.z, glm::vec3(0, 0, 1));
	SetWorldRotation(rotQ);
}

void Transform::SetWorldScale(glm::vec3 scale)
{
	m_LocalScale = scale / (GetWorldScale() / m_LocalScale);
}

void Transform::Translate(glm::vec3 translation)
{
	m_LocalPosition += translation;
}

void Transform::Rotate(float angle, glm::vec3 axis)
{
	m_LocalRotation = glm::rotate(m_LocalRotation, glm::radians(angle), axis);
}

void Transform::Rotate( glm::vec3 eulerAngles)
{
	m_LocalRotation *= glm::quat(glm::radians(eulerAngles));
}

void Transform::Scale(glm::vec3 scale)
{
	m_LocalScale *= scale;
}

const glm::mat4 Transform::GetLocalTransform() const 
{ 
	glm::mat4 local = glm::translate(glm::mat4(1.f), m_LocalPosition);
	local *= glm::mat4_cast(m_LocalRotation);
	local = glm::scale(local, m_LocalScale);
	return local;
}

const glm::vec3 Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::quat Transform::GetLocalRotationQuat() const
{
	return m_LocalRotation;
}

const glm::vec3 Transform::GetLocalRotation() const
{
	return glm::eulerAngles(m_LocalRotation);
}

const glm::vec3 Transform::GetLocalScale() const
{
	return m_LocalScale;
}

const glm::mat4 Transform::GetWorldTransform() const 
{ 
	glm::mat4 world = glm::translate(glm::mat4(1.f), GetWorldPosition());
	world *= glm::mat4_cast(GetWorldRotationQuat());
	world = glm::scale(world, GetWorldScale());
	return world;
}


const glm::vec3 Transform::GetWorldPosition() const
{
	glm::vec3 worldPos = m_LocalPosition;
	Transform* parent = m_Parent;
	while(parent)
	{
		worldPos += parent->m_LocalPosition;
		parent = parent->m_Parent;
	}
	return worldPos;
}

const glm::quat Transform::GetWorldRotationQuat() const
{
	glm::quat worldRot = m_LocalRotation;
	Transform* parent = m_Parent;
	while(parent)
	{
		worldRot *= parent->m_LocalRotation;
		parent = parent->m_Parent;
	}
	return worldRot;
}

const glm::vec3 Transform::GetWorldRotation() const
{
	return glm::eulerAngles(GetWorldRotationQuat());
}

const glm::vec3 Transform::GetWorldScale() const
{
	glm::vec3 worldScale = m_LocalScale;
	Transform* parent = m_Parent;
	while(parent)
	{
		worldScale *= parent->m_LocalScale;
		parent = parent->m_Parent;
	}
	return worldScale;
}

const glm::vec3 Transform::GetForwardDirection() const
{
	return glm::normalize(glm::inverse(glm::mat4_cast(GetWorldRotationQuat()))[0]);
}

const glm::vec3 Transform::GetUpDirection() const
{
	return glm::normalize(glm::inverse(glm::mat4_cast(GetWorldRotationQuat()))[1]);
}

const glm::vec3 Transform::GetRightDirection() const
{
	return glm::normalize(glm::inverse(glm::mat4_cast(GetWorldRotationQuat()))[2]);
}