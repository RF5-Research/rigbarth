#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>
#include "il2cpp-appdata.h"
#include "helpers.h"
#include <Constants.hpp>
#include <thread>
#include "Enums.hpp"
#include <nlohmann/json.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <logging.hpp>

using json = nlohmann::json;

namespace Hooks::Enums
{
	std::map<std::string, int> AdvScriptID {};
	std::map<std::string, int> EventScriptID {};
	std::map<std::string, int> GameFlagData {};

	uint64_t Tramp_Enum_TryParse_AdvScriptID;
	NOINLINE bool __cdecl Hook_Enum_TryParse_AdvScriptID(app::String* value, int32_t* null, int32_t* result)
	{
		auto success = PLH::FnCast(Tramp_Enum_TryParse_AdvScriptID, Hook_Enum_TryParse_AdvScriptID)(value, null, result);
		auto enumString = il2cppi_to_string(value);

		if (!success)
		{
			auto pos = AdvScriptID.find(enumString);
			if (pos != AdvScriptID.end())
			{
				LOG_INFO("Found AdvScript: {}", enumString);
				*result = pos->second;
				return 1;
			}
			else
			{
				LOG_WARNING("Failed to find AdvScript: {}", enumString);
				return success;
			}
		}
		LOG_INFO("Loading base AdvScript: {}", enumString);
		return success;
	}

	uint64_t Tramp_Enum_TryParse_EventScriptID;
	NOINLINE bool __cdecl Hook_Enum_TryParse_EventScriptID(app::String* value, int32_t* null, int32_t* result)
	{
		auto success = PLH::FnCast(Tramp_Enum_TryParse_EventScriptID, Hook_Enum_TryParse_EventScriptID)(value, null, result);
		auto enumString = il2cppi_to_string(value);
		if (!success)
		{
			auto pos = EventScriptID.find(enumString);
			if (pos != EventScriptID.end())
			{
				LOG_INFO("Found EventScript: {}", enumString);
				*result = pos->second;
				return 1;
			}
			else
			{
				LOG_WARNING("Failed to find EventScript: {}", enumString);
				return success;
			}
		}
		LOG_INFO("Loading base EventScript: {}", enumString);
		return success;
	}

	uint64_t Tramp_EventScriptId__Enum__ToString;
	NOINLINE app::String* __cdecl Hook_EventScriptId__Enum__ToString(app::Enum__Boxed* __this, MethodInfo* method)
	{
		auto value = PLH::FnCast(Tramp_EventScriptId__Enum__ToString, Hook_EventScriptId__Enum__ToString)(__this, method);
		auto enumString = il2cppi_to_string(value);
		if (!std::any_of(enumString.begin(), enumString.end(), isalpha))
		{
			auto id = std::stoi(enumString);
			for (auto it = EventScriptID.begin(); it != EventScriptID.end(); ++it)
			{
				if (it->second == id)
				{
					LOG_INFO("Found EventScript {}", it->first);
					return reinterpret_cast<app::String*>(il2cpp_string_new(it->first.c_str()));
				}
			}
			LOG_WARNING("Failed to find EventScript {}", enumString);
		}
		//LOG_INFO("Loading base EventScript: {}", enumString);
		return value;
	}

	void LoadGameFlagData()
	{
		auto dir = std::filesystem::absolute(std::format("{}/GameFlagData", Constants::PLUGIN_NAME));
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
						LOG_ERROR("Invalid JSON: {}", path.generic_string());
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
									LOG_WARNING("GameFlagData {} contains duplicate ID: {}", key, id);
									isDuplicate = true;
									break;
								}
							}

							if (!isDuplicate)
							{
								GameFlagData.emplace(key, value);
							}
						}
					}
				}
			}
		}
	}

	void InstallHooks()
	{

		LoadGameFlagData();
		std::jthread([]() -> void
		{
			while (true)
			{
				if (il2cppi_is_initialized(app::Enum_TryParse_1__MethodInfo))
				{
					Tramp_Enum_TryParse_AdvScriptID = uint64_t((*app::Enum_TryParse_1__MethodInfo)->Il2CppVariant.rgctx_data->method->methodPointer);
					const_cast<MethodInfo*>((*app::Enum_TryParse_1__MethodInfo)->Il2CppVariant.rgctx_data->method)->methodPointer = reinterpret_cast<Il2CppMethodPointer>(&Hook_Enum_TryParse_AdvScriptID);
					break;
				}
			}
		}).detach();

		std::jthread( []() -> void
		{
			while (true)
			{
				if (il2cppi_is_initialized(app::EventScriptID__Enum__TypeInfo))
				{
					Tramp_EventScriptId__Enum__ToString = uint64_t((*app::EventScriptID__Enum__TypeInfo)->vtable.ToString.methodPtr);
					(*app::EventScriptID__Enum__TypeInfo)->vtable.ToString.methodPtr = reinterpret_cast<Il2CppMethodPointer>(&Hook_EventScriptId__Enum__ToString);
					break;
				}
			}
		}).detach();

		std::jthread( []() -> void
		{
			while (true)
			{
				if (il2cppi_is_initialized(app::Enum_TryParse_3__MethodInfo))
				{
					Tramp_Enum_TryParse_EventScriptID = uint64_t((*app::Enum_TryParse_3__MethodInfo)->Il2CppVariant.rgctx_data->method->methodPointer);
					const_cast<MethodInfo*>((*app::Enum_TryParse_3__MethodInfo)->Il2CppVariant.rgctx_data->method)->methodPointer = reinterpret_cast<Il2CppMethodPointer>(&Hook_Enum_TryParse_EventScriptID);
					break;
				}
			}
		}).detach();
	}
}