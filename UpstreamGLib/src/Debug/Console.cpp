#include "Console.h"
#include <cstring>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "System/Application.h"
#include "Input/InputManager.h"
#include "Config.h"

#define IM_DEF_COLOR	ImColor{ 200, 210, 220, 255 }
#define IM_INFO_COLOR	ImColor{ 100, 150, 255, 255 }
#define IM_WARN_COLOR	ImColor{ 220, 220, 40, 255 }
#define IM_ERR_COLOR	ImColor{ 255, 80, 80, 255 }
#define IM_CRIT_COLOR	ImColor{ 200, 20, 20, 255 }

Console::Console()
{
	Logger::GetInstance()->OnLogAdded() += EventDelegate<LogEvent>{ ConnectFunc<&Console::AddEntryCallback>, this };
	InputManager::GetInstance()->OnKeyboardKey() += InputKeyboardDelegate{ ConnectFunc<&Console::Toggle>, this };
	std::memset(m_InputBuffer, '\0', 256);
	m_Logs.resize(GLR_CONSOLE_LOG_SIZE);
}

Console::~Console()
{
	Logger::GetInstance()->OnLogAdded() -= EventDelegate<LogEvent>{ ConnectFunc<&Console::AddEntryCallback>, this };
	InputManager::GetInstance()->OnKeyboardKey() -= InputKeyboardDelegate{ ConnectFunc<&Console::Toggle>, this };
}

void Console::Draw()
{
	if (!m_IsOpen) return;
	if (ImGui::Begin("Console", &m_IsOpen))
	{
		ImGui::SameLine(); ImGui::Checkbox("Warnings", &m_ShowWarn);
		ImGui::SameLine(); ImGui::Checkbox("Errors", &m_ShowError);
		//ImGui::SameLine(); ImGui::Checkbox("Time Stamp", &m_ShowTime;
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("Child1", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar))
		{
			for (int i = 0; i < m_Logs.size(); ++i)
			{
				LogEntry::LogType lType = std::get<0>(m_Logs[i]);
				if ((lType != LogEntry::Warning || (lType == LogEntry::Warning && m_ShowWarn)) &&
					(lType != LogEntry::Error || (lType == LogEntry::Error && m_ShowError)))
				{
					ImColor txtCol;
					switch (lType)
					{
					case LogEntry::Info:
						txtCol = IM_INFO_COLOR;
						break;
					case LogEntry::Warning:
						txtCol = IM_WARN_COLOR;
						break;
					case LogEntry::Error:
						txtCol = IM_ERR_COLOR;
						break;
					case LogEntry::Critical:
						txtCol = IM_CRIT_COLOR;
						break;
					default:
						txtCol = IM_DEF_COLOR;
						break;
					}
					
					ImGui::PushTextWrapPos(ImGui::GetWindowSize().x);
					ImGui::TextColored(txtCol, "%s", std::get<1>(m_Logs[i]).data());
					ImGui::PopTextWrapPos();
					ImGui::Separator();
				}
			}
		}
		ImGui::EndChild();
		ImGui::Separator();
		if (ImGui::InputText("Cmd", m_InputBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			Logger::GetInstance()->Log(m_InputBuffer);
			std::memset(m_InputBuffer, '\0', 256);
		}
	}
	ImGui::End();
}

void Console::Clear()
{
	m_LogIndex = 0;
	for(int i = 0; i < GLR_CONSOLE_LOG_SIZE; ++i)
	{
		std::get<1>(m_Logs[i]).clear();
	}
}

bool Console::AddEntryCallback(const LogEvent* const e)
{
	m_Logs[m_LogIndex] = std::tuple{e->Entry.Severity, e->Entry.ToString() };
	if (m_LogIndex == GLR_CONSOLE_LOG_SIZE - 1)
		std::rotate(m_Logs.begin(), m_Logs.begin() + 1, m_Logs.end());
	else
		++m_LogIndex;

	if (e->Entry.Text == "export")
		Logger::GetInstance()->ExportToText("Logs.txt");
	else if (e->Entry.Text == "clear")
		Clear();
	
	return true;
}

bool Console::Toggle(const InputKeyboardEvent* e)
{
	if (e->Key == KeyboardKey::GRAVE_ACCENT && !e->IsPressed)
	{
		m_IsOpen = !m_IsOpen;
	}
	return true;
}
