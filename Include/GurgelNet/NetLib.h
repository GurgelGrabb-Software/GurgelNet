#pragma once

#define AS_DLL 0

#if AS_DLL
	#if NETWORK_LIB
		#define NETWORK_API __declspec(dllexport)
	#else
		#define NETWORK_API __declspec(dllimport)
	#endif // NETWORK_LIB
#else
	#define NETWORK_API
#endif // AS_DLL

