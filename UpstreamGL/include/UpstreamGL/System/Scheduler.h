#pragma once

#include <chrono>
#include <vector>

#include <UpstreamGL/System/Timer.h>

#include "Delegate.h"

namespace upgl {
using UpdateDelegate = Delegate<void(float)>;
using DelayedDelegate = Delegate<void(void *)>;

struct DelayedCallback {
  DelayedDelegate Callback{};
  void *UserData = nullptr;
  double IntervalMS;
  bool CallOnce = true;
  Timer IntervalTimer;

  inline bool IsTimeToCall() const { return IntervalTimer.GetMilliseconds() >= IntervalMS; }

  inline bool operator==(const DelayedDelegate &other) const { return Callback == other; }
};

class Scheduler {
public:
  Scheduler() = default;
  Scheduler(const Scheduler &) = delete;
  Scheduler &operator=(const Scheduler &) = delete;
  ~Scheduler() = default;

  void Update(float deltaTime);
  void FixedUpdate(float fixedDeltaTime);
  void RegisterUpdate(const UpdateDelegate &updateDelegate);
  void RegisterFixedUpdate(const UpdateDelegate &updateDelegate);
  void RegisterDelayed(const DelayedDelegate &delayedDelegate, void *userData, float intervalMS,
                       bool callOnce = true);
  void UnregisterUpdate(const UpdateDelegate &updateDelegate);
  void UnregisterFixedUpdate(const UpdateDelegate &updateDelegate);
  void UnregisterDelayed(const DelayedDelegate &updateDelegate);

private:
  ::std::vector<UpdateDelegate> m_RegisteredUpdates;
  ::std::vector<UpdateDelegate> m_FixedUpdates;
  ::std::vector<DelayedCallback> m_DelayedCallbacks;
};
} // namespace upgl