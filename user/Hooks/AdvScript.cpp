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
#include <bit>
#include "Constants.hpp"
#include "AdvCommands.hpp"

using json = nlohmann::json;
using namespace app;

namespace advScript
{
	std::vector<Sprite*> MapIcons;

	uint64_t Tramp_SubEventManager__ctor;
	NOINLINE void __cdecl Hook_SubEventManager__ctor(SubEventManager* __this, MethodInfo* method)
	{
		PLH::FnCast(Tramp_SubEventManager__ctor, Hook_SubEventManager__ctor)(__this, method);
		auto add = reinterpret_cast<void (*)(List_1_SubEventMasterDataBase_*, SubEventMasterDataBase* item, const MethodInfo*)>(__this->fields.SubEventDatas->klass->vtable.Add.methodPtr);
		auto addMethodInfo = __this->fields.SubEventDatas->klass->vtable.Add.method;
		auto type = Type_GetType_2(reinterpret_cast<String*>(il2cpp_string_new("SubEventMasterDataBase, Assembly-CSharp")), nullptr);

		auto dir = std::filesystem::absolute(std::format("{}/SubEventData", Constants::RigbarthPath));
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
						SubEventMasterDataBase* object = reinterpret_cast<SubEventMasterDataBase*>(
							JsonUtility_FromJson(
								reinterpret_cast<String*>(
									il2cpp_string_new(json[i].dump().c_str())
									),
								type,
								nullptr
							));
						add(__this->fields.SubEventDatas, object, addMethodInfo);
					}
				}
			}
		}
	}

	uint64_t Tramp_MapIconMng_getQuestIcon;
	NOINLINE Sprite* __cdecl Hook_MapIconMng_getQuestIcon(MapIconMng* __this, int32_t iconNo, MethodInfo* method)
	{
		return MapIcons[0];
	}

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);

		//{
		//	auto dir = std::filesystem::absolute(std::format("{}/Assets/map_pins.bundle", rigbarthPath));
		//	auto type = Type_GetType_2(reinterpret_cast<String*>(il2cpp_string_new("UnityEngine.U2D.SpriteAtlas, UnityEngine.CoreModule")), nullptr);

		//	AssetBundleCreateRequest* createRequest = AssetBundle_LoadFromFileAsync((String*)il2cpp_string_new(dir.generic_string().c_str()), nullptr);
		//	while (!AsyncOperation_get_isDone((AsyncOperation*)createRequest, nullptr))
		//	{

		//	}
		//	AssetBundle* ab = AssetBundleCreateRequest_get_assetBundle(createRequest, nullptr);
		//	AssetBundleRequest* assetRequest = AssetBundle_LoadAssetAsync(ab, (String*)il2cpp_string_new("assets/sprites/map_pins.spriteatlas"), type, nullptr);
		//	while (!AsyncOperation_get_isDone((AsyncOperation*)assetRequest, nullptr))
		//	{

		//	}
		//	auto x = (SpriteAtlas*)AssetBundleRequest_get_asset(assetRequest, nullptr);
		//	if (x)
		//	{
		//		//MapIcons = std::vector<Sprite*>();
		//		//MapIcons.push_back(x);
		//	}
		//}
		
		//auto detour_MapIconMng_getQuestIcon = new PLH::x64Detour(
		//	reinterpret_cast<char*>(MapIconMng_getQuestIcon),
		//	reinterpret_cast<char*>(Hook_MapIconMng_getQuestIcon),
		//	&Tramp_MapIconMng_getQuestIcon,
		//	dis
		//);
		//detour_MapIconMng_getQuestIcon->hook();

		//auto detour_SubEventManager__ctor = new PLH::x64Detour(
		//	reinterpret_cast<char*>(SubEventManager__ctor),
		//	reinterpret_cast<char*>(Hook_SubEventManager__ctor),
		//	&Tramp_SubEventManager__ctor,
		//	dis
		//);
		//detour_SubEventManager__ctor->hook();

		//if ((*Enum__TypeInfo)->_1.cctor_finished && (*Enum__TypeInfo)->_1.initialized)
		//{
		//	il2cpp_runtime_class_init((Il2CppClass*)(*Enum__TypeInfo));
		//}

		//EventControllerBase__SetEventFlagPoint -> FlagDataStorage__SetScriptFlag is related to adding Active Points
		//NpcData__get_LoveStroyState: Important for proposescript
		//UILoveEventConvTextData in MasterData
	}
}
