#include <polyhook2/Detour/x64Detour.hpp>
#include <polyhook2/CapstoneDisassembler.hpp>
#include "AdvCommands.hpp"
#include "il2cpp-appdata.h"

namespace Hooks::AdvMain
{
	uint64_t Tramp_AdvMain_ReadCommand;
	NOINLINE app::AdvMain_WorkList__Enum __cdecl Hook_AdvMain_ReadCommand(app::AdvMain* __this, app::NpcController* npc, MethodInfo* method)
	{
		auto result = PLH::FnCast(Tramp_AdvMain_ReadCommand, Hook_AdvMain_ReadCommand)(__this, npc, method);
		auto pos = AdvCommands::Callbacks.find(__this->fields.Cmd.CmdID);
		if (pos != AdvCommands::Callbacks.end())
		{
			__this->fields.nextWork = app::AdvMain_WorkList__Enum::WORK_READ_TEXT;
			printf("Invoking CMD: %d\n", pos->first);
			pos->second(__this, npc);
			return __this->fields.nextWork;
		}
		else
		{
			return result;
		}
	}

	void InstallHooks()
	{
		PLH::CapstoneDisassembler dis(PLH::Mode::x64);

		auto detour_AdvMain_ReadCommand = new PLH::x64Detour(
			reinterpret_cast<char*>(app::AdvMain_ReadCommand),
			reinterpret_cast<char*>(Hook_AdvMain_ReadCommand),
			&Tramp_AdvMain_ReadCommand,
			dis
		);
		detour_AdvMain_ReadCommand->hook();
	}
}