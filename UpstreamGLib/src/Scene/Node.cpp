#include "Node.h"
#include "Math/Math.h"
#include <glm/gtx/euler_angles.hpp>
#include <algorithm>
Node::Node(const std::string& name)
	:m_Parent(nullptr), m_Children(), m_Name(name), 
		m_LocalTransform(glm::identity<glm::mat4>()), 
		m_LocalPosition(0), m_LocalRotation(glm::identity<glm::quat>()), m_LocalScale(1)
{
}

Node::~Node()
{
}

void Node::OnEnter()
{
	for (auto c: m_ComponentRegistry)
	{
		c.second->OnEnter();
	}
}

void Node::OnExit()
{
}

Node* Node::FindChildWithName(const std::string& name) 
{ 
	auto it = std::find_if(m_Children.begin(), m_Children.end(), 
		[name](Node* node) -> bool{return node->GetName() == name; }); 
	
	if (it != m_Children.end())
		return *it;

	return nullptr;
}

void Node::SetLocalTransform(const glm::mat4& transform) 
{ 
	m_LocalTransform = transform;
}

void Node::SetWorldTransform(const glm::mat4& transform) 
{ 
	SetWorldPosition(Math::GetTranslation(transform));
	SetWorldRotation(Math::GetRotationQuat(transform));
	SetWorldScale(Math::GetScale(transform));
}

void Node::SetLocalPosition(glm::vec3 position)
{
	m_LocalPosition = position;
}

void Node::SetLocalRotation(glm::quat rotation)
{
	m_LocalRotation = rotation;
}

void Node::SetLocalRotation(glm::vec3 rotation)
{
	m_LocalRotation = glm::identity<glm::quat>();
	m_LocalRotation = glm::rotate(m_LocalRotation, rotation.x, glm::vec3(1, 0, 0));
	m_LocalRotation = glm::rotate(m_LocalRotation, rotation.y, glm::vec3(0, 1, 0));
	m_LocalRotation = glm::rotate(m_LocalRotation, rotation.z, glm::vec3(0, 0, 1));
}

void Node::SetLocalScale(glm::vec3 scale)
{
	m_LocalScale = scale;
}

void Node::SetWorldPosition(glm::vec3 position)
{
	m_LocalPosition = position - (GetWorldPosition() - m_LocalPosition);
}

void Node::SetWorldRotation(glm::quat rotation)
{
	m_LocalRotation = rotation * (rotation * glm::inverse(GetWorldRotationQuat() * glm::inverse(m_LocalRotation)));
}

void Node::SetWorldRotation(glm::vec3 rotation)
{
	auto rotQ = glm::identity<glm::quat>();
	rotQ = glm::rotate(rotQ, rotation.x, glm::vec3(1, 0, 0));
	rotQ = glm::rotate(rotQ, rotation.y, glm::vec3(0, 1, 0));
	rotQ = glm::rotate(rotQ, rotation.z, glm::vec3(0, 0, 1));
	SetWorldRotation(rotQ);
}

void Node::SetWorldScale(glm::vec3 scale)
{
	m_LocalScale = scale / (GetWorldScale() / m_LocalScale);
}

void Node::Translate(glm::vec3 translation)
{
	m_LocalPosition += translation;
}

void Node::Rotate(float angle, glm::vec3 axis)
{
	m_LocalRotation = glm::rotate(m_LocalRotation, glm::radians(angle), axis);
}

void Node::Rotate( glm::vec3 eulerAngles)
{
	m_LocalRotation *= glm::quat(glm::radians(eulerAngles));
}

void Node::Scale(glm::vec3 scale)
{
	m_LocalScale *= scale;
}

const glm::mat4 Node::GetLocalTransform() const 
{ 
	glm::mat4 local = glm::translate(glm::mat4(1.f), m_LocalPosition);
	local *= glm::mat4_cast(m_LocalRotation);
	local = glm::scale(local, m_LocalScale);
	return local;
}

const glm::vec3 Node::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::quat Node::GetLocalRotationQuat() const
{
	return m_LocalRotation;
}

const glm::vec3 Node::GetLocalRotation() const
{
	return glm::eulerAngles(m_LocalRotation);
}

const glm::vec3 Node::GetLocalScale() const
{
	return m_LocalScale;
}

const glm::mat4 Node::GetWorldTransform() const 
{ 
	glm::mat4 world = glm::translate(glm::mat4(1.f), GetWorldPosition());
	world *= glm::mat4_cast(GetWorldRotationQuat());
	world = glm::scale(world, GetWorldScale());
	return world;
}


const glm::vec3 Node::GetWorldPosition() const
{
	glm::vec3 worldPos = m_LocalPosition;
	Node* parent = m_Parent;
	while(parent)
	{
		worldPos += parent->m_LocalPosition;
		parent = parent->m_Parent;
	}
	return worldPos;
}

const glm::quat Node::GetWorldRotationQuat() const
{
	glm::quat worldRot = m_LocalRotation;
	Node* parent = m_Parent;
	while(parent)
	{
		worldRot *= parent->m_LocalRotation;
		parent = parent->m_Parent;
	}
	return worldRot;
}

const glm::vec3 Node::GetWorldRotation() const
{
	return glm::eulerAngles(GetWorldRotationQuat());
}

const glm::vec3 Node::GetWorldScale() const
{
	glm::vec3 worldScale = m_LocalScale;
	Node* parent = m_Parent;
	while(parent)
	{
		worldScale *= parent->m_LocalScale;
		parent = parent->m_Parent;
	}
	return worldScale;
}

const glm::vec3 Node::GetForwardDirection() const
{
	return glm::normalize(glm::inverse(glm::mat4_cast(GetWorldRotationQuat()))[0]);
}

const glm::vec3 Node::GetUpDirection() const
{
	return glm::normalize(glm::inverse(glm::mat4_cast(GetWorldRotationQuat()))[1]);
}

const glm::vec3 Node::GetRightDirection() const
{
	return glm::normalize(glm::inverse(glm::mat4_cast(GetWorldRotationQuat()))[2]);
}