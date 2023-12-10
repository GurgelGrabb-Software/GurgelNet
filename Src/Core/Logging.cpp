#include "Logging.h"

FNetLibLogCallback CNetLogger::_logCallback = nullptr;

void CNetLogger::LogClientMessage(ENetLogLevel level, const std::string& msg)
{
	if (_logCallback)
	{
		const std::string fullmsg = logger::FormatLogMsg("{} {}", "|Client|", msg);
		_logCallback(level, fullmsg.c_str());
	}
}

void CNetLogger::LogServerMessage(ENetLogLevel l, const std::string& msg)
{
	if (_logCallback)
	{
		const std::string fullmsg = logger::FormatLogMsg("{} {}", "|Server|", msg);
		_logCallback(l, fullmsg.c_str());
	}
}
