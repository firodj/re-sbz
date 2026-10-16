#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// Global variables
LPDIRECT3DDEVICE7 &g_pD3DDevice_4D6C78 = *(LPDIRECT3DDEVICE7*)0x4D6C78;
D3DDEVICEDESC7 g_d3dDevDesc_4D6980 = *(D3DDEVICEDESC7 *)0x4D6980;

DWORD &g_dwFogEnable_4D6CF4      = *(DWORD *)0x4D6CF4; 
DWORD &g_dwFogRangeEnable_4D6CFC = *(DWORD *)0x4D6CFC;
DWORD &g_dwFogMode_4B00C8        = *(DWORD *)0x4B00C8;
DWORD &g_dwFogTableEnable_4D6CF8 = *(DWORD *)0x4D6CF8;
DWORD& g_dwFogColor_4B00CC = *(DWORD*)0x4B00CC;

/**
 * Enables or disables fog (and optionally range-based fog) on the D3D7 device.
 * @param bFogEnable FALSE disables fog; any other value enables it.
 * @param bFogRangeEnable Range fog (eye-relative distance) request; only honored
 *                        when the device advertises D3DPRASTERCAPS_FOGRANGE.
 * @return HRESULT of the last SetRenderState call performed.
 */
HRESULT __cdecl SetFogEnable_40DFB0(DWORD bFogEnable, int bFogRangeEnable)
{
    DebugLog("[HNH:SUB] SetFogEnable_40DFB0\n");

    HRESULT hr; // [esp+0h] [ebp-4h]

    g_dwFogEnable_4D6CF4 = bFogEnable; /*0x40dfbe*/
    hr = g_pD3DDevice_4D6C78->SetRenderState( D3DRENDERSTATE_FOGENABLE  , bFogEnable); /*0x40dfde*/
    if (g_d3dDevDesc_4D6980.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_FOGRANGE) /*0x40dfef*/
    {
        g_dwFogRangeEnable_4D6CFC = bFogRangeEnable; /*0x40dff4*/
        if (bFogRangeEnable) /*0x40dffd*/
        {
            if (g_dwFogEnable_4D6CF4) /*0x40e006*/
                return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, g_dwFogEnable_4D6CF4); /*0x40e023*/
        }
    }
    return hr; /*0x40e029*/
}

void register_SetFogEnable_40DFB0() {
    LPVOID addr = (LPVOID)0x40DFB0;
    LPVOID* orig = NULL;
    MH_STATUS res = MH_CreateHook(addr, reinterpret_cast<LPVOID>(SetFogEnable_40DFB0), reinterpret_cast<LPVOID*>(&orig));
    if (res == MH_OK) res = MH_EnableHook(addr);
    DebugLog("[HNH] Hooked SetFogEnable_40DFB0; %s\n", MH_StatusToString(res));
}
