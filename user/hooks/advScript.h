#pragma once
#include "pch-il2cpp.h"
#include "il2cpp-appdata.h"
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>

using namespace app;

namespace advScript
{
	extern uint64_t Tramp_Enum_TryParse_EventScriptID;
	NOINLINE bool __cdecl Hook_Enum_TryParse_EventScriptID(String* value, int32_t* null, int32_t* result);

	extern uint64_t Tramp_EventFlagManager_Start;
	NOINLINE void __cdecl Hook_EventFlagManager_Start(EventFlagManager* __this, MethodInfo* method);

	extern uint64_t Tramp_EventScriptId__Enum__ToString;
	NOINLINE String* __cdecl Hook_EventScriptId__Enum__ToString(Enum__Boxed* __this, MethodInfo* method);

	extern uint64_t Tramp_Enum_TryParse_AdvScriptID;
	NOINLINE bool __cdecl Hook_Enum_TryParse_AdvScriptID(String* value, int32_t* null, int32_t* result);

	extern uint64_t Tramp_AdvScriptReader_Init_d_4_MoveNext;
	NOINLINE bool __cdecl Hook_AdvScriptReader_Init_d_4_MoveNext(AdvScriptReader_Init_d_4* __this, MethodInfo* method);
}
