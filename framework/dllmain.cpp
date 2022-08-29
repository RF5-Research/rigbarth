// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// DLL entry point

#include "il2cpp-init.h"
#include "Main.hpp"

// DLL entry point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    DisableThreadLibraryCalls(hModule);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InstallHooks, NULL, 0, NULL);
        init_il2cpp();
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) Run, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}