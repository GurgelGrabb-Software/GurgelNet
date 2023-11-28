#include "GurgelNet/Core/LibrarySettings.h"

SNetLibSetting::SNetLibSetting()
	: type(ENetLibSetting_None)
	, data()
{
}

SNetLibSetting::SNetLibSetting(const SNetLibSetting& s)
	: type(s.type)
	, data(s.data)
{
}

SNetLibSetting::SNetLibSetting(SNetLibSetting&& s) noexcept
	: type(s.type)
	, data(s.data)
{
}

SNetLibSetting::SNetLibSetting(ENetLibSetting setting, int iVal)
	: type(setting)
	, data()
{
	data.s_int = iVal;
}

SNetLibSetting::SNetLibSetting(ENetLibSetting setting, bool bVal)
	: type(setting)
	, data()
{
	data.s_flag = bVal;
}

SNetLibSetting::SNetLibSetting(ENetLibSetting setting, void* ptrVal)
	: type(setting)
	, data()
{
	data.s_ptr = ptrVal;
}

void SNetLibSetting::SetInt(ENetLibSetting s, int iVal)
{
	type = s;
	data.s_int = iVal;
}

void SNetLibSetting::SetFlag(ENetLibSetting s, bool bVal)
{
	type = s;
	data.s_flag = bVal;
}

void SNetLibSetting::SetPtr(ENetLibSetting s, void* ptrVal)
{
	type = s;
	data.s_ptr = ptrVal;
}
