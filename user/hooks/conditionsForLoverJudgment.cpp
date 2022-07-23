#include "pch-il2cpp.h"
#include "il2cpp-appdata.h"
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>

using namespace app;

namespace conditionsForLoverJudgement
{
	uint64_t Tramp_ConditionsForLoverJudgment_CheckConditionsForLoverJudgment;
	NOINLINE bool __cdecl Hook_ConditionsForLoverJudgment_CheckConditionsForLoverJudgment(
		int32_t npcid,
		int32_t lovelv,
		int32_t eventType,
		int32_t eventStep1,
		int32_t eventStep2,
		MethodInfo* method)
	{
		//TODO: Handle according based on npcid;
		return true;
	}

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);

		auto detour_ConditionsForLoverJudgment_CheckConditionsForLoverJudgment = new PLH::x64Detour(
			reinterpret_cast<char*>(ConditionsForLoverJudgment_CheckConditionsForLoverJudgment),
			reinterpret_cast<char*>(Hook_ConditionsForLoverJudgment_CheckConditionsForLoverJudgment),
			&Tramp_ConditionsForLoverJudgment_CheckConditionsForLoverJudgment,
			dis
		);
		detour_ConditionsForLoverJudgment_CheckConditionsForLoverJudgment->hook();
	}
}