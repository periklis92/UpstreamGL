#pragma once

#include <ctime>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <tuple>
#include <vector>

#include <UpstreamGL/System/Delegate.h>
#include <UpstreamGL/System/Event.h>
#include <UpstreamGL/System/EventDispatcher.h>

namespace upgl {
struct LogEntry {
  enum LogType { None, Info, Warning, Error, Critical };

  std::string Text;
  std::time_t Time;
  LogType Severity;

  inline std::string TimeStr() const {
    std::stringstream ss;
    struct tm *TM = localtime(&Time);
    ss << std::put_time(TM, "%H:%M:%S");

    return ss.str();
  }

  inline std::string SeverityToStr() const {
    switch (Severity) {
    case Info:
      return "Info";
    case Warning:
      return "Warning";
    case Error:
      return "Error";
    case Critical:
      return "Critical";
    default:
      return "";
    }
  }

  inline std::stringstream ToStream(bool wTime = true) const {
    std::stringstream stream;
    if (wTime)
      stream << "(" << TimeStr() << ") ";
    if (Severity != None)
      stream << "[" << SeverityToStr() << "] ";
    stream << Text;
    return stream;
  }

  inline std::string Str(bool wTime = true) const { return ToStream(wTime).str(); }
};

struct LogEvent : Event {
  UPGL_IMPL_EVT_BODY(LogEvent)
  LogEvent(const LogEntry &entry) : Entry(entry) {}
  const LogEntry &Entry;
};

class Logger {
public:
  static void Log(LogEntry::LogType severity, const std::string &text);
  static void Log(LogEntry::LogType severity, const char *text, std::size_t size);
  static void Log(LogEntry::LogType severity, const char *format, ...);
  // void ExportToText(const std::string& filename);
  void Clear();
  static void SetStream(std::ostream *stream) { m_LogStream = stream; }
  static std::ostream &GetStream() { return *m_LogStream; }
  // static Logger* GetInstance();

  // EventDispatcher<LogEvent>& OnLogAdded() { return m_LogDispatcher; }

private:
  static std::ostream *m_LogStream;
  // std::vector<LogEntry> m_Logs;
  // EventDispatcher<LogEvent> m_LogDispatcher;
};
} // namespace upgl