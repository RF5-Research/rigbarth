// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"
#include <iostream>
#include "il2cpp-appdata.h"
#include "helpers.h"
#include "advScript.h"
using namespace app;

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

uint64_t Tramp_ScriptWork__cctor;
NOINLINE void __cdecl Hook_ScriptWork__cctor(MethodInfo* method)
{
    PLH::FnCast(Tramp_ScriptWork__cctor, ScriptWork__cctor)(method);
    //ScriptWork__TypeInfo scriptWork = ScriptWork__TypeInfo();
    //scriptWork->static_fields->CommandArgList;
    auto cmds = (*ScriptWork__TypeInfo)->static_fields->CommandArgList;
    std::string json = std::string("[\n");
    for (int i = 0; i < cmds->max_length; i++)
    {
        auto cmd_args = cmds->vector[i];
        if (cmd_args)
        {
            auto command_args = std::vector<int>();
            json.append("{\t\"ID\": " + std::to_string(i) + ",\n");
            json.append("\t\"Params\": [\n");
            for (int ii = 0; ii < cmd_args->max_length; ii++)
            {
                if (ii + 1 == cmd_args->max_length)
                    json.append("\t\t" + std::to_string(cmd_args->vector[ii]) + "\n");
                else
                    json.append("\t\t" + std::to_string(cmd_args->vector[ii]) + ",\n");
            }
            json.append("\n\t]\n\t},\n");
        }
    }
    printf("Test");
}


// Custom injected code entry point
void Run()
{
    // Initialize thread data - DO NOT REMOVE
    il2cpp_thread_attach(il2cpp_domain_get());

    // If you would like to write to a log file, specify the name above and use il2cppi_log_write()
    // il2cppi_log_write("Startup");

    // If you would like to output to a new console window, use il2cppi_new_console() to open one and redirect stdout
     il2cppi_new_console();

    // Place your custom code here
    advScript::InstallHooks();

    //PLH::CapstoneDisassembler dis(PLH::Mode::x64);
    //auto detour_AdvScriptReader_Init_d_4_MoveNext = new PLH::x64Detour(
    //    (char*)AdvScriptReader_Init_d_4_MoveNext,
    //    (char*)Hook_ScriptWork__cctor,
    //    &Tramp_ScriptWork__cctor,
    //    dis
    //);
    //detour_AdvScriptReader_Init_d_4_MoveNext->hook();

}