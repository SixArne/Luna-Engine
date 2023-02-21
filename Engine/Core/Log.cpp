#include "pch.h"
#include "Log.h"

std::shared_ptr<spdlog::logger> Engine::Log::s_Logger{ nullptr };

void Engine::Log::Init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	spdlog::enable_backtrace(32);
	s_Logger = spdlog::stdout_color_mt("ENGINE");
	s_Logger->set_level(spdlog::level::trace);

	s_Logger->info("Logger initialized and ready for use.");
}