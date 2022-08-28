#pragma once
#include <map>
#include <string>

namespace Hooks::Enums
{
	extern std::map<std::string, int> AdvScriptID;
	extern std::map<std::string, int> EventScriptID;
	extern std::map<std::string, int> GameFlagData;

	void InstallHooks();
}