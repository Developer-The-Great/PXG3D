#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <iostream>
#include <string>


enum class Verbosity
{
	Default,
	Info,
	Debug,
	Warning,
	Error
};

class Debug
{
public:

	template<typename ...Args>
	static void Log(Verbosity verbosity, std::string str, const Args & ...args)
	{
		if (!debugOn) { return; }

		Verbosity verbosityUsed = verbosity;

		switch (verbosity)
		{
		case Verbosity::Info:
			spdlog::set_level(spdlog::level::info);
			spdlog::info(str, args...);
			break;
		case Verbosity::Debug:
			spdlog::set_level(spdlog::level::debug);
			spdlog::debug(str,args...);
			break;
		case Verbosity::Warning:
			spdlog::set_level(spdlog::level::warn);
			spdlog::warn(str, args...);
			break;
		case Verbosity::Error:
			spdlog::set_level(spdlog::level::err);
			spdlog::error(str, args...);
			break;

		default:
			break;
		}

	};

	template<typename ...Args>
	static void Log(std::string str, const Args & ...args)
	{
		Log(DefaultVerbosity, str, args...);

	};

	static void SetPattern(std::string pattern);

	static void SetDebugState(bool newState);

	static void SetDefaultVerbosity(Verbosity defaultVerbosity);

private:

	static bool debugOn;

	static Verbosity DefaultVerbosity;
};


