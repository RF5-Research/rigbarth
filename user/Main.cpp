// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include <iostream>
#include "il2cpp-appdata.h"
#include "helpers.h"
#include "hooks/Hooks.hpp"
#include "DebugMenu.hpp"
#include <RigbarthCommands.hpp>
#include <AdvCommands.hpp>

// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

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
    AdvCommands::Initialize();
    DebugMenu::Initialize();
    Hooks::InstallHooks();
    RigbarthCommands::Subscribe();
}