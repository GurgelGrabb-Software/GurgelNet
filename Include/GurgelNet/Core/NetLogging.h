// GurgelNet - 2023
// ------------------------------------------------------------
// NetLogging.h 
// ------------------------------------------------------------
// ------------------------------------------------------------

#pragma once

enum ENetLogLevel
{
	ENetLogLevel_Verbose	= 0x01,
	ENetLogLevel_Message	= 0x02,
	ENetLogLevel_Warning	= 0x03,
	ENetLogLevel_Error		= 0x04,
	ENetLogLevel_Confirm	= 0x05,
};

typedef void(*FNetLibLogCallback)(ENetLogLevel, const char*);