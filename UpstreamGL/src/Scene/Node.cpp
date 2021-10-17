#include <UpstreamGL/Scene/Node.h>

Node::Node(const std::string& name)
	:m_Name(name)
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
	for (auto c: m_ComponentRegistry)
	{
		c.second->OnExit();
	}
}