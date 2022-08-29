#include "AdvCommands.hpp"
#include "il2cpp-appdata.h"
#include "helpers.h"
#include "AdvCommands.hpp"

namespace RigbarthCommands
{
	void IfTimeZoneGotoStepNext(app::AdvMain* self, app::NpcController* npc)
	{
		auto args = self->fields.Cmd.Arg;
		auto timeManager = app::SingletonMonoBehaviour_1_TimeManager__get_Instance(*app::SingletonMonoBehaviour_1_TimeManager__get_Instance__MethodInfo);
		auto time = app::TimeManager_get_TimeZone(timeManager, nullptr);
		if (time == static_cast<app::TimeZone__Enum>(args->vector[0]))
		{
			auto eventControllerBase = app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance(*app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance__MethodInfo);
			app::EventControllerBase_SetNextNpcEventStep_1(eventControllerBase, args->vector[1], nullptr);
			self->fields.scriptwork->fields.commandIndex = self->fields.scriptwork->fields.commandNum;
		}
	}

	void IfNPCLifecycleStateGotoStepNext(app::AdvMain* self, app::NpcController* npc)
	{

		auto args = self->fields.Cmd.Arg;
		auto npcDataManager = app::SingletonMonoBehaviour_1_NpcDataManager__get_Instance(*app::SingletonMonoBehaviour_1_NpcDataManager__get_Instance__MethodInfo);
		auto npcData = app::NpcDataManager_GetNpcData_1(npcDataManager, static_cast<app::NPCID__Enum>(args->vector[0]), nullptr);
		auto state = app::NpcData_get_CurrentLifecycleState(npcData, nullptr);
		
		if (state == static_cast<app::LifecycleState__Enum>(args->vector[1]))
		{
			auto eventControllerBase = app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance(*app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance__MethodInfo);
			app::EventControllerBase_SetNextNpcEventStep_1(eventControllerBase, args->vector[2], nullptr);
			self->fields.scriptwork->fields.commandIndex = self->fields.scriptwork->fields.commandNum;
		}
	}

	/*CurrentLifecycleState*/
	//npcdata.StandbyAnimState Is Sleeping
	//Morning and Night dialogue
	//CurrentLifecycleState
	void IfInNPCFOVGotoStepNext(app::AdvMain* self, app::NpcController* npc)
	{
		auto args = self->fields.Cmd.Arg;
		auto npcDataManager = app::SingletonMonoBehaviour_1_NpcDataManager__get_Instance(*app::SingletonMonoBehaviour_1_NpcDataManager__get_Instance__MethodInfo);
		auto npcData = app::NpcDataManager_GetNpcData_1(npcDataManager, static_cast<app::NPCID__Enum>(args->vector[0]), nullptr);

		auto actor = app::ActorManager_GetActor(npcData->fields.actorId, nullptr);
		auto actorController = actor->fields._ActiveController_k__BackingField;
		auto actorModel = app::HumanController_get_HumanModel(reinterpret_cast<app::HumanController*>(actorController), nullptr);
		auto actorLookPoint = app::HumanModel_get_LookPoint(actorModel, nullptr);
		auto actorPos = app::Transform_get_position(actorLookPoint, nullptr);

		bool canSeePlayer = false;
		//https://github.com/Comp3interactive/FieldOfView
		if (*app::LayerMaskDefine_Mask__TypeInfo)
		{
			auto layer = (*app::LayerMaskDefine_Mask__TypeInfo)->static_fields->CharacterBase;
			auto colliders = app::Physics_OverlapSphere(actorPos, 10, layer.m_Mask, app::QueryTriggerInteraction__Enum::UseGlobal, nullptr);
			for (int i = 0; i < colliders->max_length; i++)
			{
				auto collider = colliders->vector[i];
				auto tag = il2cppi_to_string(app::Component_1_get_tag(reinterpret_cast<app::Component_1*>(collider), nullptr));
				if (tag == "Player")
				{
					auto transform = app::Component_1_get_transform(reinterpret_cast<app::Component_1*>(collider), nullptr);
					auto pos = app::Transform_get_position(transform, nullptr);
					auto wallLayer = (*app::LayerMaskDefine_Mask__TypeInfo)->static_fields->Wall;

					app::Vector3 dir =
					{
						pos.x - actorPos.x,
						pos.y - actorPos.y,
						pos.z - actorPos.z,
					};
					canSeePlayer = !app::Physics_Raycast_1(actorPos, dir, app::Vector3_Distance(actorPos, pos, nullptr), wallLayer.m_Mask, nullptr);
					//if (canSeePlayer)
					//{
					//	printf("Can see player\n");
					//}
					//else
					//{
					//	printf("Cannot see player\n");
					//}
				}
			}
		}

		if (canSeePlayer)
		{
			auto eventControllerBase = app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance(*app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance__MethodInfo);
			app::EventControllerBase_SetNextNpcEventStep_1(eventControllerBase, self->fields.Cmd.Arg->vector[1], nullptr);
			self->fields.scriptwork->fields.commandIndex = self->fields.scriptwork->fields.commandNum;
		}
	}

	void Subscribe()
	{
		AdvCommands::SubscribeCommand(600, &IfInNPCFOVGotoStepNext);
		AdvCommands::SubscribeCommand(601, &IfNPCLifecycleStateGotoStepNext);
		AdvCommands::SubscribeCommand(602, &IfTimeZoneGotoStepNext);


	}
}