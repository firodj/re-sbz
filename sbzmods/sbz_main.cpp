#include "pch.h"

#include <iostream>
#include <type_traits>
#include <fstream>
#include <cinttypes>

#include "console.hpp"
#include "globals.hpp"
#include "util.hpp"
#include "sbz_header.hpp"
#include "sbz_intern.hpp"
#include "src/struct.hpp"

sbz::CSBZGlobal (&g_SBZGlobal) = *(sbz::CSBZGlobal*)0x880680;

typedef std::add_pointer_t<int WINAPI(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)> WinMainFn;

static WinMainFn origWinMain;

int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

void* (__cdecl* orig_new)(size_t Size) = nullptr;
void* __cdecl hook_new(size_t Size);

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

bool CheckIsSBZ() {
	// Check if executable is: Sexy Beach Zero English.exe
	char path[MAX_PATH];
	GetModuleFileNameA(NULL, path, MAX_PATH);
	const char* base = strrchr(path, '\\');
	base = base ? base + 1 : path;

	DebugLog("[DLLMain] Found Base Module: %p\n", base);

	if (_stricmp(base, "Sexy Beach Zero English.exe") == 0) {
		return true;
	}

	return false;
}
namespace sbz {
	void register_Pack();
}

void InitSBZ()
{

	util::Hook(0x403F90, hookWinMain, "WinMain_403F90", (LPVOID*)&origWinMain);
	util::HookOff();
	util::Hook(0x7C3320, hook_new, "new_7C3320", (LPVOID*)&orig_new);
	util::HookOn();

	sbz::register_Pack();
}

void* __cdecl hook_new(size_t Size) {

	void* ptr = orig_new(Size);
	DebugLog("[SBZ] new(0x%x) => 0x%08X\n", Size, (UINT_PTR)ptr);

	return ptr;
}

bool SBZCustomRender(const char* window_name)
{
	if (!strcmp("World Viewer", window_name)) {
		static bool isOpen = true;
		// Window flags
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
		ImGui::SetNextWindowSize(ImVec2(450, 600), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(25, 25), ImGuiCond_FirstUseEver);

		if (ImGui::Begin(window_name, &isOpen, flags)) {

			ImGui::Text("Press <INSERT> to toggle menu.");

			sbz::CSceneEntry* pSceneEntry;
			int nChildCount;

			sbz::CSceneStack &sceneStack = g_SBZGlobal.sceneStack_1A8;
			int nTopIdx = 0;
			if (sceneStack.nCount > 0) {
				nTopIdx = sceneStack.nTopIdx;
			}
			pSceneEntry = &sceneStack.scenes[nTopIdx];
			nChildCount = pSceneEntry->nChildCount;
			
			ImGui::Text("sceneStack.nCount = %d", sceneStack.nCount);
			ImGui::Text("nTopIdx = %d", nTopIdx);
			ImGui::Text("nChildCount = %d", nChildCount);
			ImGui::Text("cb.nID = %d %s", pSceneEntry->cb.nID, std::string(sbz::SceneToString((sbz::ESBZScene)pSceneEntry->cb.nID)).c_str());

			for (int i = 0; i < nChildCount; i++) {
				int nChildIdx = pSceneEntry->childIndices[i];
				int nChildID = sceneStack.scenes[nChildIdx].cb.nID;
				ImGui::Text("- [%d] cb.nID = %d %s", nChildIdx, nChildID, std::string(sbz::SceneToString((sbz::ESBZScene)nChildID)).c_str());
			}

			if (ImGui::CollapsingHeader("Scene Hack")) {
				static const char* scene_ops[3] = { "0-PushScene_643800", "1-PushSceneWState_6431F0", "2-PopSceneMaybe_643520" };
				static int current_scene_ops = 0;
				static int scene_state = 1;
				static int scene_transistion = 0;
				static int scene_fade_in = 500;
				static int scene_fade_out = 500;
				static sbz::ESBZScene current_scene_id = (sbz::ESBZScene)-1; // Index of the current selection
				if (ImGui::BeginCombo("Scene Ops", scene_ops[current_scene_ops])) {
					for (int i = 0; i < ARRAYSIZE(scene_ops); i++) {
						const bool is_selected = (current_scene_ops == i);

						if (ImGui::Selectable(scene_ops[i], is_selected))
						{
							current_scene_ops = i;
						}

						// Set the initial focus when opening the combo
						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				if (ImGui::InputInt("Scene State", &scene_state)) {
					if (scene_state < 1) {
						scene_state = 1;
					}
					else if (scene_state > 3) {
						scene_state = 3;
					}
				}

				if (ImGui::InputInt("Scene Transition", &scene_transistion)) {
					if (scene_state < 0) {
						scene_state = 0;
					}
					else if (scene_state > 16) {
						scene_state = 16;
					}
				}

				if (ImGui::BeginCombo("Scene Select", std::string(SceneToString(current_scene_id)).c_str()))
				{
					sbz::EnumerateScenes([](sbz::ESBZScene scene, std::string_view name)
						{
							const bool is_selected = (current_scene_id == scene);
							if (ImGui::Selectable(std::string(name).c_str(), is_selected))
							{
								current_scene_id = scene;
							}

							// Set the initial focus when opening the combo
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}

							//std::cout << static_cast<std::int32_t>(scene) << " -> " << name << '\n';
							return true; // return false to stop early
						});
					ImGui::EndCombo();
				}

				if (ImGui::Button("Jump")) {
					sbz::InitScene_4951A0(current_scene_id, current_scene_ops, scene_state, scene_transistion, scene_fade_in, scene_fade_out);
				}
			}

			ImGui::End();
		}
		return true;
	}
	if (!strcmp("Scene Preview", window_name)) {
		return true;
	}

	return false;
}


//////////////////

std::string getModuleDir()
{
	char buffer[MAX_PATH];
	// Pass NULL to get the path of the current executable. 
	// If inside a DLL, pass the DLL's HMODULE instead.
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}


int WINAPI hookWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	DebugLog("[DllSBZ] WinMain !\n");

	int ret = origWinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);


	return ret;
}

