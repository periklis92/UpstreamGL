#pragma once

#include <chrono>
#include <list>
#include "Delegate.h"

using UpdateDelegate = Delegate<void(float)>;
using DelayedDelegate = Delegate<void(void*)>;

struct DelayedCallback
{
	DelayedDelegate Callback{};
	void* UserData = nullptr;
	std::chrono::duration<double> Interval;
	bool CallOnce = true;
	std::chrono::system_clock::time_point LastCall;

	inline bool IsTimeToCall() const
	{
		return (std::chrono::system_clock::now() >= LastCall + Interval);
	}

	inline bool operator==(const DelayedDelegate& other) const
	{
		return Callback == other;
	}
};

class Scheduler
{
public:
	Scheduler() = default;
	Scheduler(const Scheduler&) = delete;
	Scheduler& operator=(const Scheduler&) = delete;
	~Scheduler() = default;

	void Update(float deltaTime);
	void Register(const UpdateDelegate& updateDelegate);
	void Register(const DelayedDelegate& delayedDelegate, void* userData, float intervalSeconds, bool callOnce = true);
	void Unregister(const UpdateDelegate& updateDelegate);
	void Unregister(const DelayedDelegate& updateDelegate);

private:
	std::list<UpdateDelegate> m_RegisteredUpdates;
	std::list<DelayedCallback> m_DelayedCallbacks;
};