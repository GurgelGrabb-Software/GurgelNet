// GurgelNet - 2023
// ------------------------------------------------------------
// LibrarySettings.h 
// ------------------------------------------------------------
// ------------------------------------------------------------

#pragma once
#include "GurgelNet/NetLib.h"

enum NETWORK_API ENetLibSetting
{
	ENetLibSetting_None			= 0x00,				// No setting (used for defaulting, does nothing when passed in)

	ENetLibSetting_Callback_Log	= 0x01,				// Callback to recieve logging output from the library ( void(ENetLogLevel level, const char* message) ) 

	ENetLibSetting_Multithread,						// Run all net layers on separate threads
};

struct NETWORK_API SNetLibSetting
{
	SNetLibSetting();
	SNetLibSetting(const SNetLibSetting& s);
	SNetLibSetting(SNetLibSetting&& s) noexcept;
	SNetLibSetting(ENetLibSetting setting, int iVal);
	SNetLibSetting(ENetLibSetting setting, bool bVal);
	SNetLibSetting(ENetLibSetting setting, void* ptrVal);

	void SetInt(ENetLibSetting s, int iVal);
	void SetFlag(ENetLibSetting s, bool bVal);
	void SetPtr(ENetLibSetting s, void* ptrVal);

	ENetLibSetting type;
	
	union USettingData
	{
		int s_int;
		bool s_flag;
		void* s_ptr;
	} data;
};