#include "UpstreamGL/System/Application.h"
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <iostream>

#include <UpstreamGL/Debug/Console.h>
#include <UpstreamGL/Debug/Logger.h>
#include <UpstreamGL/System/File.h>

namespace upgl {
std::ostream *Logger::m_LogStream = nullptr;

void Logger::Log(LogEntry::LogType severity, const std::string &text) {
  Log(severity, text.c_str(), text.size());
}

void Logger::Log(LogEntry::LogType severity, const char *text, std::size_t size) {
  auto con = Application::GetInstance()->GetConsole();
  auto entry = LogEntry{std::string(text), time(0), severity};
  if (con != nullptr) {
    con->AddEntry(entry);
  }
  if (m_LogStream) {
    auto str = entry.Str();
    m_LogStream->write(str.c_str(), str.size());
    m_LogStream->write("\n", 1);
  }
}

void Logger::Log(LogEntry::LogType severity, const char *format, ...) {
  va_list args;
  va_start(args, format);

  char buf[1024];
  int sz = vsprintf(&buf[0], format, args);

  va_end(args);
  Log(severity, &buf[0], sz);
}

// void Logger::ExportToText(const std::string& filename)
// {
// 	File file(filename, File::Output);
// 	for (uint32_t i = 0; i < m_Logs.size(); ++i)
// 	{
// 		auto str = m_Logs[i].ToStream();
// 		file << str << "\n";
// 	}
// }

// void Logger::Clear()
// {
// 	m_Logs.clear();
// }

// Logger* Logger::GetInstance()
// {
// 	static Logger __Logger;
// 	return &__Logger;
// }
} // namespace upgl