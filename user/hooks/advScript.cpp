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
#include <thread>
#include <cstring>

using namespace app;

namespace advScript
{
	std::map<std::string, int> Scripts;
	const char* scriptPath = "rigbarth/AdvScripts";
	void LoadScripts()
	{
		Scripts = std::map<std::string, int>();
		auto scripts = (*AdvScriptReader__TypeInfo)->static_fields->Scripts;
		int32_t index = scripts->fields._size;
		auto add = (int32_t (*)(List_1_System_Byte__1*, Byte__Array * item, MethodInfo*))scripts->klass->vtable.Add.methodPtr;
		auto add_MethodInfo = scripts->klass->vtable.Add.method;
		auto dir = std::filesystem::absolute(scriptPath);
		if (std::filesystem::exists(dir))
		{
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator::recursive_directory_iterator(dir))
			{
				if (dirEntry.is_regular_file())
				{
					auto scriptPath = dirEntry.path();
					if (utils::string::iequals(scriptPath.extension().generic_string(), ".advScript"))
					{
						std::ifstream file(scriptPath.generic_string(), std::ios::binary);
						std::vector<uint8_t> bytes(
							(std::istreambuf_iterator<char>(file)),
							(std::istreambuf_iterator<char>()));
						file.close();

						//Won't work due to not being initialized yet
						//auto type = il2cpp_class_get_type((Il2CppClass*)*Byte__TypeInfo);
						//auto type_object = il2cpp_type_get_object(type);
						//Is there a better way of doing this...
						auto type = Type_GetType_2(reinterpret_cast<String*>(il2cpp_string_new("System.Byte, System.Runtime")), nullptr);
						auto array = (Byte__Array*)Array_CreateInstance_1(type, static_cast<int32_t>(bytes.size()), nullptr);
						
						std::copy(bytes.begin(), bytes.end(), array->vector);
						add(scripts, array, (MethodInfo*)add_MethodInfo);
						auto scriptName = scriptPath.stem().generic_string();
						std::transform(scriptName.begin(), scriptName.end(), scriptName.begin(), ::toupper);
						Scripts.insert(std::pair<std::string, int>(scriptName, index++));
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
	NOINLINE bool __cdecl Hook_Enum_TryParse(String* value, int32_t* null, int32_t* result)
	{
		auto success = PLH::FnCast(Tramp_Enum_TryParse, Hook_Enum_TryParse)(value, null, result);
		if (!success)
		{ 
			auto enumString = il2cppi_to_string(value);
			auto pos = Scripts.find(enumString);
			if (pos != Scripts.end())
			{
				printf("Loading AdvScript: %s\n", enumString.c_str());
				*result = pos->second;
				return 1;
			}
		}
		return success;
	}

	//Put this in its own class

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);

		auto detour_AdvScriptReader_Init_d_4_MoveNext = new PLH::x64Detour(
			reinterpret_cast<char*>(AdvScriptReader_Init_d_4_MoveNext),
			reinterpret_cast<char*>(Hook_AdvScriptReader_Init_d_4_MoveNext),
			&Tramp_AdvScriptReader_Init_d_4_MoveNext,
			dis
		);
		detour_AdvScriptReader_Init_d_4_MoveNext->hook();

		//Don't hook generic method
		//Unfortunately, the methodinfo isn't lazy init yet
		std::thread hook([]() -> void
			{
				while (true)
				{
					if (il2cppi_is_initialized(Enum_TryParse_1__MethodInfo) && (*Enum_TryParse_1__MethodInfo)->Il2CppVariant.rgctx_data->method->methodPointer)
					{
						Tramp_Enum_TryParse = uint64_t((*Enum_TryParse_1__MethodInfo)->Il2CppVariant.rgctx_data->method->methodPointer);
						const_cast<MethodInfo*>((*Enum_TryParse_1__MethodInfo)->Il2CppVariant.rgctx_data->method)->methodPointer = reinterpret_cast<Il2CppMethodPointer >(&Hook_Enum_TryParse);
						break;
					}
				}
			});
		hook.join();
	}
}