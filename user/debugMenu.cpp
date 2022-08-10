#include "pch-il2cpp.h"
#include "il2cpp-appdata.h"
#include "debugMenu.h"
#include "helpers.h"
#include <stdio.h>
#include "imgui_stdlib.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	if (ImGui::Button("SetEventFlag"))
	{
		auto eventControllerBase = app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance(*app::SingletonMonoBehaviour_1_EventControllerBase__get_Instance__MethodInfo);
		auto flagManager = eventControllerBase->fields._FlagManager;
		printf("Setting Event Flag\n");
		app::EventFlagManager_SetEventFlagFromScriptDebug(flagManager, (app::String*)il2cpp_string_new(test.c_str()), nullptr);
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


void InstallHooks()
{
	//Credits: https://github.com/rdbo/ImGui-DirectX-11-Kiero-Hook
	//Note: As of writing, >1.86 imgui is broken
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
	{
		kiero::bind(8, (void**)&oPresent, hkPresent);
	}
}
