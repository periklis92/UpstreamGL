#include <UpstreamGL/System/Timer.h>

namespace upgl {
Timer::Timer() { Reset(); }

void Timer::Reset() { m_StartPoint = std::chrono::system_clock::now(); }

float Timer::GetMilliseconds() const {
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration<float, std::milli>(now - m_StartPoint).count();
}

float Timer::GetSeconds() const {
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration<float>(now - m_StartPoint).count();
}
} // namespace upgl