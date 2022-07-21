#include "pch-il2cpp.h"
#include "helpers.h"
#include "il2cpp-appdata.h"
#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>
#include <map>
#include <thread>


using namespace std;
using namespace app;
namespace advScript
{
	std::map<const char*, int> Scripts;

	void LoadScripts()
	{
		Scripts = std::map<const char*, int>();
		auto scripts = (*AdvScriptReader__TypeInfo)->static_fields->Scripts;
		int32_t index;
		auto add = (int32_t (*)(List_1_System_Byte__1*, Byte__Array * item, MethodInfo*))scripts->klass->vtable.Add.methodPtr;
		auto add_MethodInfo = scripts->klass->vtable.Add.method;
		auto dir = std::format("{}/rigbarth/AdvScripts", filesystem::current_path().generic_string().c_str());
		if (filesystem::exists(dir))
		{
			for (const auto& dirEntry : filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
			{
				if (dirEntry.is_regular_file())
				{
					auto scriptPath = dirEntry.path();
					if (scriptPath.extension().compare("advScript"))
					{
						std::ifstream file(scriptPath.generic_string(), std::ios::binary);
						std::vector<uint8_t> bytes(
							(std::istreambuf_iterator<char>(file)),
							(std::istreambuf_iterator<char>()));
						file.close();

						auto type = il2cpp_class_get_type((Il2CppClass*)*Byte__Array__TypeInfo);
						auto type_object = il2cpp_type_get_object(type);
						auto array = (Byte__Array*)Array_CreateInstance_1((Type*)type_object, bytes.size(), nullptr);
						auto vector = (uint8_t*)&array->vector;
						vector = bytes.data();
						add(scripts, array, (MethodInfo*)add_MethodInfo);
						auto scriptName = scriptPath.filename().generic_string();
						std::transform(scriptName.begin(), scriptName.end(), scriptName.begin(), ::toupper);
						Scripts.insert(std::pair<const char*, int>(scriptName.c_str(), index));
					}
				}
			}
		}
	}

	uint64_t Tramp_AdvScriptReader_Init_d_4_MoveNext;
	NOINLINE bool __cdecl Hook_AdvScriptReader_Init_d_4_MoveNext(AdvScriptReader_Init_d_4* __this, MethodInfo* method)
	{
		auto result = PLH::FnCast(Tramp_AdvScriptReader_Init_d_4_MoveNext, Hook_AdvScriptReader_Init_d_4_MoveNext)(__this, method);
		if (!result)
			LoadScripts();
		return result;
	}

	uint64_t Tramp_Enum_TryParse;
	NOINLINE bool __cdecl Hook_Enum_TryParse(String* value, int32_t* result, MethodInfo* method)
	{
		auto success = PLH::FnCast(Tramp_Enum_TryParse, Hook_Enum_TryParse)(value, result, method);
		if (il2cppi_is_initialized(Enum_TryParse_1__MethodInfo) && *Enum_TryParse_1__MethodInfo == method)
		{
			if (!success)
			{ 
				auto enumString = il2cppi_to_string(value).c_str();
				printf("%s", il2cppi_to_string(value).c_str());
				result = new int32_t(Scripts[enumString]);
			}
			else
				printf("%s", il2cppi_to_string(value).c_str());
		}
		return success;
	}

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);

		auto detour_AdvScriptReader_Init_d_4_MoveNext = new PLH::x64Detour(
			(char*)AdvScriptReader_Init_d_4_MoveNext,
			(char*)Hook_AdvScriptReader_Init_d_4_MoveNext,
			&Tramp_AdvScriptReader_Init_d_4_MoveNext,
			dis
		);
		detour_AdvScriptReader_Init_d_4_MoveNext->hook();

		//TODO: Don't hook generic method
		auto detour_Enum_TryParse = new PLH::x64Detour(
			(char*)Enum_TryParse,
			(char*)Hook_Enum_TryParse,
			&Tramp_Enum_TryParse,
			dis
		);
		detour_Enum_TryParse->hook();

	}
}