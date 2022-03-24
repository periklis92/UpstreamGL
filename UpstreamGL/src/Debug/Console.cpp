#include <cstring>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <UpstreamGL/Debug/Console.h>
#include <UpstreamGL/Debug/Debug.h>
#include <UpstreamGL/Debug/Logger.h>
#include <UpstreamGL/Input/InputManager.h>

#define IM_DEF_COLOR                                                                               \
  ImColor { 200, 210, 220, 255 }
#define IM_INFO_COLOR                                                                              \
  ImColor { 100, 150, 255, 255 }
#define IM_WARN_COLOR                                                                              \
  ImColor { 220, 220, 40, 255 }
#define IM_ERR_COLOR                                                                               \
  ImColor { 255, 80, 80, 255 }
#define IM_CRIT_COLOR                                                                              \
  ImColor { 200, 20, 20, 255 }

namespace upgl {
Console::Console() {
  InputManager::GetInstance()->OnKeyboardKey() +=
      InputKeyboardDelegate{ConnectFunc<&Console::__Toggle>, this};
  std::memset(m_InputBuffer, '\0', 256);
  m_Logs.reserve(UPGL_CONSOLE_LOG_SIZE);
}

Console::~Console() {
  InputManager::GetInstance()->OnKeyboardKey() -=
      InputKeyboardDelegate{ConnectFunc<&Console::__Toggle>, this};
}

void Console::Draw() {
  if (!m_IsOpen)
    return;
  if (ImGui::Begin("Console", &m_IsOpen)) {
    ImGui::SameLine();
    ImGui::Checkbox("Warnings", &m_ShowWarn);
    ImGui::SameLine();
    ImGui::Checkbox("Errors", &m_ShowError);

    const float footer_height_to_reserve =
        ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("Child1", ImVec2(0, -footer_height_to_reserve), true,
                          ImGuiWindowFlags_HorizontalScrollbar)) {
      for (int i = 0; i < m_Logs.size(); ++i) {
        LogEntry::LogType lType = m_Logs[i].LogType;
        if ((lType != LogEntry::Warning || (lType == LogEntry::Warning && m_ShowWarn)) &&
            (lType != LogEntry::Error || (lType == LogEntry::Error && m_ShowError))) {
          ImColor txtCol;
          switch (lType) {
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
          ImGui::TextColored(txtCol, "%s", m_Logs[i].Str.c_str());
          ImGui::PopTextWrapPos();
          ImGui::Separator();
        }
      }
    }

    if (m_Scroll) {
      ImGui::SetScrollHereY(1.0f);
      m_Scroll = false;
    }

    ImGui::EndChild();
    ImGui::Separator();
    if (ImGui::InputText("Command", m_InputBuffer, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {
      if (m_InputBuffer[0] != '\0') {
        Logger::Log(LogEntry::None, m_InputBuffer);
        std::memset(m_InputBuffer, '\0', 256);
      }
      ImGui::SetKeyboardFocusHere();
    }
  }
  ImGui::End();
}

void Console::Clear() {
  m_LogIndex = 0;
  for (int i = 0; i < UPGL_CONSOLE_LOG_SIZE; ++i) {
    m_Logs[i].Str.clear();
  }
}

void Console::AddEntry(const LogEntry &e) {
  if (m_LogIndex == UPGL_CONSOLE_LOG_SIZE - 1) {
    std::rotate(m_Logs.begin(), m_Logs.begin() + 1, m_Logs.end());
    m_Logs[m_LogIndex] = ConsoleEntry{e.Severity, e.Str()};
  } else {
    ++m_LogIndex;
    m_Logs.push_back(ConsoleEntry{e.Severity, e.Str()});
  }
  m_Scroll = true;
}

bool Console::__Toggle(const InputKeyboardEvent *e) {
  if (e->Key == KeyboardKey::GRAVE_ACCENT && !e->IsPressed) {
    m_IsOpen = !m_IsOpen;
  }
  return true;
}
} // namespace upgl