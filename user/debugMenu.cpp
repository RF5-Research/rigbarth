#include <dxgi.h>
#include <stdio.h>

#include "kiero.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_stdlib.h"
#include "il2cpp-appdata.h"
#include "helpers.h"
#include "DebugMenu.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace DebugMenu
{

	Present oPresent;
	HWND window = NULL;
	WNDPROC oWndProc;
	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	ID3D11RenderTargetView* mainRenderTargetView;

	void InitImGui()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(pDevice, pContext);
	}

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}

	bool init = false;
	std::string test;
	HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (!init)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
			{
				pDevice->GetImmediateContext(&pContext);
				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				window = sd.OutputWindow;
				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
				pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
				pBackBuffer->Release();
				oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
				InitImGui();
				init = true;
			}
			else
				return oPresent(pSwapChain, SyncInterval, Flags);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//GameMain__InitializeAfterSceneLoaded
		//static int count;
		//ImGui::InputInt("Increment", &count);
		//if (ImGui::Button("Increase furniture cap; Click when save is loaded."))
		//{
		//	auto save = app::SaveDataManager_get_GameSaveData(nullptr);
		//	auto add = save->fields.FurnitureData->fields.FurnitureSaveData->klass->vtable.Add;
		//	auto addMethod = add.method;
		//	auto addPtr = reinterpret_cast<void (*)(app::List_1_ItemFurniture_FurnitureSaveData_*, app::FurnitureSaveData * item, MethodInfo*)>(add.methodPtr);
		//	printf("%d\n", save->fields.FurnitureData->fields.FurnitureSaveData->fields._size);
		//	for (int i = 0; i < count; i++)
		//	{
		//		auto obj = (app::FurnitureSaveData*)il2cpp_object_new((const Il2CppClass*)(*app::FurnitureSaveData__TypeInfo));
		//		app::FurnitureSaveData__ctor(obj, 1, nullptr);
		//		addPtr(save->fields.FurnitureData->fields.FurnitureSaveData, obj, (MethodInfo*)addMethod);
		//	}
		//	printf("%d\n", save->fields.FurnitureData->fields.FurnitureSaveData->fields._size);
		//}
		static bool on;
		ImGui::Checkbox("RayCast", &on);
		if (on)
		{
			auto npcDataManager = app::SingletonMonoBehaviour_1_NpcDataManager__get_Instance(*app::SingletonMonoBehaviour_1_NpcDataManager__get_Instance__MethodInfo);
			auto npcData = app::NpcDataManager_GetNpcData_1(npcDataManager, app::NPCID__Enum::Elsje, nullptr);

			auto actor = app::ActorManager_GetActor(npcData->fields.actorId, nullptr);
			auto actorController = actor->fields._ActiveController_k__BackingField;
			auto actorModel = app::HumanController_get_HumanModel((app::HumanController*)actorController, nullptr);
			auto actorLookPoint = app::HumanModel_get_LookPoint(actorModel, nullptr);
			auto actorPos = app::Transform_get_position(actorLookPoint, nullptr);

			//https://github.com/Comp3interactive/FieldOfView
			if (*app::LayerMaskDefine_Mask__TypeInfo)
			{
				//Character
				auto layer = (*app::LayerMaskDefine_Mask__TypeInfo)->static_fields->CharacterBase;
				auto colliders = app::Physics_OverlapSphere(actorPos, 10, layer.m_Mask, app::QueryTriggerInteraction__Enum::UseGlobal, nullptr);
				for (int i = 0; i < colliders->max_length; i++)
				{
					auto collider = colliders->vector[i];
					auto tag = il2cppi_to_string(app::Component_1_get_tag((app::Component_1*)collider, nullptr));
					if (tag == "Player")
					{
						auto transform = app::Component_1_get_transform((app::Component_1*)collider, nullptr);
						auto pos = app::Transform_get_position(transform, nullptr);
						auto wallLayer = (*app::LayerMaskDefine_Mask__TypeInfo)->static_fields->Wall;

						app::Vector3 dir =
						{
							pos.x - actorPos.x,
							pos.y - actorPos.y,
							pos.z - actorPos.z,
						};
						//Do a pillar raycast on the interaction and search for NpcPatrolPoint
						//Get the collider's tag
						//Find Npc that has that patrolpoint
						//Get NpcData's bedpatrolPoint
						// 
						//NpcPatrolPoint
						//ObjectInteractionController.DoInteraction
						//In RunScript -> obj{0:000}_{1}_{2:000}
						//Param 0: 1
						//Param 1: Enum
						//Param 2: Enum Step (always starts at 0
						for (int ii = 0; ii < npcData->fields.BedPatrolPoint->fields.NpcPatrolRoutes->max_length; ii++)
						{
							//auto x = il2cppi_to_string(npcData->fields.BedPatrolPoint->fields.NpcPatrolRoutes->vector[ii]->fields.TargetPoint);
							//printf("TargetPoint: %s\n", x.c_str());
							//auto y = il2cppi_to_string(npcData->fields.BedPatrolPoint->fields.);
							//printf("TargetSeat: %s\n", y.c_str());

						}
						//if (app::Physics_Raycast_1(actorPos, dir, app::Vector3_Distance(actorPos, pos, nullptr), wallLayer.m_Mask, nullptr))
						//{
						//	printf("Cannot see player\n");
						//}
						//else
						//{
						//	printf("Can see player\n");
						//}
					}
				}
			}
		}

		static int eventID;
		ImGui::InputInt("Event ID", &eventID);
		if (ImGui::Button("Enable Event Script") && eventID)
		{
			auto eventControllerBase = app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance(*app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance__MethodInfo);
			auto flagManager = eventControllerBase->fields._FlagManager;
			auto datas = flagManager->fields.EventUnlockFlagDatas;
			auto vec = datas->fields.datas->fields._items->vector;

			for (int i = 0; i < datas->fields.datas->fields._size; i++)
			{
				if (static_cast<int>(vec[i]->fields.ScriptId) == eventID)
				{
					printf("Enabling Event Script\n");
					auto flagManager = eventControllerBase->fields._FlagManager;
					app::EventFlagManager_RegistActiveEventPointList(flagManager, vec[i], nullptr);
					app::EventFlagManager_EnableEventPoint(flagManager, vec[i], nullptr);
					break;
				}
			}
		}

		ImGui::InputText("Event Script", &test);
		if (ImGui::Button("Force Script"))
		{
			auto eventControllerBase = app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance(*app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance__MethodInfo);
			printf("SubEventDatas: %d\n", eventControllerBase->fields.subEventManager->fields.SubEventDatas->fields._size);
			printf("OccurrenceSubEventDatas: %d\n", eventControllerBase->fields.subEventManager->fields.OccurrenceSubEventDatas->fields._size);
			//auto eventControllerBase = app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance(*app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance__MethodInfo);
			//app::EventControllerBase_set_ForceScriptName(eventControllerBase, (app::String*)il2cpp_string_new(test.c_str()), nullptr);
			//app::EventControllerBase_set_ForceEvent(eventControllerBase, true, nullptr);
		}
		//ImGui::InputText("Monster", &mobName);
		//if (ImGui::Button("Spawn Monster"))
		//{
		//	//MOCO_1
		//	printf("Adding Mob\n");
		//	auto tutorialManager = (app::TutorialManager*)app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance(*app::SingletonMonoBehaviour_1_Tutorial_TutorialManager___ctor__MethodInfo);

		//	app::TutorialManager_MonsterCreate(tutorialManager, (app::String*)il2cpp_string_new(mobName.c_str()), -40.18685, -10.044149, 41.36737, nullptr);
		//}

		//static int eventID;
		//ImGui::InputInt("Event ID", &eventID);
		//if (ImGui::Button("Added") && eventID)
		//{
		//	printf("Adding Event\n");
		//	auto eventControllerBase = app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance(*app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance__MethodInfo);
		//	app::SubEventManager_DEBUG_AddOccurrenceSubEvent(eventControllerBase->fields.subEventManager, eventID, nullptr);
		//}

		ImGui::End();

		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		return oPresent(pSwapChain, SyncInterval, Flags);
	}


	void DebugMenu::Initialize()
	{
		//Credits: https://github.com/rdbo/ImGui-DirectX-11-Kiero-Hook
		//Note: As of writing, >1.86 imgui is broken
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
		}
	}
}