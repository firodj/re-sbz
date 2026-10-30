#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// === GLOBAL VARIABLES ===
DWORD &g_dwFogEnable_4D6CF4      = *(DWORD *)0x4D6CF4; 
DWORD &g_dwFogRangeEnable_4D6CFC = *(DWORD *)0x4D6CFC;
DWORD &g_dwFogMode_4B00C8        = *(DWORD *)0x4B00C8;
DWORD &g_dwFogTableEnable_4D6CF8 = *(DWORD *)0x4D6CF8;
DWORD &g_dwFogColor_4B00CC       = *(DWORD*)0x4B00CC;
BOOL &g_bFogIsActive_4B3480      = *(BOOL*)0x4B3480;

// === GLOBAL FUNCTIONS ===
HRESULT (__cdecl * SetFogEnable_40DFB0)(BOOL bFogEnable, BOOL bFogRangeEnable) = reinterpret_cast<HRESULT(__cdecl*)(BOOL bFogEnable, BOOL bFogRangeEnable)>(0x40DFB0);
HRESULT (__cdecl * SetFogMode_40E02D)(int bFogTableEnable, DWORD dwFogMode) = reinterpret_cast<HRESULT(__cdecl*)(int bFogTableEnable, DWORD dwFogMode)>(0x40E02D);
HRESULT (__cdecl * SetFogColor_40E0B4)(DWORD dwFogColor) = reinterpret_cast<HRESULT(__cdecl*)(DWORD dwFogColor)>(0x40E0B4);
HRESULT (__cdecl * SetFogRange_40E0E2)(DWORD dwFogStart, DWORD dwFogEnd) = reinterpret_cast<HRESULT(__cdecl*)(DWORD dwFogStart, DWORD dwFogEnd)>(0x40E0E2);

// === FUNCTION IMPLEMENTATIONS ===
/**
 * Enables or disables fog (and optionally range-based fog) on the D3D7 device.
 * @param bFogEnable FALSE disables fog; any other value enables it.
 * @param bFogRangeEnable Range fog (eye-relative distance) request; only honored
 *                        when the device advertises D3DPRASTERCAPS_FOGRANGE.
 * @return HRESULT of the last SetRenderState call performed.
 */
HRESULT __cdecl hook_SetFogEnable_40DFB0(BOOL bFogEnable, BOOL bFogRangeEnable)
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

/**
* Selects the fog mode (D3DFOG_*) as table (pixel) fog or vertex fog.
* @param bFogTableEnable Requests pixel/table fog; only honored when the device
*                        advertises D3DPRASTERCAPS_FOGTABLE, otherwise vertex
*                        fog is used.
* @param dwFogMode D3DFOG_NONE / D3DFOG_EXP / D3DFOG_EXP2 / D3DFOG_LINEAR.
* @return HRESULT of the SetRenderState call performed.
*/
HRESULT __cdecl hook_SetFogMode_40E02D(int bFogTableEnable, DWORD dwFogMode)
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

HRESULT __cdecl hook_SetFogColor_40E0B4(DWORD dwFogColor)
{
  DebugLog("[HNH:SUB] SetFogColor_40E0B4\n");

  g_dwFogColor_4B00CC = dwFogColor;
  return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGCOLOR, dwFogColor);
}

HRESULT __cdecl hook_SetFogRange_40E0E2(DWORD dwFogStart, DWORD dwFogEnd)
{
  HRESULT hr; // [esp+0h] [ebp-4h]

  DebugLog("[HNH:SUB] SetFogRange_40E0E2\n");

  hr = 0;
  if ( g_dwFogMode_4B00C8 == 3 )
  {
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGSTART, dwFogStart);
    return g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_FOGEND, dwFogEnd);
  }
  return hr;
}

void register_SetFogEnable_40DFB0() {
  hnh_HookOff();
  hnh_Hook(0x40DFB0, hook_SetFogEnable_40DFB0, "SetFogEnable_40DFB0"); // [OK] 
  hnh_Hook(0x40E02D, hook_SetFogMode_40E02D, "SetFogMode_40E02D"); // [OK] 
  hnh_Hook(0x40E0B4, hook_SetFogColor_40E0B4, "SetFogColor_40E0B4"); // [OK] 
  hnh_Hook(0x40E0E2, hook_SetFogRange_40E0E2, "SetFogRange_40E0E2"); // [OK] 
  hnh_HookOn();
}
