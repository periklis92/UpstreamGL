#include "Profiler.h"

#include <sstream>
#include <algorithm>

#include "imgui.h"
#include "System/Application.h"
#include "System/Scheduler.h"
#include "System/File.h"
#include "Input/InputManager.h"

Profiler::Profiler()
{
	Application::GetInstance()->GetScheduler()->RegisterDelayed(
		DelayedDelegate{ ConnectFunc<&Profiler::__PlotUpdater>, this }, nullptr, GLR_PROFILER_PLOT_INTERVAL_SECS, false);
	
	InputManager::GetInstance()->OnKeyboardKey() += InputKeyboardDelegate{ ConnectFunc<&Profiler::__ProfilerToggle>, this };
}

Profiler::~Profiler()
{
	// Application::GetInstance()->GetScheduler()->Unregister(
		// DelayedDelegate{ ConnectFunc<&Profiler::__PlotUpdater>, this });

	// InputManager::GetInstance()->OnKeyboardKey() -= InputKeyboardDelegate{ ConnectFunc<&Profiler::__ProfilerToggle>, this };
}

void Profiler::StartClock(const std::string& measure)
{
	m_Results[measure].LastStarted = std::chrono::high_resolution_clock::now();	
	m_ClockQueue.push(measure);
}

void Profiler::EndClock()
{
	if (!m_ClockQueue.empty())
	{
		auto& resultToUpdate = m_Results[m_ClockQueue.top()];
		auto count = (std::chrono::high_resolution_clock::now() - resultToUpdate.LastStarted);
		resultToUpdate.TotalTime += count;
		resultToUpdate.PlotAvg += count;
		resultToUpdate.TimesMeasured++;
		resultToUpdate.TimesMeasuredPlot++;
		m_ProfilerDispatcher.Publish(ProfilerEvent{ resultToUpdate, m_ClockQueue.top() });
		m_ClockQueue.pop();
	}
}

Profiler* Profiler::GetInstance()
{
	static Profiler __Profiler;
	return &__Profiler;
}

void Profiler::ResultsToText(const std::string& filename) const
{
	File file(filename, File::Output);
	std::stringstream str;
	str << "-------Profiler Diagnostics-------\n";
	for (auto& r : m_Results)
	{
		str << "--------- " << r.first << " ---------\n"
			<< "Total Time: " << std::to_string(r.second.TotalTime.count()) << "s\n"
			<< "Average Time:" << std::to_string(r.second.AverageTimeSec().count()) << "s\n"
			<< "Sample Size:" << std::to_string(r.second.TimesMeasured) << "\n";
	}

	file << str;
}

void Profiler::Draw()
{
	if (!m_IsWindowOpen) return;
	if (ImGui::Begin("Profiler", &m_IsWindowOpen))
	{
		for (auto& p: m_PlotData)
		{
			ImGui::Text("Time: %.04fms", p.second.GetData()[p.second.GetIndex()]);
			ImGui::PlotLines(p.first.c_str(), p.second.GetData(), GLR_PROFILER_PLOT_POINTS);
			ImGui::Separator();
		}
	}
	ImGui::End();
}

void Profiler::__PlotUpdater(void*)
{
	for (auto& p: m_Results)
	{
		m_PlotData[p.first].AddData(p.second.PlotAvg.count() / (float)p.second.TimesMeasuredPlot);
		p.second.PlotAvg = std::chrono::duration<float, std::milli>(0.f);
		p.second.TimesMeasuredPlot = 0;
	}
}

bool Profiler::__ProfilerToggle(const InputKeyboardEvent* e)
{
	if (e->Key == KeyboardKey::F12 && e->IsPressed)
	{
		m_IsWindowOpen = !m_IsWindowOpen;
	}
	return true;
}