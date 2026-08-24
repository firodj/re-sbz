#include "pch.h"

#include <iostream>
#include <type_traits>
#include <fstream>

#include "console.hpp"
#include "globals.hpp"
#include "menu.hpp"
#include "util.hpp"
#include "user32hook.hpp"

#include "hnh_header.hpp"
#include "hnh_intern.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace user32hook {
///////////////////////////////////////////////////////////////// user32hook //

// GetDesktopWindow
typedef std::add_pointer_t<HWND WINAPI(
	void
)> GetDesktopWindowFn;

GetDesktopWindowFn oGetDesktopWindow = nullptr;

HWND WINAPI hookGetDesktopWindow(void);

// GetWindowRect
typedef std::add_pointer_t<BOOL WINAPI(
	HWND	hWnd,
	LPRECT	pRect
)> GetWindowRectFn;

GetWindowRectFn oGetWindowRect = nullptr;

BOOL WINAPI hookGetWindowRect(HWND	hWnd, LPRECT pRect);

// RegisterClassA
typedef std::add_pointer_t<
	ATOM
	WINAPI
	(
		const WNDCLASSA *lpWndClass
	)> RegisterClassAFn;
RegisterClassAFn oRegisterClassA = nullptr;
ATOM WINAPI hookRegisterClassA(const WNDCLASSA* lpWndClass);

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
	);

// PeekMessageA
typedef std::add_pointer_t<
	BOOL
	WINAPI
	(
		LPMSG lpMsg,
		HWND  hWnd,
		UINT  wMsgFilterMin,
		UINT  wMsgFilterMax,
		UINT  wRemoveMsg
	)> PeekMessageAFn;
PeekMessageAFn oPeekMessageA = nullptr;
BOOL WINAPI hookPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);

SHORT WINAPI hookGetAsyncKeyState(int vKey);

HWND g_hwndDesktop = 0;
HWND WINAPI hookGetDesktopWindow(void) {
	g_hwndDesktop = oGetDesktopWindow();
	DebugLog("[HNH] DesktopWindow = %x\n", g_hwndDesktop);
	return g_hwndDesktop;
}

BOOL WINAPI hookGetWindowRect(HWND	hWnd, LPRECT pRect) {
	BOOL bRet = oGetWindowRect(hWnd, pRect);
	if (bRet) {
		if (hWnd == g_hwndDesktop && pRect) {
			DebugLog("[User32] GetWindowRect -> %d, %d, %d, %d\n",
				pRect->left, pRect->top, pRect->right, pRect->bottom);
			pRect->right = 1280;
			pRect->bottom = 960;
		}
	}
	return bRet;
}

ATOM WINAPI hookRegisterClassA(const WNDCLASSA* lpWndClass) {
	DebugLog("[User32] RegisterClassA lpszClassName=%s lpfnWndProc=0x%x\n", lpWndClass->lpszClassName, lpWndClass->lpfnWndProc);
	ATOM aRet = oRegisterClassA(lpWndClass);
	return aRet;
}

HWND WINAPI hookCreateWindowExW(
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

BOOL WINAPI hookPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) {
	BOOL ret = oPeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
	if (ret && lpMsg) {
		const char* msgName = "";
		bool wm_input = true;
		bool logging = false;
		bool translate = false;
		switch (lpMsg->message) {
		case WM_CREATE:			msgName = "WM_CREATE";	break;
		case WM_DESTROY:		msgName = "WM_DESTROY";	break;
		case WM_MOUSEMOVE:		msgName = "WM_MOUSEMOVE";		break;
		case WM_NCMOUSEMOVE:	msgName = "WM_NCMOUSEMOVE";		break;
		case WM_MOUSELEAVE:		msgName = "WM_MOUSELEAVE";		break;
		case WM_NCMOUSELEAVE:	msgName = "WM_NCMOUSELEAVE";	break;
		case WM_LBUTTONDOWN:	msgName = "WM_LBUTTONDOWN";		break;
		case WM_LBUTTONUP:		msgName = "WM_LBUTTONUP";		break;
		case WM_LBUTTONDBLCLK:	msgName = "WM_LBUTTONDBLCLK";	break;
		case WM_RBUTTONDOWN:	msgName = "WM_RBUTTONDOWN";		break;
		case WM_RBUTTONUP:		msgName = "WM_RBUTTONUP";		break;
		case WM_RBUTTONDBLCLK:	msgName = "WM_RBUTTONDBLCLK";	break;
		case WM_MBUTTONDOWN:	msgName = "WM_MBUTTONDOWN";		break;
		case WM_MBUTTONUP:		msgName = "WM_MBUTTONUP";		break;
		case WM_MBUTTONDBLCLK:	msgName = "WM_MBUTTONDBLCLK";	break;
		case WM_MOUSEWHEEL: msgName = "WM_MOUSEWHEEL"; break;
		case WM_MOUSEHWHEEL: msgName = "WM_MOUSEHWHEEL"; break;
		case WM_XBUTTONDOWN:	msgName = "WM_XBUTTONDOWN";		break;
		case WM_XBUTTONUP:		msgName = "WM_XBUTTONUP";		break;
		case WM_XBUTTONDBLCLK:	msgName = "WM_XBUTTONDBLCLK";	break;
		case WM_KEYDOWN: msgName = "WM_KEYDOWN"; logging = true; translate = true; break;
		case WM_KEYUP: msgName = "WM_KEYUP"; logging = true; translate = true; break;
		case WM_CHAR: msgName = "WM_CHAR"; logging = true; break;
		case WM_SYSKEYDOWN: msgName = "WM_SYSKEYDOWN"; break;
		case WM_SYSKEYUP: msgName = "WM_SYSKEYUP"; break;
		case WM_SYSCHAR: msgName = "WM_SYSCHAR"; break;
		case WM_SETFOCUS: msgName = "WM_SETFOCUS"; break;
		case WM_KILLFOCUS: msgName = "WM_KILLFOCUS"; break;
		case WM_INPUTLANGCHANGE: msgName = "WM_INPUTLANGCHANGE"; break;

		default:
			wm_input = false;
			switch (lpMsg->message) {

			case WM_MOVE:			msgName = "WM_MOVE";		break;
			case WM_SIZE:			msgName = "WM_SIZE";		break;
			case WM_ACTIVATE:		msgName = "WM_ACTIVATE"; 	break;
			case WM_PAINT:			msgName = "WM_PAINT";		break;
			case WM_TIMER:			msgName = "WM_TIMER";	break;
			default:
				logging = true;
			}
		}
		if (wm_input && translate) {
			TranslateMessage(lpMsg);
		}
		if (wm_input && ImGui::GetCurrentContext() != nullptr) {
			ImGui_ImplWin32_WndProcHandler(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
			ImGuiIO& io = ImGui::GetIO();
		}

		if (logging) {
			DebugLog("[user32] PeekMessage req.hWnd=0x%x hWnd=0x%x message=0x%x(%s) wParam=0x%x lParam=0x%x time=%d pt=(%d,%d)\n",
				hWnd, lpMsg->hwnd,
				lpMsg->message, msgName, lpMsg->wParam, lpMsg->lParam,
				lpMsg->time, lpMsg->pt.x, lpMsg->pt.y
			);
		}
	}
	return ret;
}

SHORT WINAPI hookGetAsyncKeyState(int vKey) {
	if (menu::isOpen) {
		return 0;
	}
	return globals::oGetAsyncKeyState(vKey);
}

void Init() {
	MH_STATUS res;

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

	res = MH_CreateHook(PeekMessageA, reinterpret_cast<LPVOID>(user32hook::hookPeekMessageA), reinterpret_cast<LPVOID*>(&user32hook::oPeekMessageA));
	if (res == MH_OK) res = MH_EnableHook(PeekMessageA);
	DebugLog("[HNH] Hooked PeekMessageA; %s\n", MH_StatusToString(res));

	res = MH_CreateHook(GetAsyncKeyState, reinterpret_cast<LPVOID>(user32hook::hookGetAsyncKeyState), reinterpret_cast<LPVOID*>(&globals::oGetAsyncKeyState));
	if (res == MH_OK) res = MH_EnableHook(GetAsyncKeyState);
	DebugLog("[HNH] Hooked GetAsyncKeyState; %s\n", MH_StatusToString(res));
}

///////////////////////////////////////////////////////////////// user32hook //
}