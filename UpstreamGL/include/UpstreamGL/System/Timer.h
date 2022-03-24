#pragma once

#include <chrono>

namespace upgl {
class Timer {
public:
  Timer();

  void Reset();
  float GetMilliseconds() const;
  float GetSeconds() const;

private:
  std::chrono::system_clock::time_point m_StartPoint;
};
} // namespace upgl