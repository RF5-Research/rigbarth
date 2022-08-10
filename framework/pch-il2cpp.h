// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_IL2CPP_H
#define PCH_IL2CPP_H
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS


// add headers that you want to pre-compile here
#include <cstdint>
// Application-specific types
#include "il2cpp-types.h"

// IL2CPP API function pointers
#include "il2cpp-api-functions-ptr.h"

// TypeInfo pointers
#define DO_TYPEDEF(a, n) extern n ## __Class** n ## __TypeInfo
namespace app {
#include "il2cpp-types-ptr.h"
}
#undef DO_TYPEDEF

// IL2CPP APIs
#define DO_API(r, n, p) extern r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#endif //PCH_IL2CPP_H