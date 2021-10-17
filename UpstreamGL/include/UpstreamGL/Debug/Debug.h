#pragma once

#include "../Config.h"
#include "Profiler.h"
#include "Logger.h"
#include "Console.h"

#ifdef  UPGL_PROFILE
#define GLR_PROFILER_START(section)			Profiler::GetInstance()->StartClock(section)
#define GLR_PROFILER_END()					Profiler::GetInstance()->EndClock()
#define GLR_PROFILER_EXPORT_FILE(filename)	Profiler::GetInstance()->ResultsToText(filename)
#define GLR_PROFILER_DRAW()	                Profiler::GetInstance()->Draw()
#else
#define GLR_PROFILER_START(section)			
#define GLR_PROFILER_END()					
#define GLR_PROFILER_EXPORT_FILE(filename)
#define GLR_PROFILER_DRAW()
#endif

#ifdef  UPGL_DEBUG
#define GLR_DEBUG_LOG(message, ...)              Logger::GetInstance()->Log(LogEntry::None, message, __VA_ARGS__);
#define GLR_DEBUG_LOG_INFO(message, ...)         Logger::GetInstance()->Log(LogEntry::Info, message, __VA_ARGS__);
#define GLR_DEBUG_LOG_WARNING(message, ...)      Logger::GetInstance()->Log(LogEntry::Warning, message, __VA_ARGS__);
#define GLR_DEBUG_LOG_ERROR(message, ...)        Logger::GetInstance()->Log(LogEntry::Error, message, __VA_ARGS__);
#define GLR_DEBUG_LOG_CRITICAL(message, ...)     Logger::GetInstance()->Log(LogEntry::Critical, message, __VA_ARGS__);
#else
#define GLR_DEBUG_LOG(message)          
#define GLR_DEBUG_LOG_INFO(message)     
#define GLR_DEBUG_LOG_WARNING(message)  
#define GLR_DEBUG_LOG_ERROR(message)    
#define GLR_DEBUG_LOG_CRITICAL(message) 
#endif

#define VA_ARGS(...) , ##__VA_ARGS__
#define GLR_LOG(message, ...)                    Logger::GetInstance()->Log(LogEntry::None, message VA_ARGS(__VA_ARGS__));
#define GLR_LOG_INFO(message, ...)               Logger::GetInstance()->Log(LogEntry::Info, message VA_ARGS(__VA_ARGS__));
#define GLR_LOG_WARNING(message, ...)            Logger::GetInstance()->Log(LogEntry::Warning, message VA_ARGS(__VA_ARGS__));
#define GLR_LOG_ERROR(message, ...)              Logger::GetInstance()->Log(LogEntry::Error, message VA_ARGS(__VA_ARGS__));
#define GLR_LOG_CRITICAL(message, ...)           Logger::GetInstance()->Log(LogEntry::Critical, message VA_ARGS(__VA_ARGS__));