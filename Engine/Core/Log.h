#ifndef LOG_H
#define LOG_H

#include "pch.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; };

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#ifdef DEBUG
#define L_TRACE(...)	::Engine::Log::GetLogger()->trace(__VA_ARGS__);
#define L_INFO(...)		::Engine::Log::GetLogger()->info(__VA_ARGS__);
#define L_DEBUG(...)	::Engine::Log::GetLogger()->debug(__VA_ARGS__);
#define L_WARN(...)		::Engine::Log::GetLogger()->warn(__VA_ARGS__);
#define L_ERROR(...)	::Engine::Log::GetLogger()->error(__VA_ARGS__);

#define L_DUMP()		::Engine::Log::GetLogger()->dump_backtrace();

#else
#define L_TRACE(...)
#define L_INFO(...)
#define L_DEBUG(...)
#define L_WARN(...)
#define L_ERROR(...)	

#define L_DUMP()

#endif

#endif
