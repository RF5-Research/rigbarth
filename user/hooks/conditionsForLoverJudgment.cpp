#include "il2cpp-appdata.h"
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>

using namespace app;

namespace Hooks::ConditionsForLoverJudgment
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

	//Undo their hardcoding
	uint64_t Tramp_ConditionsForLoverJudgment_CheckCanbeLoverNPCID;
	NOINLINE bool __cdecl Hook_ConditionsForLoverJudgment_CheckCanbeLoverNPCID(
		NPCID__Enum npcid,
		MethodInfo* method)
	{
		auto saveData = app::SaveDataManager_get_GameSaveData(nullptr);
		auto playerData = saveData->fields.PlayerData;
		auto marriedNPCID = playerData->fields.MarriedNPCID;
		if (static_cast<int>(marriedNPCID) > 2)
			return marriedNPCID == npcid;
		else
		{
			auto npcDataManager = SingletonMonoBehaviour_1_NpcDataManager__get_Instance(*SingletonMonoBehaviour_1_NpcDataManager__get_Instance__MethodInfo);
			auto npcData = NpcDataManager_GetNpcData_1(npcDataManager, npcid, nullptr);
			return npcData->fields.statusData->fields.MarriageCandidate;
		}
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


		auto detour_ConditionsForLoverJudgment_CheckCanbeLoverNPCID = new PLH::x64Detour(
			reinterpret_cast<char*>(ConditionsForLoverJudgment_CheckCanbeLoverNPCID),
			reinterpret_cast<char*>(Hook_ConditionsForLoverJudgment_CheckCanbeLoverNPCID),
			&Tramp_ConditionsForLoverJudgment_CheckCanbeLoverNPCID,
			dis
		);
		detour_ConditionsForLoverJudgment_CheckCanbeLoverNPCID->hook();

	}
}