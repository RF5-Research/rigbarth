#include <Constants.hpp>
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>
#include "il2cpp-appdata.h"
#include <nlohmann/json.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <logging.hpp>

using json = nlohmann::json;

namespace Hooks::NpcDataManager
{
	std::vector<int> MarriagableCandidates {};

	uint64_t Tramp_NpcDataManager_SetNpcStatusData;
	NOINLINE void __cdecl Hook_NpcDataManager_SetNpcStatusData(app::NpcDataManager* __this, MethodInfo* method)
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

	void LoadMarriagableCandidates()
	{
		auto dir = std::filesystem::absolute(std::format("{}/MarriagableCandidates", Constants::PLUGIN_NAME));
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
		{
			if (dirEntry.is_regular_file())
			{
				auto path = dirEntry.path();
				if (boost::iequals(path.extension().generic_string(), ".json"))
				{
					std::ifstream file(path);
					auto json = json::parse(file, nullptr, false);
					if (json.is_discarded())
					{
						LOG_ERROR("Invalid JSON: %s", path.generic_string());
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
	}

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);

		LoadMarriagableCandidates();

		auto detour_NpcDataManager_SetNpcStatusData = new PLH::x64Detour(
			reinterpret_cast<char*>(app::NpcDataManager_SetNpcStatusData),
			reinterpret_cast<char*>(Hook_NpcDataManager_SetNpcStatusData),
			&Tramp_NpcDataManager_SetNpcStatusData,
			dis
		);
		detour_NpcDataManager_SetNpcStatusData->hook();
	}
}