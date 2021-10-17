#pragma once

#include <sstream>
#include <string>
#include <tuple>
#include <ctime>
#include <vector>
#include <iomanip>
#include <iostream>

#include <UpstreamGL/System/Delegate.h>
#include <UpstreamGL/System/Event.h>
#include <UpstreamGL/System/EventDispatcher.h>

struct LogEntry
{
	enum LogType
	{
		None, Info, Warning, Error, Critical
	};

	std::string Text;
	std::time_t Time;
	LogType Severity;

	inline std::string TimeStr() const
	{
		std::stringstream ss;
		ss << std::put_time(std::localtime(&Time), "%H:%M:%S");
		return ss.str();
	}

	inline std::string SeverityToStr() const
	{
		switch (Severity)
		{
		case Info:
			return " Info";
		case Warning:
			return " Warning";
		case Error:
			return " Error";
		case Critical:
			return " Critical";
		default:
			return "";
		}
	}

	inline std::stringstream ToStream(bool wTime = true) const
	{
		std::stringstream stream;
		if (wTime)
			stream << "(" << TimeStr() << ")";
		stream  << SeverityToStr() << ": " << Text;
		return stream;
	}

	inline std::string ToString(bool wTime = true) const
	{
		return ToStream(wTime).str();
	}
};

struct LogEvent
	: Event
{
	UPGL_IMPL_EVT_BODY(LogEvent)
	LogEvent(const LogEntry& entry)
	:Entry(entry) {}
	const LogEntry& Entry;
};

class Logger
{
public:
	Logger() { std::cout <<"Logger Started"; }
	void Log(const std::string& text, LogEntry::LogType severity = LogEntry::None);
	void Log( LogEntry::LogType severity,const char* format, ...);
	void ExportToText(const std::string& filename);
	void Clear();

	static Logger* GetInstance();

	EventDispatcher<LogEvent>& OnLogAdded() { return m_LogDispatcher; }

private:
	std::vector<LogEntry> m_Logs;
	EventDispatcher<LogEvent> m_LogDispatcher;
};