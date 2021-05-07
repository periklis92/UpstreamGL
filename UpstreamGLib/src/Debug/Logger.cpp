#include "Logger.h"

#include <cstdarg>
#include <cstdio>

#include "System/File.h"

void Logger::Log(const std::string& text, LogEntry::LogType severity)
{
	LogEntry newEntry{ text, time(0), severity };
	m_Logs.push_back(newEntry);
	m_LogDispatcher.Publish(LogEvent{ newEntry });
}

void Logger::Log(LogEntry::LogType severity, const char* format,  ...)
{
	va_list args;
	va_start(args, format);
	std::string buf;
	buf.resize(1024);
	std::vsprintf(&buf[0], format, args);
	buf.shrink_to_fit();
	LogEntry newEntry{ buf, time(0), severity };
	m_Logs.push_back(newEntry);
	m_LogDispatcher.Publish(LogEvent{ newEntry });

	va_end(args);
}

void Logger::ExportToText(const std::string& filename)
{
	File file(filename, File::Output);
	for (uint32_t i = 0; i < m_Logs.size(); ++i)
	{
		auto str = m_Logs[i].ToStream();
		file << str << "\n";
	}
}

void Logger::Clear()
{
	m_Logs.clear();
}

Logger* Logger::GetInstance()
{
	static Logger __Logger;
	return &__Logger;
}