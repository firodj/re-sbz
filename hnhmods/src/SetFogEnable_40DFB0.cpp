#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"


#include <d3d.h>

//enum
//{
//    D3DRENDERSTATE_FOGENABLE      = 28, // 0x1C (d3dtypes.h)
//    D3DRENDERSTATE_RANGEFOGENABLE = 48, // 0x30 (d3dtypes.h)
//};

//static const DWORD D3DPRASTERCAPS_FOGRANGE = 0x00010000; // (d3dcaps.h)

IDirect3DDevice7 *&g_pD3DDevice_4D6C78 = *(IDirect3DDevice7 **)0x4D6C78;
// d3dDevDesc_4D6980 (D3DDEVICEDESC7) .dpcTriCaps.dwRasterCaps -> 0x4D6980 + 0x44

D3DDEVICEDESC7 g_d3dDevDesc_4D6980 = *(D3DDEVICEDESC7 *)0x4D6980;

// NOTE: the IDA name is misleading -- 4D6CF4 actually mirrors the FOGENABLE state,
// while 4D6CFC (unnamed in IDA) mirrors the RANGEFOGENABLE request.
DWORD &g_dwFogEnable_4D6CF4      = *(DWORD *)0x4D6CF4; // WRITABLE
DWORD &g_dwFogRangeEnable_4D6CFC = *(DWORD *)0x4D6CFC; // WRITABLE

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
                return  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_RANGEFOGENABLE, g_dwFogEnable_4D6CF4); /*0x40e023*/
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
