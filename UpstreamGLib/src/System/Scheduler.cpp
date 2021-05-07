#include "Scheduler.h"
#include "Debug/Debug.h"

void Scheduler::Update(float deltaTime)
{
	for (auto& d : m_RegisteredUpdates)
	{
		d(deltaTime);
	}

	auto it = m_DelayedCallbacks.begin();

	for (;it != m_DelayedCallbacks.end();)
	{
		if (it->IsTimeToCall())
		{
			it->Callback(it->UserData);
			it->LastCall = std::chrono::high_resolution_clock::now();
			if (it->CallOnce)
			{
				it = m_DelayedCallbacks.erase(it);
				continue;
			}
		}
		++it;
	}
}

void Scheduler::Register(const UpdateDelegate& updateDelegate)
{
	m_RegisteredUpdates.push_back(updateDelegate);
}

void Scheduler::Register(const DelayedDelegate& delayedDelegate, void* userData, float intervalSeconds, bool callOnce)
{
	DelayedCallback callback
	{
		delayedDelegate, userData, std::chrono::duration<float>(intervalSeconds), callOnce, std::chrono::high_resolution_clock::now()
	};

	m_DelayedCallbacks.push_back(callback);
}

void Scheduler::Unregister(const UpdateDelegate& updateDelegate)
{
	m_RegisteredUpdates.remove(updateDelegate);
}

void Scheduler::Unregister(const DelayedDelegate& updateDelegate)
{
	auto it = std::find(m_DelayedCallbacks.begin(), m_DelayedCallbacks.end(), updateDelegate);
	m_DelayedCallbacks.erase(it);
}