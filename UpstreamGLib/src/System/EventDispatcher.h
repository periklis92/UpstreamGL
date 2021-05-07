#pragma once

#include <list>
#include <type_traits>

#include "Delegate.h"
#include "Event.h"


template<class EventType>
using EventDelegate = Delegate<bool(const EventType*)>;

template<class EventType>
class EventDispatcher
{
	using delegate_type = EventDelegate<EventType>;
	template<class CallerType>
	using func_type = typename delegate_type::template member_type<CallerType>;
	//using delegate_callback = typename delegate_type::member_callback;
public:
	EventDispatcher() = default;
	EventDispatcher(const EventDispatcher&) = delete;
	EventDispatcher& operator=(const EventDispatcher&) = delete;
	~EventDispatcher() = default;

	void Subscribe(delegate_type callback)
	{
		m_Delegates.push_back(callback);
	}

	EventDispatcher& operator+=(const delegate_type& delegate)
	{
		m_Delegates.push_back(delegate);
		return *this;
	}

	EventDispatcher& operator+=(delegate_type&& delegate)
	{
		m_Delegates.push_back(std::move(delegate));
		return *this;
	}

	EventDispatcher& operator-=(const delegate_type& delegate)
	{
		m_Delegates.remove(delegate);
		return *this;
	}

	void Publish(const EventType* e)
	{
		for (auto it = m_Delegates.begin(); it != m_Delegates.end();)
		{
			bool res = (*it).Invoke(e);
			if (!res)
			{
				it = m_Delegates.erase(it);
			}
			else
				++it;
		}
	}

	void Publish(const EventType& e)
	{
		Publish(&e);
	}

private:
	std::list<delegate_type> m_Delegates;
};