#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>
#include "Constants.hpp"
#include "Enums.hpp"
#include "il2cpp-appdata.h"
#include <boost/algorithm/string/predicate.hpp>
#include <thread>
#include <future>
#include <logging.hpp>

namespace Hooks::AdvScriptReader
{
	void LoadBaseScripts()
	{
		auto scripts = (*app::AdvScriptReader__TypeInfo)->static_fields->Scripts;
		auto dir = std::filesystem::absolute(std::format("{}/Base/AdvScripts", Constants::PLUGIN_NAME));
		if (std::filesystem::exists(dir))
		{
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
			{
				if (dirEntry.is_regular_file())
				{
					auto scriptPath = dirEntry.path();
					if (boost::iequals(scriptPath.extension().generic_string(), ".advScript"))
					{
						auto scriptName = scriptPath.stem().generic_string();

						app::AdvScriptId__Enum result {};
						auto task = std::async(
							std::launch::async,
							app::Enum_TryParse_1,
							reinterpret_cast<app::String*>(il2cpp_string_new(scriptName.c_str())), &result, *app::Enum_TryParse_1__MethodInfo
						);

						std::ifstream file(scriptPath, std::ios::binary);
						std::vector<uint8_t> bytes(
							(std::istreambuf_iterator<char>(file)),
							(std::istreambuf_iterator<char>()));

						//Won't work due to not being initialized yet
						//auto type = il2cpp_class_get_type((Il2CppClass*)*Byte__TypeInfo);
						//auto type_object = il2cpp_type_get_object(type);
						//Is there a better way of doing this...
						auto type = app::Type_GetType_2(reinterpret_cast<app::String*>(il2cpp_string_new("System.Byte, System.Runtime")), nullptr);
						auto array = reinterpret_cast<app::Byte__Array*>(app::Array_CreateInstance_1(type, static_cast<int32_t>(bytes.size()), nullptr));
						std::copy(bytes.begin(), bytes.end(), array->vector);

						task.get();
						scripts->fields._items->vector[static_cast<int32_t>(result)] = array;
						*scripts->fields._items->vector[static_cast<int32_t>(result)] = *array;
						LOG_INFO("Replaced base AdvScript {}", scriptName);
					}
				}
			}
		}
	}
	void LoadScripts()
	{
		std::jthread tLoadBaseScripts(LoadBaseScripts);

		auto scripts = (*app::AdvScriptReader__TypeInfo)->static_fields->Scripts;
		int32_t index = scripts->fields._size;
		auto add = reinterpret_cast<void (*)(app::List_1_System_Byte__1*, app::Byte__Array * item, MethodInfo*)>(scripts->klass->vtable.Add.methodPtr);
		auto add_MethodInfo = scripts->klass->vtable.Add.method;
		auto dir = std::filesystem::absolute(std::format("{}/AdvScripts", Constants::PLUGIN_NAME));
		if (std::filesystem::exists(dir))
		{
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
			{
				if (dirEntry.is_regular_file())
				{
					auto scriptPath = dirEntry.path();
					if (boost::iequals(scriptPath.extension().generic_string(), ".advScript"))
					{
						std::ifstream file(scriptPath, std::ios::binary);
						std::vector<uint8_t> bytes(
							(std::istreambuf_iterator<char>(file)),
							(std::istreambuf_iterator<char>()));

						//Won't work due to not being initialized yet
						//auto type = il2cpp_class_get_type((Il2CppClass*)*Byte__TypeInfo);
						//auto type_object = il2cpp_type_get_object(type);
						//Is there a better way of doing this...
						auto type = app::Type_GetType_2(reinterpret_cast<app::String*>(il2cpp_string_new("System.Byte, System.Runtime")), nullptr);
						auto array = reinterpret_cast<app::Byte__Array*>(app::Array_CreateInstance_1(type, static_cast<int32_t>(bytes.size()), nullptr));

						std::copy(bytes.begin(), bytes.end(), array->vector);
						add(scripts, array, const_cast<MethodInfo*>(add_MethodInfo));
						auto scriptName = scriptPath.stem().generic_string();
						std::transform(scriptName.begin(), scriptName.end(), scriptName.begin(), ::toupper);
						Enums::AdvScriptID.emplace(scriptName, index++);
					}
				}
			}
		}
		tLoadBaseScripts.join();
	}

	//EventScript_ScriptPlayer__Update
	uint64_t Tramp_AdvScriptReader_Init_d_4_MoveNext;
	NOINLINE bool __cdecl Hook_AdvScriptReader_Init_d_4_MoveNext(app::AdvScriptReader_Init_d_4* __this, MethodInfo* method)
	{
		auto result = PLH::FnCast(Tramp_AdvScriptReader_Init_d_4_MoveNext, Hook_AdvScriptReader_Init_d_4_MoveNext)(__this, method);
		if (!result)
			LoadScripts();
		return result;
	}

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);

		auto detour_AdvScriptReader_Init_d_4_MoveNext = new PLH::x64Detour(
			reinterpret_cast<char*>(app::AdvScriptReader_Init_d_4_MoveNext),
			reinterpret_cast<char*>(Hook_AdvScriptReader_Init_d_4_MoveNext),
			&Tramp_AdvScriptReader_Init_d_4_MoveNext,
			dis
		);
		detour_AdvScriptReader_Init_d_4_MoveNext->hook();

	}
}