#pragma once

#include "../Config.h"
#include "Console.h"
#include "Logger.h"
#include "Profiler.h"

#ifdef UPGL_PROFILE
#define UPGL_PROFILER_START(section) upgl::Profiler::GetInstance()->StartClock(section)
#define UPGL_PROFILER_END() upgl::Profiler::GetInstance()->EndClock()
#define UPGL_PROFILER_EXPORT_FILE(filename) upgl::Profiler::GetInstance()->ResultsToText(filename)
#define UPGL_PROFILER_DRAW() upgl::Profiler::GetInstance()->Draw()
#else
#define UPGL_PROFILER_START(section)
#define UPGL_PROFILER_END()
#define UPGL_PROFILER_EXPORT_FILE(filename)
#define UPGL_PROFILER_DRAW()
#endif

#define UPGL_LOG(message, ...) upgl::Logger::Log(LogEntry::None, message, ##__VA_ARGS__)
#define UPGL_LOG_INFO(message, ...) upgl::Logger::Log(LogEntry::Info, message, ##__VA_ARGS__)
#define UPGL_LOG_WARNING(message, ...) upgl::Logger::Log(LogEntry::Warning, message, ##__VA_ARGS__)
#define UPGL_LOG_ERROR(message, ...) upgl::Logger::Log(LogEntry::Error, message, ##__VA_ARGS__)
#define UPGL_LOG_CRITICAL(message, ...) upgl::Logger::Log(LogEntry::Critical, ##__VA_ARGS__)
#define UPGL_LOG_SVR(severity, message, ...) upgl::Logger::Log(severity, message, ##__VA_ARGS__)

#ifdef UPGL_DEBUG
#include <cassert>
#define UPGL_ASSERT(expression) assert(expression)
#ifdef _WIN32
#define UPGL_ASSERT_MSG(expression, msg)                                                           \
  (void)((!!(expression)) ||                                                                       \
         (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0))
#else
#define UPGL_ASSERT_MSG(expression, msg) assert(expression)
#endif
#else
#define UPGL_ASSERT(expression)
#define UPGL_ASSERT_MSG(expression, msg)
#define UPGL_LOG_DEBUG(severity, message, ...)
#endif
