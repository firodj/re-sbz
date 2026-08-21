#include "pch.h"

#include <iostream>
#include <type_traits>
#include <fstream>

#include <pybind11/embed.h> // Essential header for embedding
namespace py = pybind11;

#include "console.hpp"
#include "globals.hpp"
#include "hnh_header.hpp"
#include "hnh_intern.hpp"
#include "util.hpp"

HWND g_hwndDesktop = 0;

namespace user32hook {
	// GetDesktopWindow

	typedef std::add_pointer_t<HWND WINAPI(
		void
	)> GetDesktopWindowFn;

	GetDesktopWindowFn oGetDesktopWindow = nullptr;

	HWND WINAPI hookGetDesktopWindow(void) {
		g_hwndDesktop = oGetDesktopWindow();
		DebugLog("[HNH] DesktopWindow = %x\n", g_hwndDesktop);
		return g_hwndDesktop;
	}

	// GetWindowRect
	typedef std::add_pointer_t<BOOL WINAPI(
		HWND	hWnd,
		LPRECT	pRect
	)> GetWindowRectFn;

	GetWindowRectFn oGetWindowRect = nullptr;

	BOOL WINAPI hookGetWindowRect(HWND	hWnd, LPRECT pRect) {
		BOOL bRet = oGetWindowRect(hWnd, pRect);
		if (bRet) {
			if (hWnd == g_hwndDesktop && pRect) {
				DebugLog("[User32] GetWindowRect -> %d, %d, %d, %d\n",
					pRect->left, pRect->top, pRect->right, pRect->bottom);
				pRect->right = 1024;
				pRect->bottom = 768;
			}
		}
		return bRet;
	}

	// RegisterClassA
	typedef std::add_pointer_t<
		ATOM
		WINAPI
		(
			const WNDCLASSA *lpWndClass
		)> RegisterClassAFn;
	RegisterClassAFn oRegisterClassA = nullptr;
	ATOM WINAPI hookRegisterClassA(const WNDCLASSA* lpWndClass) {
		DebugLog("[User32] RegisterClassA lpszClassName=%s lpfnWndProc=0x%x\n", lpWndClass->lpszClassName, lpWndClass->lpfnWndProc);
		ATOM aRet = oRegisterClassA(lpWndClass);
		return aRet;
	}

	// CreateWindowExA
	typedef std::add_pointer_t<
		HWND
		WINAPI
		(
			DWORD dwExStyle,
			LPCWSTR lpClassName,
			LPCWSTR lpWindowName,
			DWORD dwStyle,
			int X,
			int Y,
			int nWidth,
			int nHeight,
			HWND hWndParent,
			HMENU hMenu,
			HINSTANCE hInstance,
			LPVOID lpParam
		)> CreateWindowExWFn;
	CreateWindowExWFn oCreateWindowExW;
	HWND
		WINAPI
		hookCreateWindowExW(
			DWORD dwExStyle,
			LPCWSTR lpClassName,
			LPCWSTR lpWindowName,
			DWORD dwStyle,
			int X,
			int Y,
			int nWidth,
			int nHeight,
			HWND hWndParent,
			HMENU hMenu,
			HINSTANCE hInstance,
			LPVOID lpParam
		) {

#define BUFFER_SIZE 256
		char callerTrace[BUFFER_SIZE] = ""; // Initialize with an empty string
		size_t current_len = util::GetCallStack(callerTrace, BUFFER_SIZE, 3);

		char szClassName[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, lpClassName, -1, szClassName, MAX_PATH, nullptr, nullptr);
		char szWindowName[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, lpWindowName, -1, szWindowName, MAX_PATH, nullptr, nullptr);


		HWND hwnd = oCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth,
			nHeight, hWndParent, hMenu, hInstance, lpParam);

		DebugLog("[user32 %s] CreateWindowExA lpClassName=%s lpWindowName=%s hwndParent=0x%x hwnd=0x%x\n", callerTrace,
			szClassName, szWindowName, hWndParent, hwnd);

		return hwnd;
	}




	// TODO: update minhook
}

// WinMain
typedef std::add_pointer_t<int WINAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)> WinMainFn;

static WinMainFn origWinMain;

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

// WndProc
static WNDPROC origWndProc;

LRESULT CALLBACK hookWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	DebugLog("[HNH] WndProc hWnd=0x%x Msg=0x%x\n", hWnd, Msg);
	return origWndProc(hWnd, Msg, wParam, lParam);
}

LPVOID GetTargetEntryPoint() {
	// Passing NULL retrieves the base address of the main executable (.exe)
	HMODULE hModule = GetModuleHandleA(NULL);
	if (!hModule) return nullptr;

	// Cast the base address to access PE structures
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;

	// Check for valid DOS signature ('MZ')
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) return nullptr;

	// Locate the NT headers
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);

	// Check for valid NT signature ('PE')
	if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) return nullptr;

	// Calculate absolute Virtual Address: Base Address + AddressOfEntryPoint
	DWORD entryPointRVA = ntHeaders->OptionalHeader.AddressOfEntryPoint;
	return (LPVOID)((BYTE*)hModule + entryPointRVA);
}

bool CheckIsHNH() {
	// Check if executable is: HaNaHiMe.exe
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	const char* base = strrchr(path, '\\');
	base = base ? base + 1 : path;

	DebugLog("[HNH] Found Base Module: %p\n", base);

	if (_stricmp(base, "HaNaHiMe.exe") == 0) {
		DebugLog("[HNH] Detected HaNaHiMe. Applying HNH-specific hooks.\n");
		return true;
	}

	return false;
}

void InitHNH() {
	LPVOID pfnWinMain = (LPVOID)0x485CE0;

	MH_STATUS res = MH_CreateHook(pfnWinMain, reinterpret_cast<LPVOID>(hookWinMain), reinterpret_cast<LPVOID*>(&origWinMain));
	if (res == MH_OK) res = MH_EnableHook(pfnWinMain);
	DebugLog("[HNH] Hooked WinMain; %s\n", MH_StatusToString(res));

	res = MH_CreateHook(GetDesktopWindow, reinterpret_cast<LPVOID>(user32hook::hookGetDesktopWindow), reinterpret_cast<LPVOID*>(&user32hook::oGetDesktopWindow));
	if (res == MH_OK) res = MH_EnableHook(GetDesktopWindow);
	DebugLog("[HNH] Hooked GetDesktopWindow; %s\n", MH_StatusToString(res));

	res = MH_CreateHook(GetWindowRect, reinterpret_cast<LPVOID>(user32hook::hookGetWindowRect), reinterpret_cast<LPVOID*>(&user32hook::oGetWindowRect));
	if (res == MH_OK) res = MH_EnableHook(GetWindowRect);
	DebugLog("[HNH] Hooked GetWindowRect; %s\n", MH_StatusToString(res));

	res = MH_CreateHook(RegisterClassA, reinterpret_cast<LPVOID>(user32hook::hookRegisterClassA), reinterpret_cast<LPVOID*>(&user32hook::oRegisterClassA));
	if (res == MH_OK) res = MH_EnableHook(RegisterClassA);
	DebugLog("[HNH] Hooked RegisterClassA; %s\n", MH_StatusToString(res));

	res = MH_CreateHook(CreateWindowExW, reinterpret_cast<LPVOID>(user32hook::hookCreateWindowExW), reinterpret_cast<LPVOID*>(&user32hook::oCreateWindowExW));
	if (res == MH_OK) res = MH_EnableHook(CreateWindowExW);
	DebugLog("[HNH] Hooked CreateWindowExW; %s\n", MH_StatusToString(res));

	//LPVOID pfnWndProc = (LPVOID)0x428c11;
	//res = MH_CreateHook(pfnWndProc, reinterpret_cast<LPVOID>(hookWndProc), reinterpret_cast<LPVOID*>(&origWndProc));
	//if (res == MH_OK) res = MH_EnableHook(pfnWndProc);
	//DebugLog("[HNH] Hooked WndProc; %s\n", MH_StatusToString(res));

}

void FinishHNH() {
	
}

void DrawHNH() {
	static bool isOpen = true;
	// Window flags
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("HNH Menu", &isOpen, flags)) {

		ImGui::Text("Press <INSERT> to toggle menu.");
	}
	ImGui::End();
	
}


//////////////////

std::string getModuleDir() {
	char buffer[MAX_PATH];
	// Pass NULL to get the path of the current executable. 
	// If inside a DLL, pass the DLL's HMODULE instead.
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	DebugLog("[HNH] WinMain !\n");

	py::scoped_interpreter guard{};

	InitializePythonInterpreter();

	int ret = origWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	FinishHNH();

	return ret;
}

