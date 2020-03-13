#include "Debug.h"


Verbosity Debug::DefaultVerbosity = Verbosity::Debug;
bool Debug::debugOn = true;

inline void Debug::SetPattern(std::string pattern)
{
	spdlog::set_pattern(pattern);
}

void Debug::SetDebugState(bool newState)
{
	debugOn = newState;
}

void Debug::SetDefaultVerbosity(Verbosity defaultVerbosity)
{
	DefaultVerbosity = defaultVerbosity;
}
