#include "Logging.h"

FNetLibLogCallback CNetLogger::_logCallback = nullptr;

void CNetLogger::LogMessage(ENetLogLevel level, const std::string& msg)
{
	if (_logCallback)
	{
		_logCallback(level, msg.c_str());
	}
}