#pragma once
#include "GurgelNet/NetLib.h"
#include "GurgelNet/Core/NetLogging.h"

#include <string>
#include <string_view>
#include <format>

#define NET_LOG( Level, Msg, ... ) CNetLogger::LogMessage( Level, logger::FormatLogMsg( Msg , __VA_ARGS__ ) )

namespace logger
{
	template<typename ... TFmt>
	std::string FormatLogMsg(std::string_view str, TFmt&& ... fmt)
	{
		return std::vformat(str, std::make_format_args(fmt...));
	}
}

class CNetLogger
{
public:
	static void LogMessage(ENetLogLevel l, const std::string& msg);
	static FNetLibLogCallback _logCallback;
};
