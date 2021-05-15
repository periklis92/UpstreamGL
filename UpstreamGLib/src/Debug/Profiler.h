#pragma once

#include <stack>
#include <chrono>
#include <map>
#include <string>
#include <vector>

#include "System/Event.h"
#include "System/EventDispatcher.h"
#include "System/Delegate.h"
#include "Config.h"

struct ProfilerMeasurement;
struct InputKeyboardEvent;

struct ProfilerEvent
	:public Event
{
	GLR_IMPL_EVT_BODY(ProfilerEvent)
	ProfilerEvent(ProfilerMeasurement& measurement, const std::string name )
		:Name(name), Measurement(measurement) {}
	std::string Name;
	ProfilerMeasurement& Measurement;
};

struct ProfilerMeasurement
{
	std::chrono::duration<float> TotalTime{ 0 };
	std::chrono::duration<float, std::milli> PlotAvg{ 0 };
	std::chrono::high_resolution_clock::time_point LastStarted{ };
	std::uint32_t TimesMeasured{ 0 }, TimesMeasuredPlot{ 0 };

	inline std::chrono::duration<float> AverageTimeSec() const
	{
		return TotalTime / (float)TimesMeasured;
	}
};

struct PlotData
{
private:
	std::vector<float> m_Data;
	uint32_t m_Index = 0;

public:
	PlotData()
	{
		m_Data.resize(GLR_PROFILER_PLOT_POINTS);
	}

	uint32_t GetIndex() const
	{
		return m_Index;
	}

	void AddData(float data)
	{
		if (m_Index == GLR_PROFILER_PLOT_POINTS - 1)
		{
			std::rotate(m_Data.begin(), m_Data.begin() + 1, m_Data.end());
		}
		else
			++m_Index;

		m_Data[m_Index] = data;
	}

	const float* GetData() const
	{
		return m_Data.data();
	}
};

class Profiler
{
public:
	~Profiler();

	void StartClock(const std::string& measure);
	void EndClock();
	void ResultsToText(const std::string& filename) const;

	void Draw();

	static Profiler* GetInstance();

	EventDispatcher<ProfilerEvent>& OnMeasurementMade() { return m_ProfilerDispatcher; }
	
private:
	Profiler();
	void __PlotUpdater(void*);
	bool __ProfilerToggle(const InputKeyboardEvent*);
	
private:
	std::map<std::string, ProfilerMeasurement> m_Results;
	std::stack<std::string> m_ClockQueue;
	std::map<std::string, PlotData> m_PlotData;
	EventDispatcher<ProfilerEvent> m_ProfilerDispatcher;
	bool m_IsWindowOpen{true};

};