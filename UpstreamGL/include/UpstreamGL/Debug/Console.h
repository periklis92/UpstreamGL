#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "Logger.h"

namespace upgl {
struct InputKeyboardEvent;

struct ConsoleEntry {
  LogEntry::LogType LogType;
  std::string Str;
};

class Console {
public:
  Console();
  ~Console();

  void Draw();
  void Clear();
  void AddEntry(const LogEntry &e);

private:
  bool __Toggle(const InputKeyboardEvent *e);

private:
  std::vector<ConsoleEntry> m_Logs;
  bool m_IsOpen{false};
  int m_LogIndex{-1};
  char m_InputBuffer[256];
  bool m_ShowWarn{true}, m_ShowError{true}, m_ShowTime{false}, m_Scroll{false};
};
} // namespace upgl