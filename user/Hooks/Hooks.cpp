#include "AdvMain.hpp"
#include "AdvScriptReader.hpp"
#include "ConditionsForLoverJudgment.hpp"
#include "Enums.hpp"
#include "EventFlagManager.hpp"
#include "NpcDataManager.hpp"
#include "ScriptWork.hpp"

namespace Hooks
{
	void InstallHooks()
	{
		AdvMain::InstallHooks();
		AdvScriptReader::InstallHooks();
		ConditionsForLoverJudgment::InstallHooks();
		Enums::InstallHooks();
		EventFlagManager::InstallHooks();
		NpcDataManager::InstallHooks();
		ScriptWork::InstallHooks();
	}
}
