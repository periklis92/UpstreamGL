#pragma once

#include <chrono>
#include <cstdint>
#include <map>
#include <stack>
#include <string>
#include <vector>

#include <UpstreamGL/Config.h>
#include <UpstreamGL/System/Delegate.h>
#include <UpstreamGL/System/Event.h>
#include <UpstreamGL/System/EventDispatcher.h>
#include <UpstreamGL/System/Timer.h>

namespace upgl {
struct ProfilerMeasurement;
struct InputKeyboardEvent;

struct ProfilerEvent : public Event {
  UPGL_IMPL_EVT_BODY(ProfilerEvent)
  ProfilerEvent(ProfilerMeasurement &measurement, const std::string name)
      : Name(name), Measurement(measurement) {}
  std::string Name;
  ProfilerMeasurement &Measurement;
};

struct ProfilerMeasurement {
  float TotalTime{0};
  float PlotAvg{0};
  Timer ProfTimer{};
  uint32_t TimesMeasured{0}, TimesMeasuredPlot{0};

  inline float AverageTimeSec() const { return TotalTime / (float)TimesMeasured; }
};

struct PlotData {
private:
  std::vector<float> m_Data;
  uint32_t m_Index = 0;

public:
  PlotData() { m_Data.resize(UPGL_PROFILER_PLOT_POINTS); }

  inline uint32_t GetIndex() const { return m_Index; }

  inline void AddData(float data) {
    if (m_Index == UPGL_PROFILER_PLOT_POINTS - 1) {
      std::rotate(m_Data.begin(), m_Data.begin() + 1, m_Data.end());
    } else {
      ++m_Index;
    }

    m_Data[m_Index] = data;
  }

  inline const float *GetData() const { return m_Data.data(); }
};

class Profiler {
public:
  ~Profiler();

  void StartClock(const std::string &measure);
  void EndClock();
  void ResultsToText(const std::string &filename) const;

  void Draw();

  static Profiler *GetInstance();

  inline EventDispatcher<ProfilerEvent> &OnMeasurementMade() { return m_ProfilerDispatcher; }

private:
  Profiler();
  void __PlotUpdater(void *);
  bool __ProfilerToggle(const InputKeyboardEvent *);

private:
  std::map<std::string, ProfilerMeasurement> m_Results;
  std::stack<std::string> m_ClockQueue;
  std::map<std::string, PlotData> m_PlotData;
  EventDispatcher<ProfilerEvent> m_ProfilerDispatcher;
  bool m_IsWindowOpen{true};
};
} // namespace upgl