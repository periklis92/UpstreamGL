#pragma once

#include "../Config.h"
#include "Profiler.h"
#include "Logger.h"
#include "Console.h"

#ifdef  UPGL_PROFILE
#define UPGL_PROFILER_START(section)			Profiler::GetInstance()->StartClock(section)
#define UPGL_PROFILER_END()					Profiler::GetInstance()->EndClock()
#define UPGL_PROFILER_EXPORT_FILE(filename)	Profiler::GetInstance()->ResultsToText(filename)
#define UPGL_PROFILER_DRAW()	                Profiler::GetInstance()->Draw()
#else
#define UPGL_PROFILER_START(section)			
#define UPGL_PROFILER_END()					
#define UPGL_PROFILER_EXPORT_FILE(filename)
#define UPGL_PROFILER_DRAW()
#endif

#ifdef  UPGL_DEBUG
#define UPGL_DEBUG_LOG(message, ...)              Logger::GetInstance()->Log(LogEntry::None, message, __VA_ARGS__);
#define UPGL_DEBUG_LOG_INFO(message, ...)         Logger::GetInstance()->Log(LogEntry::Info, message, __VA_ARGS__);
#define UPGL_DEBUG_LOG_WARNING(message, ...)      Logger::GetInstance()->Log(LogEntry::Warning, message, __VA_ARGS__);
#define UPGL_DEBUG_LOG_ERROR(message, ...)        Logger::GetInstance()->Log(LogEntry::Error, message, __VA_ARGS__);
#define UPGL_DEBUG_LOG_CRITICAL(message, ...)     Logger::GetInstance()->Log(LogEntry::Critical, message, __VA_ARGS__);
#else
#define UPGL_DEBUG_LOG(message)          
#define UPGL_DEBUG_LOG_INFO(message)     
#define UPGL_DEBUG_LOG_WARNING(message)  
#define UPGL_DEBUG_LOG_ERROR(message)    
#define UPGL_DEBUG_LOG_CRITICAL(message) 
#endif

#define VA_ARGS(...) , ##__VA_ARGS__
#define UPGL_LOG(message, ...)                    Logger::GetInstance()->Log(LogEntry::None, message VA_ARGS(__VA_ARGS__));
#define UPGL_LOG_INFO(message, ...)               Logger::GetInstance()->Log(LogEntry::Info, message VA_ARGS(__VA_ARGS__));
#define UPGL_LOG_WARNING(message, ...)            Logger::GetInstance()->Log(LogEntry::Warning, message VA_ARGS(__VA_ARGS__));
#define UPGL_LOG_ERROR(message, ...)              Logger::GetInstance()->Log(LogEntry::Error, message VA_ARGS(__VA_ARGS__));
#define UPGL_LOG_CRITICAL(message, ...)           Logger::GetInstance()->Log(LogEntry::Critical, message VA_ARGS(__VA_ARGS__));