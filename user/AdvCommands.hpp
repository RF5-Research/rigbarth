#pragma once
#include <map>
#include <vector>

namespace AdvCommands
{
	typedef void (*ReadCommandCallback)(app::AdvMain*, app::NpcController*);

	extern std::map<int32_t, std::vector<int32_t>> Commands;
	extern std::map<int32_t, ReadCommandCallback> Callbacks;
	bool SubscribeCommand(int32_t cmdID, ReadCommandCallback functionPTR);
	void Initialize();
}