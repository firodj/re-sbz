#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

/**
 * Selects the fog mode (D3DFOG_*) as table (pixel) fog or vertex fog.
 * @param bFogTableEnable Requests pixel/table fog; only honored when the device
 *                        advertises D3DPRASTERCAPS_FOGTABLE, otherwise vertex
 *                        fog is used.
 * @param dwFogMode D3DFOG_NONE / D3DFOG_EXP / D3DFOG_EXP2 / D3DFOG_LINEAR.
 * @return HRESULT of the SetRenderState call performed.
 */
HRESULT __cdecl SetFogMode_40E02D(int bFogTableEnable, DWORD dwFogMode)
{
    DebugLog("[HNH:SUB] SetFogMode_40E02D\n");

    BOOL bTableFog; // [esp+0h] [ebp-8h]

    g_dwFogMode_4B00C8 = dwFogMode; /*0x40e036*/
    bTableFog = bFogTableEnable && (g_d3dDevDesc_4D6980.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGTABLE) != 0; /*0x40e051*/
    g_dwFogTableEnable_4D6CF8 = bTableFog; /*0x40e064*/
    if (bTableFog) /*0x40e071*/
        return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGTABLEMODE, dwFogMode); /*0x40e08a*/
    else
        return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE, dwFogMode); /*0x40e0aa*/
}

void register_SetFogMode_40E02D() {
    LPVOID addr = (LPVOID)0x40E02D;
    LPVOID* orig = NULL;
    MH_STATUS res = MH_CreateHook(addr, reinterpret_cast<LPVOID>(SetFogMode_40E02D), reinterpret_cast<LPVOID*>(&orig));
    if (res == MH_OK) res = MH_EnableHook(addr);
    DebugLog("[HNH] Hooked SetFogMode_40E02D; %s\n", MH_StatusToString(res));
}
