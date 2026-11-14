// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#if defined _M_X64
#pragma comment(lib, "minhook/lib/libMinHook.x64.lib") // For 64-bit builds
#elif defined _M_IX86
#pragma comment(lib, "minhook/lib/libMinHook.x86.lib") // For 32-bit builds
#endif

#ifdef ENABLE_BACKEND_DX9
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d9.lib")
#else
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#endif

#ifdef ENABLE_BACKEND_VULKAN
#pragma comment(lib, "vulkan-1.lib")
#endif

#include <windows.h>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <iostream>

#ifdef ENABLE_BACKEND_DX9
#include <dxgi.h>
#include <dxgi1_4.h>
#include <d3d9.h>
#else
#include <d3d10_1.h>
#include <d3d10.h>
#include <d3d11.h>
#include <d3d12.h>
#endif
#ifdef ENABLE_BACKEND_VULKAN
#include <vulkan/vulkan.h>
#endif


#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"

#ifdef ENABLE_BACKEND_DX9
#include "imgui/backends/imgui_impl_dx9.h"
#else
#include "imgui/backends/imgui_impl_dx10.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/backends/imgui_impl_dx12.h"
#endif

#ifdef ENABLE_BACKEND_VULKAN
#include "imgui/backends/imgui_impl_vulkan.h"
#endif

#include "minhook/include/MinHook.h"

#endif //PCH_H
