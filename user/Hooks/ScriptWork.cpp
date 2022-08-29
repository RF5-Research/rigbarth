#include <AdvCommands.hpp>
#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>
#include "il2cpp-appdata.h"
#include <logging.hpp>

namespace Hooks::ScriptWork
{
	uint64_t Tramp_ScriptWork_ReadNext;
	NOINLINE app::stCommand_t __cdecl Hook_ScriptWork_ReadNext(app::ScriptWork* __this, MethodInfo* method)
	{
		if (__this->fields.commandIndex >= __this->fields.commandNum)
		{
		}
		else
		{
			auto addr = __this->fields.Addr;
			auto m_TextTableWork = __this->fields.m_TextTableWork;
			auto vector = m_TextTableWork->vector;
			int16_t cmdID = m_TextTableWork->max_length > addr + 1 ? std::byteswap(int16_t(vector[addr] << 8 | vector[addr + 1])) : 0;
			auto pos = AdvCommands::Commands.find(cmdID);
			if (pos != AdvCommands::Commands.end())
			{
				LOG_INFO("Found CMD ID: {}", pos->first);
				__this->fields.Addr += 2;
				auto intType = app::Type_GetType_2(reinterpret_cast<app::String*>(il2cpp_string_new("System.Int32, System.Runtime")), nullptr);
				auto params = reinterpret_cast<app::Int32__Array*>(app::Array_CreateInstance_1(intType, static_cast<int32_t>(pos->second.size()), nullptr));
				//auto stringType = Type_GetType_2(reinterpret_cast<String*>(il2cpp_string_new("System.String, System.Runtime")), nullptr);
				//auto text = (Int32__Array*)Array_CreateInstance_1(type, static_cast<int32_t>(0), nullptr);

				for (int i = 0; i < pos->second.size(); i++)
				{
					switch (pos->second[i])
					{
					case -1:
					{
						break;
					}
					case 4:
					{
						auto param = app::ScriptWork_GetTextTableInt(__this, __this->fields.Addr, 4, nullptr);
						__this->fields.Addr += 4;
						params->vector[i] = param;
						break;
					}
					}
				}
				__this->fields.commandIndex += 1;
				*__this->fields.command.Arg = *params;
				__this->fields.command.CmdID = cmdID;
				return __this->fields.command;
			}
			else
			{
				return PLH::FnCast(Tramp_ScriptWork_ReadNext, Hook_ScriptWork_ReadNext)(__this, method);
			}
		}
		return PLH::FnCast(Tramp_ScriptWork_ReadNext, Hook_ScriptWork_ReadNext)(__this, method);
	}

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);

		auto detour_ScriptWork_ReadNext = new PLH::x64Detour(
			reinterpret_cast<char*>(app::ScriptWork_ReadNext),
			reinterpret_cast<char*>(Hook_ScriptWork_ReadNext),
			&Tramp_ScriptWork_ReadNext,
			dis
		);
		detour_ScriptWork_ReadNext->hook();
	}
}