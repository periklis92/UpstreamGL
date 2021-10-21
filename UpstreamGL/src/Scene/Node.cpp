#include <UpstreamGL/Scene/Node.h>

Node::Node(const std::string& name)
	:m_Name(name)
{
}

Node::Node(Node&& node)
{
	m_Name = std::move(node.m_Name);
	m_ComponentRegistry = std::move(node.m_ComponentRegistry);
}

Node&& Node::operator=(Node&& node)
{
	m_Name = std::move(node.m_Name);
	m_ComponentRegistry = std::move(node.m_ComponentRegistry);
	return std::move(*this);
}

bool Node::operator==(const Node& node) const
{
	return m_Name == node.m_Name;
}

Node::~Node()
{
}

void Node::OnEnter()
{
	for (auto& c: m_ComponentRegistry)
	{
		c.second->OnEnter();
	}
}

void Node::OnExit()
{
	for (auto& c: m_ComponentRegistry)
	{
		c.second->OnExit();
	}
}