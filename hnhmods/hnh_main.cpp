#include "pch.h"

#include <iostream>
#include <type_traits>
#include <fstream>
#include <sstream>
#include <iomanip> // Required for setw and setfill

#include <pybind11/embed.h> // Essential header for embedding
namespace py = pybind11;

#include "console.hpp"
#include "globals.hpp"
#include "menu.hpp"
#include "hnh_header.hpp"
#include "hnh_intern.hpp"
#include "util.hpp"
#include "user32hook.hpp"
#include "imgui_internal.h"

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

void register_WinMain_485CE0();
void register_WndProcCheckSystem_485FC2();
void register_CheckInstalledDirRoot_4869A7();
void register_MemAlloc_40DEF7();
void register_SetFogEnable_40DFB0();
void register_IsFileExists_47E8EE();
void register_DSErrToString_423CE7();
void AssertHanaStructSize();
void register_CHanaObj__Free_40C1E4();
void register_MatrixLookAtLH_40C64A();
void register_CheckRunGame_4863C8();
void register_CDROM_VerifyDiscSignature_485C50();
void register_ShowTitleMenu_427DA8();
void register_CHanaObj__NewFrameT3EC_S354_404720();
void register_WndProcTitleMenu_428C11();
void register_GameMainLoop_47F013();
void register_StopAndRestartThread_481AE4();
void register_LoadSoundEffects_47EB7F();
void register_InGameMenu_4799E2();
void register_MenuCharSelect_47A324();
void register_PlayScreenFade_4874DE();

HANDLE g_hStartEvent = NULL;

void InitHNH() {
	g_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	MH_STATUS res;

	LPVOID pfnWinMain = (LPVOID)0x485CE0;
	res = MH_CreateHook(pfnWinMain, reinterpret_cast<LPVOID>(hookWinMain), reinterpret_cast<LPVOID*>(&origWinMain));
	if (res == MH_OK) res = MH_EnableHook(pfnWinMain);
	DebugLog("[HNH] Hooked WinMain; %s\n", MH_StatusToString(res));

	user32hook::Init();

	//LPVOID pfnWndProc = (LPVOID)0x428c11;
	//res = MH_CreateHook(pfnWndProc, reinterpret_cast<LPVOID>(hookWndProc), reinterpret_cast<LPVOID*>(&origWndProc));
	//if (res == MH_OK) res = MH_EnableHook(pfnWndProc);
	//DebugLog("[HNH] Hooked WndProc; %s\n", MH_StatusToString(res));

	AssertHanaStructSize();
	
	register_MatrixLookAtLH_40C64A();
	register_WndProcCheckSystem_485FC2();
	register_CDROM_VerifyDiscSignature_485C50();
	register_CheckInstalledDirRoot_4869A7();
	register_MemAlloc_40DEF7();
	register_SetFogEnable_40DFB0();
	register_IsFileExists_47E8EE();
	register_DSErrToString_423CE7();
	register_CHanaObj__Free_40C1E4();
	register_CheckRunGame_4863C8();
	register_ShowTitleMenu_427DA8();
	register_CHanaObj__NewFrameT3EC_S354_404720();
	register_WndProcTitleMenu_428C11();
	register_GameMainLoop_47F013();
	register_StopAndRestartThread_481AE4();
	register_LoadSoundEffects_47EB7F();
	register_InGameMenu_4799E2();
	register_MenuCharSelect_47A324();
	register_PlayScreenFade_4874DE();

	DebugLog("[HNH] All hooking done !\n");
	if (g_hStartEvent)
	{
		// Signal the event, waking up the main thread
		SetEvent(g_hStartEvent);
	}
}

void FinishHNH() {
	
}

struct CSceneState  // sizeof=0x190
{
	BYTE  bActiveSceneStep;      // +0x00 index into g_arrSceneNo_4C6F78
	BYTE  bCurrentSubStage;      // +0x01
	BYTE  gap02[2];              // +0x02
	int   nSceneDataIndices[11]; // +0x04
	BYTE  gap30[2];              // +0x30
	char  byte_32;              // +0x32
	BYTE  gap33;                 // +0x33
	int   nSceneExtraData[24];   // +0x34
	BYTE  gap94[3];              // +0x94
	char  bMaxSubStage;          // +0x97
	int   field_98[1];        // +0x98
	int   field_9C[9];                     // +0x09C
	float field_C0;                        // +0x0C0
	float field_C4;                        // +0x0C4
	__int64 qwSelMask_C8;                  // +0x0C8
	int   field_D0;                        // +0x0D0
	int   gapD4[10];                       // +0x0D4
	int   field_FC;                        // +0x0FC
	int   gap100[31];                      // +0x100
	int   field_17C;                       // +0x17C
	float frameTimeMultiplied_180;         // +0x180
	LPVOID lpDDSurface_184;  // +0x184
	float fPosX_188;                       // +0x188
	float fPosY_18C;                       // +0x18C
};

class StructFielder {
	DWORD m_offset;
	std::string m_label;
	std::string m_input_id;
	ImGuiDataType m_data_type;
	int m_data_size;
	LPVOID m_p_data;
	const char* m_format;
	bool m_is_array{};
	bool m_is_gap{};

	std::vector<StructFielder> elements;

public:
	StructFielder() : m_offset(0), m_label(""), m_data_type(0), m_p_data(nullptr),
		m_format(""), m_input_id( ""), m_is_array(false), m_is_gap(false), m_data_size(0)
	{}
	StructFielder(DWORD offset, const char* label, ImGuiDataType data_type, LPVOID p_data) {
		m_offset = offset;
		m_label = label;
		m_input_id = "##" + m_label;
		m_data_type = data_type;
		m_p_data = p_data;

		const char* gap_pfx = "gap";
		m_is_gap = (strncmp(label, gap_pfx, strlen(gap_pfx)) == 0);

		switch (data_type) {
		case ImGuiDataType_S8:
			m_data_size = 1;
			if (m_is_gap) { m_format = "%02X"; break; }
		case ImGuiDataType_S16:
			m_data_size = 2;
			if (m_is_gap) { m_format = "%04X"; break; }
		case ImGuiDataType_S32:
			m_data_size = 4;
			if (m_is_gap) { m_format = "%08X"; break; }
			m_format = "%d"; break;
		case ImGuiDataType_U8:
			m_data_size = 1;
			if (m_is_gap) { m_format = "%02X"; break; }
		case ImGuiDataType_U16:
			m_data_size = 2;
			if (m_is_gap) { m_format = "%04X"; break; }
		case ImGuiDataType_U32:
			m_data_size = 4;
			if (m_is_gap) { m_format = "%08X"; break; }
			m_format = "%u"; break;
		case ImGuiDataType_S64:
			m_data_size = 8;
			m_format = "%" PRId64; break;
		case ImGuiDataType_U64:
			m_data_size = 8;
			m_format = "%" PRIu64; break;
		case ImGuiDataType_Float:
			m_data_size = 4;
			m_format = "%.10f"; break;
		case ImGuiDataType_Double:
			m_data_size = 8;
			m_format = "%.10f"; break;
		default:
			m_format = "?";
		}
	}

	StructFielder(DWORD offset, const char* label, ImGuiDataType data_type, size_t array_count, LPVOID p_data):
		StructFielder(offset, label, data_type, p_data) 
	{
		AsArray(array_count);
	}

	void AsArray(size_t array_count) {
		if (array_count == 0) return;

		m_is_array = true;
		DWORD offset_elem = m_offset;

		if (m_data_type == ImGuiDataType_S8 || m_data_type == ImGuiDataType_U8) {
			LPBYTE p_elem = (LPBYTE)m_p_data;
			for (size_t i = 0; i < array_count; i++) {
				std::stringstream ss;
				ss << m_label << "[" << i << "]";
				elements.emplace_back(offset_elem, ss.str().c_str(), m_data_type, p_elem++);
				offset_elem += m_data_size;
			}
		}
		else if (m_data_type == ImGuiDataType_S32 || m_data_type == ImGuiDataType_U32) {
			LPDWORD p_elem = (LPDWORD)m_p_data;
			for (size_t i = 0; i < array_count; i++) {
				std::stringstream ss;
				ss << m_label << "[" << i << "]";
				elements.emplace_back(offset_elem, ss.str().c_str(), m_data_type, p_elem++);
				offset_elem += m_data_size;
			}
		}


	}

	void Draw() {
		if (!m_is_array) {
			DrawLine(m_offset, m_label.c_str(), m_input_id.c_str(), m_data_type, m_format, m_p_data);
		}
		else {
			if (m_is_gap) {
				DrawGap(m_offset, m_label.c_str());
				return;
			}
			for (auto& e : elements) {
				e.Draw();
			}
		}
	}

	void DrawHeadRow(DWORD offset, const char* label) {
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("0x%X", offset);

		if (strlen(label) > 0) {
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(label);
		}
	}
	void DrawLine(DWORD offset, const char* label, const char* input_id, ImGuiDataType data_type, const char* format, LPVOID p_data) {
		DrawHeadRow(offset, label);

		ImGui::TableSetColumnIndex(2);
		ImGui::InputScalar(input_id,
			data_type,
			p_data,
			NULL, NULL, format, ImGuiInputTextFlags_ReadOnly);
	}

	void DrawGap(DWORD offset, const char* label) {
		std::string s;
		DWORD offset_elem = offset;
		char buffer[256];
		int i = 0;
		int r = 0;

		auto DrawThem = [&]() {
			DrawHeadRow(offset_elem, r ? "" : label);
			ImGui::TableSetColumnIndex(2);
			ImGui::Text(s.c_str());
			};

		for (auto& e : elements) {
			if (!s.empty()) s += " ";
			ImGui::DataTypeFormatString(buffer, sizeof(buffer), e.m_data_type, e.m_p_data, e.m_format);
			s += buffer;
			i += e.m_data_size;

			if (i % 8 == 0) {
				DrawThem();

				s.clear();
				offset_elem += i;

				i = 0;
				r++;
			}
		}

		if (i) {
			DrawThem();
		}
	}
};

class StructViewer {
public:
	std::vector<StructFielder> fields;

	void Draw() {
		for (auto &f: fields) {
			f.Draw();
		}
	}
};

void DrawHNH(const char *window_name) {
	static_assert(sizeof(CSceneState) == 0x190, "CSceneState");

	CSceneState& g_sceneState_52F7E0 = *(CSceneState*)0x52F7E0;

	static bool isOpen = true;
	// Window flags
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_FirstUseEver);

	
	ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable;
	
	static StructViewer sviewer;
	if (sviewer.fields.size() == 0) {
		sviewer.fields.emplace_back(offsetof(CSceneState, bActiveSceneStep), "bActiveSceneStep", ImGuiDataType_U8, &g_sceneState_52F7E0.bActiveSceneStep);
		sviewer.fields.emplace_back(offsetof(CSceneState, bCurrentSubStage), "bCurrentSubStage", ImGuiDataType_U8, &g_sceneState_52F7E0.bCurrentSubStage);
		sviewer.fields.emplace_back(offsetof(CSceneState, gap02), "gap02", ImGuiDataType_U8, 2, &g_sceneState_52F7E0.gap02);
		sviewer.fields.emplace_back(offsetof(CSceneState, nSceneDataIndices), "nSceneDataIndices", ImGuiDataType_S32, 11, &g_sceneState_52F7E0.nSceneDataIndices);
		sviewer.fields.emplace_back(offsetof(CSceneState, gap30), "gap30", ImGuiDataType_U8, 2, &g_sceneState_52F7E0.gap30);
		sviewer.fields.emplace_back(offsetof(CSceneState, byte_32), "byte_32", ImGuiDataType_S8, &g_sceneState_52F7E0.byte_32);
		sviewer.fields.emplace_back(offsetof(CSceneState, gap33), "gap33", ImGuiDataType_U8, 1, &g_sceneState_52F7E0.gap33);
		sviewer.fields.emplace_back(offsetof(CSceneState, nSceneExtraData), "nSceneExtraData", ImGuiDataType_S32, 24, &g_sceneState_52F7E0.nSceneExtraData);
		sviewer.fields.emplace_back(offsetof(CSceneState, gap94), "gap94", ImGuiDataType_U8, 3, &g_sceneState_52F7E0.gap94);
		sviewer.fields.emplace_back(offsetof(CSceneState, bMaxSubStage), "bMaxSubStage", ImGuiDataType_S8, &g_sceneState_52F7E0.bMaxSubStage);
		sviewer.fields.emplace_back(offsetof(CSceneState, field_98), "field_98", ImGuiDataType_S32, 1, &g_sceneState_52F7E0.field_98);
		sviewer.fields.emplace_back(offsetof(CSceneState, field_9C), "field_9C", ImGuiDataType_S32, 9, &g_sceneState_52F7E0.field_9C);
		sviewer.fields.emplace_back(offsetof(CSceneState, field_C0), "field_C0", ImGuiDataType_Float, &g_sceneState_52F7E0.field_C0);
		sviewer.fields.emplace_back(offsetof(CSceneState, field_C4), "field_C4", ImGuiDataType_Float, &g_sceneState_52F7E0.field_C4);
		sviewer.fields.emplace_back(offsetof(CSceneState, qwSelMask_C8), "qwSelMask_C8", ImGuiDataType_S64, &g_sceneState_52F7E0.qwSelMask_C8);
		sviewer.fields.emplace_back(offsetof(CSceneState, field_D0), "field_D0", ImGuiDataType_S32, &g_sceneState_52F7E0.field_D0);
		sviewer.fields.emplace_back(offsetof(CSceneState, gapD4), "gapD4", ImGuiDataType_U32, 10, &g_sceneState_52F7E0.gapD4);
		sviewer.fields.emplace_back(offsetof(CSceneState, field_FC), "field_FC", ImGuiDataType_S32, &g_sceneState_52F7E0.field_FC);
		sviewer.fields.emplace_back(offsetof(CSceneState, gap100), "gap100", ImGuiDataType_U32, 31, &g_sceneState_52F7E0.gap100);
		sviewer.fields.emplace_back(offsetof(CSceneState, field_17C), "field_17C", ImGuiDataType_S32, &g_sceneState_52F7E0.field_17C);
		sviewer.fields.emplace_back(offsetof(CSceneState, frameTimeMultiplied_180), "frameTimeMultiplied_180", ImGuiDataType_Float, &g_sceneState_52F7E0.frameTimeMultiplied_180);
		sviewer.fields.emplace_back(offsetof(CSceneState, lpDDSurface_184), "lpDDSurface_184", ImGuiDataType_U32, &g_sceneState_52F7E0.lpDDSurface_184);
		sviewer.fields.emplace_back(offsetof(CSceneState, fPosX_188), "fPosX_188", ImGuiDataType_Float, &g_sceneState_52F7E0.fPosX_188);
		sviewer.fields.emplace_back(offsetof(CSceneState, fPosY_18C), "fPosY_18C", ImGuiDataType_Float, &g_sceneState_52F7E0.fPosY_18C);
	}

	if (ImGui::Begin(window_name, &isOpen, flags)) {
		ImGui::Text("Press <INSERT> to toggle menu.");
		if (ImGui::TreeNode("g_SceneState")) {
			if (ImGui::BeginTable("table1", 3, table_flags))
			{
				// Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
				// (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
				ImGui::TableSetupColumn("Address");
				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("Value");
				ImGui::TableHeadersRow();

				sviewer.Draw();

				ImGui::EndTable();
			}
			
			ImGui::TreePop();
		}
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

int __stdcall WinMain_485CE0(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	DebugLog("[HNH] WinMain ! Wait for Start Event...\n");
	WaitForSingleObject(g_hStartEvent, INFINITE);
	DebugLog("[HNH] WinMain continue ...\n");

	py::scoped_interpreter guard{};

	InitializePythonInterpreter();

	int ret = WinMain_485CE0(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	FinishHNH();

	return ret;
}

