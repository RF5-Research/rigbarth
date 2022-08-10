#include "pch-il2cpp.h"
#include "helpers.h"
#include "il2cpp-appdata.h"
#include "utils.h"
#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>
#include <map>
#include <future>
#include <cstring>
#include <nlohmann/json.hpp>
#include "advScript.h"

using json = nlohmann::json;
using namespace app;

namespace advScript
{
	std::map<std::string, int> Scripts;
	std::map<std::string, int> EventScripts;
	std::map<std::string, int> GameFlagData;
	std::vector<int> MarriagableCandidates;

	const char* rigbarthPath = "rigbarth";

	void ResolveGameFlagData(json* data, const char* fieldName, const char* scriptName)
	{
		if (data->is_string())
		{
			auto value = data->get<std::string>();
			auto pos = GameFlagData.find(value);
			if (pos != GameFlagData.end())
			{
				printf("Resolved `%s`: `%s`'s `%s` GameDataFlag\n", scriptName, fieldName, value.c_str());
				*data = pos->second;
			}
			else
			{
				printf("Failed to resolve %s: `%s`'s `%s` GameDataFlag\n", scriptName, fieldName, value.c_str());
			}
		}
	}

	void LoadScripts()
	{
		Scripts = std::map<std::string, int>();
		auto scripts = (*AdvScriptReader__TypeInfo)->static_fields->Scripts;
		int32_t index = scripts->fields._size;
		auto add = reinterpret_cast<void (*)(List_1_System_Byte__1*, Byte__Array * item, MethodInfo*)>(scripts->klass->vtable.Add.methodPtr);
		auto add_MethodInfo = scripts->klass->vtable.Add.method;
		auto dir = std::filesystem::absolute(std::format("{}/AdvScripts", rigbarthPath));
		if (std::filesystem::exists(dir))
		{
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
			{
				if (dirEntry.is_regular_file())
				{
					auto scriptPath = dirEntry.path();
					if (utils::string::iequals(scriptPath.extension().generic_string(), ".advScript"))
					{
						std::ifstream file(scriptPath, std::ios::binary);
						std::vector<uint8_t> bytes(
							(std::istreambuf_iterator<char>(file)),
							(std::istreambuf_iterator<char>()));

						//Won't work due to not being initialized yet
						//auto type = il2cpp_class_get_type((Il2CppClass*)*Byte__TypeInfo);
						//auto type_object = il2cpp_type_get_object(type);
						//Is there a better way of doing this...
						auto type = Type_GetType_2(reinterpret_cast<String*>(il2cpp_string_new("System.Byte, System.Runtime")), nullptr);
						auto array = (Byte__Array*)Array_CreateInstance_1(type, static_cast<int32_t>(bytes.size()), nullptr);
						
						std::copy(bytes.begin(), bytes.end(), array->vector);
						add(scripts, array, const_cast<MethodInfo*>(add_MethodInfo));
						auto scriptName = scriptPath.stem().generic_string();
						std::transform(scriptName.begin(), scriptName.end(), scriptName.begin(), ::toupper);
						Scripts.insert(std::pair<std::string, int>(scriptName, index++));
					}
				}
			}
		}
	}

	void LoadEventUnlockData(EventFlagManager* eventFlagManager)
	{
		EventScripts = std::map<std::string, int>();
		auto dir = std::filesystem::absolute(std::format("{}/EventUnlockData", rigbarthPath));
		auto eventUnlockFlagData = eventFlagManager->fields.EventUnlockFlagDatas->fields.datas;
		auto index = static_cast<int32_t>(EventScriptID__Enum::Max);

		auto add = reinterpret_cast<void (*)(List_1_EventUnlockFlagData_*, app::EventUnlockFlagData* item, MethodInfo*)>(eventUnlockFlagData->klass->vtable.Add.methodPtr);
		auto add_MethodInfo = eventUnlockFlagData->klass->vtable.Add.method;

		auto gameFlagDataType = Type_GetType_2(reinterpret_cast<String*>(il2cpp_string_new("Define.GameFlagData, Assembly-CSharp")), nullptr);
		auto eventUnlockFlagDataType = Type_GetType_2(reinterpret_cast<String*>(il2cpp_string_new("EventUnlockFlagData, Assembly-CSharp")), nullptr);

		const char* ScriptIdFieldName = "ScriptId";
		const char* PointOnFlagFieldName = "PointOnFlag";
		const char* StoryFlagFieldName = "StoryFlag";
		const char* OnFieldName = "On";
		const char* OffFieldName = "Off";
		const char* NpcOnFieldName = "NpcOn";
		const char* NpcOffFieldName = "NpcOff";
		const char* FlagOnFieldName = "FlagOn";
		const char* FlagOffFieldName = "FlagOff";
		const char* PointActiveFieldName = "PointActive";

		if (std::filesystem::exists(dir))
		{
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
			{
				if (dirEntry.is_regular_file())
				{
					auto path = dirEntry.path();
					if (utils::string::iequals(path.extension().generic_string(), ".json"))
					{
						std::ifstream file(path);
						auto json = json::parse(file, nullptr, false);
						if (json.is_discarded())
						{
							printf("Invalid JSON: %s\n", path.generic_string().c_str());
							continue;
						}
						for (int i = 0; i < json.size(); i++)
						{
							auto item = &json[i];
							std::string scriptName;
							scriptName = item->at(ScriptIdFieldName).get<std::string>();
							std::transform(scriptName.begin(), scriptName.end(), scriptName.begin(), ::tolower);

							//Check if already added to our map
							//Nobody should really be using preexisting names
							auto pos = EventScripts.find(scriptName);
							if (pos != EventScripts.end())
							{
								printf("WARNING: Found duplicate EventScript: %s\n", scriptName.c_str());
								item->at(ScriptIdFieldName) = pos->second;
							}
							//If not, continue
							else
							{
								item->at(ScriptIdFieldName) = index;
								EventScripts.insert(std::pair<std::string, int>(scriptName, index));
							}

							//Resolve GameFlagData
							auto pointOnFlag = &item->at(PointOnFlagFieldName);
							ResolveGameFlagData(pointOnFlag, PointOnFlagFieldName, scriptName.c_str());

							auto storyFlag = &item->at(StoryFlagFieldName);
							ResolveGameFlagData(storyFlag, StoryFlagFieldName, scriptName.c_str());

							auto onArray = &item->at(OnFieldName);
							for (json::iterator it = onArray->begin(); it != onArray->end(); ++it)
							{
								ResolveGameFlagData(&*it, OnFieldName, scriptName.c_str());
							}

							auto offArray = &item->at(OffFieldName);
							for (json::iterator it = offArray->begin(); it != offArray->end(); ++it)
							{
								ResolveGameFlagData(&*it, OffFieldName, scriptName.c_str());
							}

							auto npcOnArray = &item->at(NpcOnFieldName);
							for (json::iterator it = npcOnArray->begin(); it != npcOnArray->end(); ++it)
							{
								ResolveGameFlagData(&*it, NpcOnFieldName, scriptName.c_str());
							}

							auto npcOffArray = &item->at(NpcOffFieldName);
							for (json::iterator it = npcOffArray->begin(); it != npcOffArray->end(); ++it)
							{
								ResolveGameFlagData(&*it, NpcOffFieldName, scriptName.c_str());
							}

							auto flagOnArray = &item->at(FlagOnFieldName);
							for (json::iterator it = flagOnArray->begin(); it != flagOnArray->end(); ++it)
							{
								ResolveGameFlagData(&*it, FlagOnFieldName, scriptName.c_str());
							}

							auto flagOffArray = &item->at(FlagOffFieldName);
							for (json::iterator it = flagOffArray->begin(); it != flagOffArray->end(); ++it)
							{
								ResolveGameFlagData(&*it, FlagOffFieldName, scriptName.c_str());
							}

							//Enum_EnumResult result = {};
							////If preexisting EventScriptId, use it instead
							////This feels like a really bad idea
							////Maybe allow user to describe their custom enums
							////Load that and just check against custom map before trying to parse
							////As this could be slow
							//if (Enum_TryParseEnum(
							//		eventScriptIDType,
							//		reinterpret_cast<String*>(il2cpp_string_new(scriptName.c_str())),
							//		false,
							//		&result,
							//		nullptr
							//	)
							//)
							//{
							//	json[i]["ScriptId"] = static_cast<int32_t>(reinterpret_cast<EventScriptID__Enum__Boxed*>(result.parsedEnum)->value;
							//}
							//else
							//{
							//	auto pos = EventScripts.find(scriptName);
							//	//Check if already added to our map
							//	if (pos != EventScripts.end())
							//	{
							//		json[i]["ScriptId"] = pos->second;
							//	}
							//	//If not, continue
							//	else
							//	{
							//		json[i]["ScriptId"] = index;
							//		EventScripts.insert(std::pair<std::string, int>(scriptName, index));
							//	}
							//}

							item->at(PointActiveFieldName) = item->at(PointActiveFieldName).get<bool>() ? 1 : 0;
							EventUnlockFlagData* object = reinterpret_cast<EventUnlockFlagData*>(
								JsonUtility_FromJson(
									reinterpret_cast<String*>(
										il2cpp_string_new(item->dump().c_str())
									),
									eventUnlockFlagDataType,
									nullptr
							));
							add(eventUnlockFlagData, object, const_cast<MethodInfo*>(add_MethodInfo));
							index++;
						}
					}
				}
			}
		}
	}

	//EventScript_ScriptPlayer__Update
	//Read command
	uint64_t Tramp_AdvScriptReader_Init_d_4_MoveNext;
	NOINLINE bool __cdecl Hook_AdvScriptReader_Init_d_4_MoveNext(AdvScriptReader_Init_d_4* __this, MethodInfo* method)
	{
		auto result = PLH::FnCast(Tramp_AdvScriptReader_Init_d_4_MoveNext, Hook_AdvScriptReader_Init_d_4_MoveNext)(__this, method);
		if (!result)
			LoadScripts();
		return result;
	}

	uint64_t Tramp_Enum_TryParse_AdvScriptID;
	NOINLINE bool __cdecl Hook_Enum_TryParse_AdvScriptID(String* value, int32_t* null, int32_t* result)
	{
		auto success = PLH::FnCast(Tramp_Enum_TryParse_AdvScriptID, Hook_Enum_TryParse_AdvScriptID)(value, null, result);
		auto enumString = il2cppi_to_string(value);

		if (!success)
		{ 
			auto pos = Scripts.find(enumString);
			if (pos != Scripts.end())
			{
				printf("Found AdvScript: %s\n", enumString.c_str());
				*result = pos->second;
				return 1;
			}
			else
			{
				printf("Failed to find AdvScript: %s\n", enumString.c_str());
				return success;
			}
		}
		printf("Loading builtin AdvScript: %s\n", enumString.c_str());
		return success;
	}

	uint64_t Tramp_Enum_TryParse_EventScriptID;
	NOINLINE bool __cdecl Hook_Enum_TryParse_EventScriptID(String* value, int32_t* null, int32_t* result)
	{
		auto success = PLH::FnCast(Tramp_Enum_TryParse_EventScriptID, Hook_Enum_TryParse_EventScriptID)(value, null, result);
		auto enumString = il2cppi_to_string(value);
		if (!success)
		{
			auto pos = EventScripts.find(enumString);
			if (pos != EventScripts.end())
			{
				printf("Found EventScript: %s\n", enumString.c_str());
				*result = pos->second;
				return 1;
			}
			else
			{
				printf("Failed to find EventScript: %s\n", enumString.c_str());
				return success;
			}
		}
		printf("Loading builtin EventScript: %s\n", enumString.c_str());
		return success;
	}

	uint64_t Tramp_EventScriptId__Enum__ToString;
	NOINLINE String* __cdecl Hook_EventScriptId__Enum__ToString(Enum__Boxed* __this, MethodInfo* method)
	{
		auto value = PLH::FnCast(Tramp_EventScriptId__Enum__ToString, Hook_EventScriptId__Enum__ToString)(__this, method);
		auto enumString = il2cppi_to_string(value);
		if (!std::any_of(enumString.begin(), enumString.end(), isalpha))
		{
			auto id = std::stoi(enumString);
			for (auto it = EventScripts.begin(); it != EventScripts.end(); ++it)
			{
				if (it->second == id)
				{
					printf("Found EventScript %s:\n", it->first.c_str());
					return reinterpret_cast<String*>(il2cpp_string_new(it->first.c_str()));
				}
			}
			printf("Failed to find EventScript %s:\n", enumString.c_str());
		}
		return value;
	}

	PLH::x64Detour* Detour_EventFlagManager_Start;
	uint64_t Tramp_EventFlagManager_Start;
	NOINLINE void __cdecl Hook_EventFlagManager_Start(EventFlagManager* __this, MethodInfo* method)
	{
		PLH::FnCast(Tramp_EventFlagManager_Start, Hook_EventFlagManager_Start)(__this, method);
		LoadEventUnlockData(__this);
		EventFlagManager_InitEventStartPoints(__this, nullptr);
		//Call again to initialize with our own data
		//TODO: Figure out what exactly they do as this is a bit hacky
		//PLH::FnCast(Tramp_EventFlagManager_Start, Hook_EventFlagManager_Start)(__this, method);

		Detour_EventFlagManager_Start->unHook();
	}
	//EventStartPoint->Enable

	uint64_t Tramp_NpcDataManager_SetNpcStatusData;
	NOINLINE void __cdecl Hook_NpcDataManager_SetNpcStatusData(NpcDataManager* __this, MethodInfo* method)
	{
		PLH::FnCast(Tramp_NpcDataManager_SetNpcStatusData, Hook_NpcDataManager_SetNpcStatusData)(__this, method);
		auto npcDatas = __this->fields.NpcDatas;

		//Modify our candidates
		auto npcs = npcDatas->fields._items->vector;
		for (int i = 0; i < npcDatas->fields._size; i++)
		{
			auto npc = npcs[i];
			if (npc->fields.statusData)
			{
				for (auto candidate : MarriagableCandidates)
				{
					if (npc->fields.NpcId == candidate)
					{
						npc->fields.statusData->fields.MarriageCandidate = true;
						break;
					}
				}
			}
		}
	}

	uint64_t Tramp_ConditionsForLoverJudgment_CheckCanbeLoverNPCID;
	//Remove their hardcoding
	NOINLINE bool __cdecl Hook_ConditionsForLoverJudgment_CheckCanbeLoverNPCID(NPCID__Enum npcid, MethodInfo* method)
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

		auto detour_ConditionsForLoverJudgment_CheckCanbeLoverNPCID = new PLH::x64Detour(
			reinterpret_cast<char*>(ConditionsForLoverJudgment_CheckCanbeLoverNPCID),
			reinterpret_cast<char*>(Hook_ConditionsForLoverJudgment_CheckCanbeLoverNPCID),
			&Tramp_ConditionsForLoverJudgment_CheckCanbeLoverNPCID,
			dis
		);
		detour_ConditionsForLoverJudgment_CheckCanbeLoverNPCID->hook();

		auto detour_NpcDataManager_SetNpcStatusData = new PLH::x64Detour(
			reinterpret_cast<char*>(NpcDataManager_SetNpcStatusData),
			reinterpret_cast<char*>(Hook_NpcDataManager_SetNpcStatusData),
			&Tramp_NpcDataManager_SetNpcStatusData,
			dis
		);
		detour_NpcDataManager_SetNpcStatusData->hook();

		auto detour_AdvScriptReader_Init_d_4_MoveNext = new PLH::x64Detour(
			reinterpret_cast<char*>(AdvScriptReader_Init_d_4_MoveNext),
			reinterpret_cast<char*>(Hook_AdvScriptReader_Init_d_4_MoveNext),
			&Tramp_AdvScriptReader_Init_d_4_MoveNext,
			dis
		);
		detour_AdvScriptReader_Init_d_4_MoveNext->hook();

		Detour_EventFlagManager_Start = new PLH::x64Detour(
			reinterpret_cast<char*>(EventFlagManager_Start),
			reinterpret_cast<char*>(Hook_EventFlagManager_Start),
			&Tramp_EventFlagManager_Start,
			dis
		);
		Detour_EventFlagManager_Start->hook();
		
		auto task_LoadGameFlagData = std::async(std::launch::async, []() -> void
		{
			GameFlagData = std::map<std::string, int>();
			auto dir = std::filesystem::absolute(std::format("{}/GameFlagData", rigbarthPath));
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
			{
				if (dirEntry.is_regular_file())
				{
					auto path = dirEntry.path();
					if (utils::string::iequals(path.extension().generic_string(), ".json"))
					{
						std::ifstream file(path);
						auto json = json::parse(file, nullptr, false);
						if (json.is_discarded())
						{
							printf("Invalid JSON: %s\n", path.generic_string().c_str());
							continue;
						}
						for (auto& [key, value] : json.items()) {
							if (value.is_number_integer())
							{
								auto id = value.get<int>();
								bool isDuplicate = false;
								for (auto it = GameFlagData.begin(); it != GameFlagData.end(); ++it)
								{
									if (it->second == id)
									{
										printf("WARNING: GameFlagData %s contains duplicate ID: %d", key.c_str(), id);
										isDuplicate = true;
										break;
									}
								}

								if (!isDuplicate)
								{
									GameFlagData.insert(std::pair<std::string, int>(key, value));
								}
							}
						}
					}
				}
			}
		});

		
		auto task_LoadMarriagableCandidates = std::async(std::launch::async, []() -> void
		{
			MarriagableCandidates = std::vector<int>();
			auto dir = std::filesystem::absolute(std::format("{}/MarriagableCandidates", rigbarthPath));
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
			{
				if (dirEntry.is_regular_file())
				{
					auto path = dirEntry.path();
					if (utils::string::iequals(path.extension().generic_string(), ".json"))
					{
						std::ifstream file(path);
						auto json = json::parse(file, nullptr, false);
						if (json.is_discarded())
						{
							printf("Invalid JSON: %s\n", path.generic_string().c_str());
							continue;
						}
						for (int i = 0; i < json.size(); i++)
						{
							if (json[i].is_number_integer())
							{
								MarriagableCandidates.push_back(json[i].get<int>());
							}
						}
					}
				}
			}
		});
		//if ((*Enum__TypeInfo)->_1.cctor_finished && (*Enum__TypeInfo)->_1.initialized)
		//{
		//	il2cpp_runtime_class_init((Il2CppClass*)(*Enum__TypeInfo));
		//}

		//EventControllerBase__SetEventFlagPoint -> FlagDataStorage__SetScriptFlag is related to adding Active Points
		//NpcData__get_LoveStroyState: Important for proposescript
		//UILoveEventConvTextData in MasterData
		auto task_Enum_TryParse_AdvScriptID = std::async(std::launch::async, []() -> void
		{
			while (true)
			{
				if (il2cppi_is_initialized(Enum_TryParse_1__MethodInfo))
				{
					Tramp_Enum_TryParse_AdvScriptID = uint64_t((*Enum_TryParse_1__MethodInfo)->Il2CppVariant.rgctx_data->method->methodPointer);
					const_cast<MethodInfo*>((*Enum_TryParse_1__MethodInfo)->Il2CppVariant.rgctx_data->method)->methodPointer = reinterpret_cast<Il2CppMethodPointer>(&Hook_Enum_TryParse_AdvScriptID);
					break;
				}
			}
		});

		auto task_EventScriptId__Enum__ToString = std::async(std::launch::async, []() -> void
		{
			while (true)
			{
				if (il2cppi_is_initialized(EventScriptID__Enum__TypeInfo))
				{
					Tramp_EventScriptId__Enum__ToString = uint64_t((*EventScriptID__Enum__TypeInfo)->vtable.ToString.methodPtr);
					(*EventScriptID__Enum__TypeInfo)->vtable.ToString.methodPtr = reinterpret_cast<Il2CppMethodPointer>(&Hook_EventScriptId__Enum__ToString);
					break;
				}
			}
		});

		auto task_Enum_TryParse_EventScriptID = std::async(std::launch::async, []() -> void
		{
			while (true)
			{
				if (il2cppi_is_initialized(Enum_TryParse_3__MethodInfo))
				{
					Tramp_Enum_TryParse_EventScriptID = uint64_t((*Enum_TryParse_3__MethodInfo)->Il2CppVariant.rgctx_data->method->methodPointer);
					const_cast<MethodInfo*>((*Enum_TryParse_3__MethodInfo)->Il2CppVariant.rgctx_data->method)->methodPointer = reinterpret_cast<Il2CppMethodPointer>(&Hook_Enum_TryParse_EventScriptID);
					break;
				}
			}
		});
	}
}
