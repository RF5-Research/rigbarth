// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Target Unity version: 2019.4.15 - 2019.4.18

// ******************************************************************************
// * IL2CPP application-specific method definition addresses and signatures
// ******************************************************************************

using namespace app;

DO_APP_FUNC(0x01367870, bool, AdvScriptReader_Init_d_4_MoveNext, (AdvScriptReader_Init_d_4* __this, MethodInfo* method));
DO_APP_FUNC(0x00873DB0, Array*, Array_CreateInstance_1, (Type* elementType, int32_t length, MethodInfo* method));
DO_APP_FUNC_METHODINFO(0x0302B2C0, Enum_TryParse_1__MethodInfo);
DO_APP_FUNC(0x00EA4020, bool, ConditionsForLoverJudgment_CheckConditionsForLoverJudgment, (int32_t npcid, int32_t lovelv, int32_t eventType, int32_t eventStep1, int32_t eventStep2, MethodInfo* method));
DO_APP_FUNC(0x014BB1E0, bool, Enum_TryParse_1, (String* value, AdvScriptId__Enum* result, MethodInfo* method));
DO_APP_FUNC(0x0134EE20, Byte__Array*, AdvScriptReader_GetScript, (AdvScriptId__Enum id, MethodInfo* method));
DO_APP_FUNC(0x0104CC40, void, AdvMain_AdvPreLoadStart, (AdvMain* __this, Byte__Array* textobj, String* filePath, bool research, MethodInfo* method));
DO_APP_FUNC(0x0104DCA0, bool, AdvMain_AdvStart, (AdvMain* __this, String* filePath, NpcController* npc, bool research, MethodInfo* method));
DO_APP_FUNC(0x0104DBF0, IEnumerator*, AdvMain_AdvStartWaitResult, (AdvMain* __this, AdvScriptId__Enum advindex, String* filePath, bool research, MethodInfo* method));
DO_APP_FUNC(0x01367610, bool, AdvMain_AdvStartWaitResult_d_69_MoveNext, (AdvMain_AdvStartWaitResult_d_69* __this, MethodInfo* method));
DO_APP_FUNC(0x0132D810, void, LifeCycleAdvMain_AdvStart2, (LifeCycleAdvMain* __this, TextAsset* textobj, String* filePath, MethodInfo* method));
DO_APP_FUNC(0x00D41410, bool, EventControllerBase_NpcEventNext, (EventControllerBase* __this, bool research, MethodInfo* method));
DO_APP_FUNC(0x00D371C0, int32_t, EventControllerBase_EventStepProceedReserve, (EventControllerBase* __this, int32_t orderId, MethodInfo* method));
DO_APP_FUNC(0x00D38BE0, bool, EventControllerBase_ForceScriptEventStart, (EventControllerBase* __this, MethodInfo* method));
DO_APP_FUNC(0x012EF030, RF5SaveData*, SaveDataManager_get_GameSaveData, (MethodInfo* method));
DO_APP_FUNC(0x00D49660, void, EventControllerBase_RunScript, (EventControllerBase* __this, NpcEventType__Enum NowType, MethodInfo* method));
DO_APP_FUNC(0x00D47830, void, EventControllerBase_RunKokuhakuScript, (EventControllerBase* __this, int32_t npcid, MethodInfo* method));

DO_APP_FUNC(0x00D52B30, void, EventControllerBase_SetCurrentNpcEventStep_1, (EventControllerBase* __this, int32_t currentStep, MethodInfo* method));
DO_APP_FUNC(0x00D53070, void, EventControllerBase_SetNextNpcEventStep, (EventControllerBase* __this, int32_t npcId, int32_t nextStep, MethodInfo* method));
DO_APP_FUNC(0x00D53140, void, EventControllerBase_SetNextNpcEventStep_1, (EventControllerBase* __this, int32_t nextStep, MethodInfo* method));
DO_APP_FUNC(0x011DEBD0, void, EventData_SetNpcEventPath, (EventData* __this, String* path, MethodInfo* method));
DO_APP_FUNC(0x00D36F30, void, EventControllerBase_EventStart, (EventControllerBase* __this, String* fileName, bool research, MethodInfo* method));
DO_APP_FUNC(0x00D38AD0, bool, EventControllerBase_ForceEventStart, (EventControllerBase* __this, String* scriptName, MethodInfo* method));
DO_APP_FUNC(0x0089C1F0, Type*, Object_GetType, (Object* __this, MethodInfo* method));
DO_APP_FUNC(0x009C1F00, Type*, Type_GetType_2, (String* typeName, MethodInfo* method));
DO_APP_FUNC_METHODINFO(0x0302B200, Enum_TryParse_3__MethodInfo);
DO_APP_FUNC(0x0057BB40, String*, Enum_ToString, (Enum__Boxed* __this, MethodInfo* method));
DO_APP_FUNC(0x00578090, String*, Enum_GetName, (Type* enumType, Object* value, MethodInfo* method));
DO_APP_FUNC(0x003B8000, void, EventUnlockFlagData__ctor, (EventUnlockFlagData* __this, MethodInfo* method));
DO_APP_FUNC(0x011EBE20, EventUnlockFlagData*, EventFlagManager_GetNextEventUnlockData, (EventFlagManager* __this, EventScriptID__Enum scriptId, MethodInfo* method));
DO_APP_FUNC(0x00D28440, Object*, JsonUtility_FromJson, (String* json, Type* type, MethodInfo* method));
DO_APP_FUNC_METHODINFO(0x03047108, JsonUtility_FromJson__MethodInfo);
DO_APP_FUNC_METHODINFO(0x0301F0E8, SingletonMonoBehaviour_1_EventControllerBase__get_Instance__MethodInfo);
DO_APP_FUNC(0x01B59BE0, EventControllerBase*, SingletonMonoBehaviour_1_EventControllerBase__get_Instance, (MethodInfo* method));
DO_APP_FUNC(0x01B59BE0, NpcDataManager*, SingletonMonoBehaviour_1_NpcDataManager__get_Instance, (MethodInfo* method));
DO_APP_FUNC(0x00D56200, EventFlagManager*, EventControllerBase_get_FlagManager, (EventControllerBase* __this, MethodInfo* method));
DO_APP_FUNC(0x011EE110, void, EventFlagManager_Start, (EventFlagManager* __this, MethodInfo* method));
DO_APP_FUNC(0x0057BDB0, bool, Enum_TryParseEnum, (Type* enumType, String* value, bool ignoreCase, Enum_EnumResult* parseResult, MethodInfo* method));
DO_APP_FUNC(0x01349E30, void, SubEventManager_DEBUG_AddOccurrenceSubEvent, (SubEventManager* __this, int32_t eventid, MethodInfo* method));
DO_APP_FUNC(0x00E027F0, void, TutorialManager_MonsterCreate, (TutorialManager* __this, String* monster, float x, float y, float z, MethodInfo* method));
DO_APP_FUNC_METHODINFO(0x030046E8, SingletonMonoBehaviour_1_Tutorial_TutorialManager___ctor__MethodInfo);
DO_APP_FUNC(0x01508D90, void, NpcDataManager_SetNpcStatusData, (NpcDataManager* __this, MethodInfo* method));
DO_APP_FUNC(0x011EC160, void, EventFlagManager_InitEventStartPoints, (EventFlagManager * __this, MethodInfo * method));
DO_APP_FUNC(0x011EA6B0, void, EventFlagManager_EnableEventPoint, (EventFlagManager* __this, EventUnlockFlagData* unlockData, MethodInfo* method));
DO_APP_FUNC(0x011ECBC0, void, EventFlagManager_RegistActiveEventPointList, (EventFlagManager* __this, EventUnlockFlagData* unlockData, MethodInfo* method));
DO_APP_FUNC(0x00EA3FB0, bool, ConditionsForLoverJudgment_CheckCanbeLoverNPCID, (NPCID__Enum npcid, MethodInfo* method));
DO_APP_FUNC(0x011EDC10, void, EventFlagManager_SetEventFlagFromScriptDebug, (EventFlagManager* __this, String* scriptName, MethodInfo* method));
DO_APP_FUNC_METHODINFO(0x0301E6B8, SingletonMonoBehaviour_1_NpcDataManager__get_Instance__MethodInfo);
DO_APP_FUNC(0x01506FF0, NpcData*, NpcDataManager_GetNpcData_1, (NpcDataManager* __this, NPCID__Enum NpcId, MethodInfo* method));
DO_APP_FUNC(0x011EA770, void, EventFlagManager_EventFlagOnOff, (EventFlagManager* __this, String* scriptName, MethodInfo* method));
DO_APP_FUNC(0x00D58710, void, EventControllerBase_set_ForceScriptName, (EventControllerBase* __this, String* value, MethodInfo* method));
DO_APP_FUNC(0x00D586D0, void, EventControllerBase_set_ForceEvent, (EventControllerBase* __this, bool value, MethodInfo* method));
DO_APP_FUNC(0x0134D030, void, SubEventManager__ctor, (SubEventManager* __this, MethodInfo* method));
DO_APP_FUNC(0x00D26800, AssetBundleCreateRequest*, AssetBundle_LoadFromFileAsync, (String* path, MethodInfo* method));
DO_APP_FUNC(0x00D26360, AssetBundle*, AssetBundleCreateRequest_get_assetBundle, (AssetBundleCreateRequest* __this, MethodInfo* method));
DO_APP_FUNC(0x00D26530, AssetBundleRequest*, AssetBundle_LoadAssetAsync, (AssetBundle* __this, String* name, Type* type, MethodInfo* method));
DO_APP_FUNC(0x00D263E0, Object_1*, AssetBundleRequest_get_asset, (AssetBundleRequest* __this, MethodInfo* method));
DO_APP_FUNC(0x01A1F140, bool, AsyncOperation_get_isDone, (AsyncOperation* __this, MethodInfo* method));
DO_APP_FUNC(0x0143FC80, Sprite*, MapIconMng_getQuestIcon, (MapIconMng* __this, int32_t iconNo, MethodInfo* method));
DO_APP_FUNC(0x00F664D0, Transform*, ActorPlayer_get_Player, (MethodInfo* method));
DO_APP_FUNC(0x00F621B0, Actor*, ActorManager_GetActor, (ActorID__Enum actorId, MethodInfo* method));
DO_APP_FUNC(0x00F6C1C0, Transform*, Actor_get_Transform, (Actor* __this, MethodInfo* method));
DO_APP_FUNC(0x02116790, bool, Physics_Raycast_7, (Vector3 origin, Vector3 direction, RaycastHit* hitInfo, MethodInfo* method));
DO_APP_FUNC(0x00C78C40, Vector3, Transform_get_position, (Transform* __this, MethodInfo* method));
DO_APP_FUNC(0x00387FA0, Transform*, RaycastHit_get_transform, (RaycastHit__Boxed* __this, MethodInfo* method));
DO_APP_FUNC(0x00C7DA30, Vector3, Vector3_Normalize, (Vector3 value, MethodInfo* method));
DO_APP_FUNC(0x003F6170, ActorController*, Actor_get_ActiveController, (Actor* __this, MethodInfo* method));
DO_APP_FUNC(0x00F65CE0, ActorController*, ActorPlayer_get_ActiveController, (MethodInfo* method));
DO_APP_FUNC(0x02119FD0, Vector3, Rigidbody_get_position, (Rigidbody* __this, MethodInfo* method));
DO_APP_FUNC(0x003F6170, Transform*, CharacterModel_get_LookPoint, (CharacterModel* __this, MethodInfo* method));
DO_APP_FUNC(0x000D4430, float, RaycastHit_get_distance, (RaycastHit__Boxed* __this, MethodInfo* method));
DO_APP_FUNC(0x00C77C50, Vector3, Transform_TransformDirection, (Transform* __this, Vector3 direction, MethodInfo* method));
DO_APP_FUNC(0x00F4B8B0, HumanModel*, HumanController_get_HumanModel, (HumanController* __this, MethodInfo* method));
DO_APP_FUNC(0x012ADB00, Transform*, HumanModel_get_LookPoint, (HumanModel* __this, MethodInfo* method));
DO_APP_FUNC(0x01A2BA60, void, Debug_1_DrawLine, (Vector3 start, Vector3 end, Color color, MethodInfo* method));
DO_APP_FUNC(0x00387F80, Collider*, RaycastHit_get_collider, (RaycastHit__Boxed* __this, MethodInfo* method));
DO_APP_FUNC(0x00387F90, Rigidbody*, RaycastHit_get_rigidbody, (RaycastHit__Boxed* __this, MethodInfo* method));
DO_APP_FUNC(0x00C7CEB0, float, Vector3_Distance, (Vector3 a, Vector3 b, MethodInfo* method));
DO_APP_FUNC(0x01A56220, void, Gizmos_DrawRay, (Vector3 from, Vector3 direction, MethodInfo* method));
DO_APP_FUNC(0x02117B70, bool, Physics_SphereCast_1, (Vector3 origin, float radius, Vector3 direction, RaycastHit* hitInfo, float maxDistance, int32_t layerMask, MethodInfo* method));
DO_APP_FUNC(0x021154C0, Collider__Array*, Physics_OverlapSphere, (Vector3 position, float radius, int32_t layerMask, QueryTriggerInteraction__Enum queryTriggerInteraction, MethodInfo* method));
DO_APP_FUNC(0x01A9F6D0, String*, Object_1_get_name, (Object_1* __this, MethodInfo* method));
DO_APP_FUNC(0x01A28AB0, String*, Component_1_get_tag, (Component_1* __this, MethodInfo* method));
DO_APP_FUNC(0x01A28B00, Transform*, Component_1_get_transform, (Component_1* __this, MethodInfo* method));
DO_APP_FUNC(0x02117070, bool, Physics_Raycast_1, (Vector3 origin, Vector3 direction, float maxDistance, int32_t layerMask, MethodInfo* method));
DO_APP_FUNC(0x00EC49F0, void, ObjectInteractionController_DoInteraction, (ObjectInteractionController* __this, HumanController* character, MethodInfo* method));
DO_APP_FUNC(0x00456840, Transform*, ObjectInteractionController_FocusInterface_get_transform, (ObjectInteractionController* __this, MethodInfo* method));
DO_APP_FUNC(0x0106A5F0, stCommand_t, ScriptWork_ReadNext, (ScriptWork* __this, MethodInfo* method));
DO_APP_FUNC(0x0106A3B0, int32_t, ScriptWork_GetTextTableInt, (ScriptWork* __this, int32_t addr, int32_t size, MethodInfo* method));
DO_APP_FUNC(0x01052510, AdvMain_WorkList__Enum, AdvMain_ReadCommand, (AdvMain* __this, NpcController* npc, MethodInfo* method));
DO_APP_FUNC(0x00DF3FE0, TimeZone__Enum, TimeManager_get_TimeZone, (TimeManager* __this, MethodInfo* method));
DO_APP_FUNC(0x01B59BE0, TimeManager*, SingletonMonoBehaviour_1_TimeManager__get_Instance, (MethodInfo* method));
DO_APP_FUNC_METHODINFO(0x0301DF70, SingletonMonoBehaviour_1_TimeManager__get_Instance__MethodInfo);
DO_APP_FUNC(0x00727540, LifecycleState__Enum, NpcData_get_CurrentLifecycleState, (NpcData* __this, MethodInfo* method));
