#pragma once

#include <string>
#include <vector>
#include "Logger.h"

struct InputKeyboardEvent;

class Console
{
public:
	Console();
	~Console();

	void Draw();
	void Clear();

private:
	bool AddEntryCallback(const LogEvent* const e);
	bool Toggle(const InputKeyboardEvent* e);
	
private:
	std::vector<std::tuple<LogEntry::LogType, std::string>> m_Logs;
	bool m_IsOpen{false};
	int m_LogIndex{0};
	char m_InputBuffer[256];
	bool m_ShowWarn{true}, m_ShowError{true}, m_ShowTime{false};
};