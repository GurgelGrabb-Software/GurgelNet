// GurgelNet - 2023
// ------------------------------------------------------------

#pragma once

#include "GurgelNet/Core/NetTypes.h"

#include "GurgelNet/Objects/NetFunctionsInternal/NetFunctionHandle.h"
#include "GurgelNet/Objects/NetFunctionsInternal/TemplateHandles.h"
#include "GurgelNet/Objects/NetFunctionsInternal/InternalMacros.h"

// ------------------------------------------------------------
// Using these macros
// ------------------------------------------------------------
// The non-impl macro goes in the header.
// Args are the parameter types (will be const&)
//
// The Impl macro is to be used in the cpp
// 
// Example to make a void ServerFunc(const int& a):
// In .h:	ServerFunc( ServerFunc, int )
// In .cpp:	ServerFuncImpl( ServerFunc )(const int& a)
//			{
//				impl Code
//			}
// ------------------------------------------------------------

// ------------------------------------------------------------
// ServerFunc
// ------------------------------------------------------------
// A function that is callable on clients but will execute on 
// server
// 
// If called on server, behaves like a normal function call
// ------------------------------------------------------------

#define ServerFunc( Name, ... ) NF_M_SELECTOR( __VA_ARGS__, NetFunc_3Arg, NetFunc_2Arg, NetFunc_1Arg, NetFunc_0Arg )( ENetFuncLocality_Server, Name, __VA_ARGS__ )
#define ServerFuncImpl( Name ) void Name##_ImplNOCALL

// ------------------------------------------------------------
// ClientFunc
// ------------------------------------------------------------
// A function that is callable on server but will execute on 
// client(s)
// 
// If called on a client, behaves like a normal function call
// ------------------------------------------------------------

#define ClientFunc( Name, ... ) NF_M_SELECTOR( __VA_ARGS__, NetFunc_3Arg, NetFunc_2Arg, NetFunc_1Arg, NetFunc_0Arg )( ENetFuncLocality_Client, Name, __VA_ARGS__ )
#define ClientFuncImpl( Name ) void Name##_ImplNOCALL

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------