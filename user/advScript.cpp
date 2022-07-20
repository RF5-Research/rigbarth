#include "pch-il2cpp.h"
#include "il2cpp-appdata.h"
#include <iostream>
#include <format>
#include <filesystem>
#include <fstream>
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>
#include <map>

using namespace std;
using namespace app;
namespace advScript
{
	std::map<int, const char*> Scripts;

	void LoadScripts()
	{
		Scripts = std::map<int, const char*>();
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
						char test[] = { 0, 8, 8 };
						auto type = il2cpp_class_get_type((Il2CppClass*)*Byte__Array__TypeInfo);
						auto object = il2cpp_type_get_object(type);
						auto scriptName = scriptPath.generic_string();
						std::ifstream file(scriptName, std::ios::binary);
						std::vector<uint8_t> bytes(
							(std::istreambuf_iterator<char>(file)),
							(std::istreambuf_iterator<char>()));
						file.close();

						auto array = (Byte__Array*)Array_CreateInstance_1((Type*)object, bytes.size(), nullptr);
						Byte__Array* ted = nullptr;
						auto vector = (uint8_t*)&array->vector;
						vector = bytes.data();
						add(scripts, array, (MethodInfo*)add_MethodInfo);
						Scripts.insert(std::pair<int, const char*>(index, scriptPath.filename().generic_string().c_str()));
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

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);
		//TODO: Maybe use a macro instead
		auto detour_AdvScriptReader_Init_d_4_MoveNext = new PLH::x64Detour(
			(char*)AdvScriptReader_Init_d_4_MoveNext,
			(char*)Hook_AdvScriptReader_Init_d_4_MoveNext,
			&Tramp_AdvScriptReader_Init_d_4_MoveNext,
			dis
		);
		detour_AdvScriptReader_Init_d_4_MoveNext->hook();
	}
}