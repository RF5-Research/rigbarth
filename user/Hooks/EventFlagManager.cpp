#include <utils.h>
#include <Constants.hpp>
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>
#include <nlohmann/json.hpp>
#include "Enums.hpp"
#include "il2cpp-appdata.h"

using json = nlohmann::json;

namespace Hooks::EventFlagManager
{
	void ResolveGameFlagData(json* data, const char* fieldName, const char* scriptName)
	{
		if (data->is_string())
		{
			auto value = data->get<std::string>();
			auto pos = Enums::GameFlagData.find(value);
			if (pos != Enums::GameFlagData.end())
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

	void LoadEventUnlockData(app::EventFlagManager* eventFlagManager)
	{
		auto dir = std::filesystem::absolute(std::format("{}/EventUnlockData", Constants::RigbarthPath));
		auto eventUnlockFlagData = eventFlagManager->fields.EventUnlockFlagDatas->fields.datas;
		auto index = static_cast<int32_t>(app::EventScriptID__Enum::Max);

		auto add = reinterpret_cast<void (*)(app::List_1_EventUnlockFlagData_*, app::EventUnlockFlagData * item, MethodInfo*)>(eventUnlockFlagData->klass->vtable.Add.methodPtr);
		auto add_MethodInfo = eventUnlockFlagData->klass->vtable.Add.method;

		auto gameFlagDataType = app::Type_GetType_2(reinterpret_cast<app::String*>(il2cpp_string_new("Define.GameFlagData, Assembly-CSharp")), nullptr);
		auto eventUnlockFlagDataType = app::Type_GetType_2(reinterpret_cast<app::String*>(il2cpp_string_new("EventUnlockFlagData, Assembly-CSharp")), nullptr);

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
							auto pos = Enums::EventScriptID.find(scriptName);
							if (pos != Enums::EventScriptID.end())
							{
								printf("WARNING: Found duplicate EventScript: %s\n", scriptName.c_str());
								item->at(ScriptIdFieldName) = pos->second;
							}
							//If not, continue
							else
							{
								item->at(ScriptIdFieldName) = index;
								Enums::EventScriptID.emplace(scriptName, index);
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
							app::EventUnlockFlagData* object = reinterpret_cast<app::EventUnlockFlagData*>(
								app::JsonUtility_FromJson(
									reinterpret_cast<app::String*>(
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

	uint64_t Tramp_EventFlagManager_Start;
	NOINLINE void __cdecl Hook_EventFlagManager_Start(app::EventFlagManager* __this, MethodInfo* method)
	{
		PLH::FnCast(Tramp_EventFlagManager_Start, Hook_EventFlagManager_Start)(__this, method);
		LoadEventUnlockData(__this);
		app::EventFlagManager_InitEventStartPoints(__this, nullptr);
	}

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);

		auto detour_EventFlagManager_Start = new PLH::x64Detour(
			reinterpret_cast<char*>(app::EventFlagManager_Start),
			reinterpret_cast<char*>(Hook_EventFlagManager_Start),
			&Tramp_EventFlagManager_Start,
			dis
		);
		detour_EventFlagManager_Start->hook();

	}
}