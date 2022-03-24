#include <algorithm>

#include <UpstreamGL/Debug/Debug.h>
#include <UpstreamGL/System/Scheduler.h>

namespace upgl {
void Scheduler::Update(float deltaTime) {
  for (auto &d : m_RegisteredUpdates) {
    d(deltaTime);
  }

  auto it = m_DelayedCallbacks.begin();

  for (; it != m_DelayedCallbacks.end();) {
    if (it->IsTimeToCall()) {
      it->Callback(it->UserData);
      it->IntervalTimer.Reset();
      if (it->CallOnce) {
        it = m_DelayedCallbacks.erase(it);
        continue;
      }
    }
    ++it;
  }
}

void Scheduler::FixedUpdate(float fixedDeltaTime) {
  for (auto &d : m_FixedUpdates) {
    d(fixedDeltaTime);
  }
}

void Scheduler::RegisterUpdate(const UpdateDelegate &updateDelegate) {
  m_RegisteredUpdates.push_back(updateDelegate);
}

void Scheduler::RegisterFixedUpdate(const UpdateDelegate &updateDelegate) {
  m_FixedUpdates.push_back(updateDelegate);
}

void Scheduler::RegisterDelayed(const DelayedDelegate &delayedDelegate, void *userData,
                                float intervalMS, bool callOnce) {
  DelayedCallback callback{delayedDelegate, userData, intervalMS, callOnce, Timer{}};

  m_DelayedCallbacks.push_back(callback);
}

void Scheduler::UnregisterUpdate(const UpdateDelegate &updateDelegate) {
  auto it = std::find(m_RegisteredUpdates.begin(), m_RegisteredUpdates.end(), updateDelegate);

  if (it != m_RegisteredUpdates.end())
    m_RegisteredUpdates.erase(it);
}

void Scheduler::UnregisterFixedUpdate(const UpdateDelegate &updateDelegate) {
  auto it = std::find(m_FixedUpdates.begin(), m_FixedUpdates.end(), updateDelegate);

  if (it != m_FixedUpdates.end())
    m_FixedUpdates.erase(it);
}

void Scheduler::UnregisterDelayed(const DelayedDelegate &updateDelegate) {
  auto it = std::find(m_DelayedCallbacks.begin(), m_DelayedCallbacks.end(), updateDelegate);
  m_DelayedCallbacks.erase(it);
}
} // namespace upgl