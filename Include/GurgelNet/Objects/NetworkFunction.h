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

#define ServerFunc_0arg( Name )				NetFunc_0Arg( ENetFuncLocality_Server, Name )
#define ServerFunc_1arg( Name, T1 )			NetFunc_1Arg( ENetFuncLocality_Server, Name, T1 )
#define ServerFunc_2arg( Name, T1, T2 )		NetFunc_2Arg( ENetFuncLocality_Server, Name, T1, T2 )
#define ServerFunc_3arg( Name, T1, T2, T3 ) NetFunc_3Arg( ENetFuncLocality_Server, Name, T1, T2, T3 )
#define ServerFuncImpl( Name ) void Name##_ImplNOCALL

// ------------------------------------------------------------
// ClientFunc
// ------------------------------------------------------------
// A function that is callable on server but will execute on 
// client(s)
// 
// If called on a client, behaves like a normal function call
// ------------------------------------------------------------

#define ClientFunc_0arg( Name )				NetFunc_0Arg( ENetFuncLocality_Client, Name )
#define ClientFunc_1arg( Name, T1 )			NetFunc_1Arg( ENetFuncLocality_Client, Name, T1 )
#define ClientFunc_2arg( Name, T1, T2 )		NetFunc_2Arg( ENetFuncLocality_Client, Name, T1, T2 )
#define ClientFunc_3arg( Name, T1, T2, T3 ) NetFunc_3Arg( ENetFuncLocality_Client, Name, T1, T2, T3 )
#define ClientFuncImpl( Name )				void Name##_ImplNOCALL

// ------------------------------------------------------------
// ------------------------------------------------------------
// ------------------------------------------------------------