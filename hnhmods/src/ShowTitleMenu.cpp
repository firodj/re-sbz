#include "dx7.hpp"
#include "console.hpp"
#include "shared.hpp"


// === GLOBAL VARIABLES ===

CDisplayConfig &g_displayConfig_518C30 = *(CDisplayConfig *)0x518C30; // WRITABLE
uint32_t &g_dwShadowOff = *(uint32_t *)0x4B00B0; // WRITABLE
D3DXMATRIX& g_pM2 = *(D3DXMATRIX*)0x4CE918;
CD3DInitParam& g_D3DDevParams = *(CD3DInitParam*)0x4CE8B0;
uint32_t &g_bNonShowMenu_4D6C70 = *(uint32_t *)0x4D6C70; // WRITABLE
uint32_t &g_pCurMatObj = *(uint32_t *)0x4D6CA4; // WRITABLE
uint32_t &g_bCreateFlags_4D6CD4 = *(uint32_t *)0x4D6CD4; // WRITABLE
HWND &g_hWndDev_4D6C74 = *(HWND *)0x4D6C74; // WRITABLE
uint32_t &g_nObjCount = *(uint32_t *)0x4CE868; // WRITABLE
uint32_t &g_nCapObjects_4CD780 = *(uint32_t *)0x4CD780; // WRITABLE
D3DX_SURFACEFORMAT &g_dwSurfaceFormat_4B00B4 = *(D3DX_SURFACEFORMAT *)0x4B00B4; // WRITABLE
uint32_t &g_nColorBits_518C2C = *(uint32_t *)0x518C2C; // WRITABLE
unsigned int &g_nBitness = *(unsigned int *)0x518C88; // WRITABLE
uint32_t &g_nZBufDepthSelValue_518C98 = *(uint32_t *)0x518C98; // WRITABLE
int &g_nModeSetIndex = *(int *)0x518C90; // WRITABLE
uint32_t &g_nWidth = *(uint32_t *)0x518C94; // WRITABLE
uint32_t &g_nHeight = *(uint32_t *)0x518C8C; // WRITABLE
uint32_t &g_nHeight2 = *(uint32_t *)0x518C84; // WRITABLE
uint32_t &g_nDeviceCount = *(uint32_t *)0x4D6CC8; // WRITABLE
D3DX_DEVICEDESC * &g_pDevicesDesc = *(D3DX_DEVICEDESC**)0x4D6CCC; // WRITABLE

// === GLOBAL FUNCTIONS ===

BOOL(__cdecl *ShowTitleMenu_427DA8)(HWND hWnd, BOOL bFullScreen) = (BOOL(__cdecl*)(HWND, BOOL))0x427DA8;
void(__cdecl* DisplayConfig_Destroy)(CDisplayConfig* pDisplayConfig) = reinterpret_cast<void (__cdecl * )(CDisplayConfig *)>(0x427CC9);
void (*D3DSetRenderStates_4276C0)(void) = reinterpret_cast<void(*)()>(0x4276C0);
int (__cdecl*D3DInit_40A894)(CD3DInitParam *pDevParam) = reinterpret_cast<int(__cdecl*)(CD3DInitParam *)>(0x40A894);
void (*AllocateDevicesDescArray)(void) = reinterpret_cast<void(*)(void)>(0x40AEFB);

// === FUNCTION IMPLEMENTATIONS ===

void hook_AllocateDevicesDescArray()
{
  unsigned int i; // [esp+0h] [ebp-4h]

  DebugLog("[HNH:SUB] %s\n", "AllocateDevicesDescArray_40AEFB");

  g_nDeviceCount = D3DXGetDeviceCount();
  g_pDevicesDesc = (D3DX_DEVICEDESC*)MemAlloc_40DEF7(sizeof(D3DX_DEVICEDESC) * g_nDeviceCount);
  for ( i = 0; i < g_nDeviceCount; ++i )
    D3DXGetDeviceDescription(i, &g_pDevicesDesc[i]);
}

// ShowTitleMenu_427DA8
// Displays the title menu and initializes Direct3D for the game
// Returns TRUE if game should start, FALSE otherwise
BOOL __cdecl hook_ShowTitleMenu_427DA8(HWND hWnd, BOOL bFullScreen)
{
    HINSTANCE hInstance;
    HWND hDesktopWindow;
    DWORD dwDepthBitsTable[10];
    int i;
    int hr;
    RECT rect;
    int bRunGame;
    CD3DInitParam d3dParams;

    DebugLog("[HNH:SUB] %s\n", "ShowTitleMenu_427DA8");

    // Clear and initialize display configuration
    memset(&g_displayConfig_518C30, 0, sizeof(g_displayConfig_518C30));
    DisplayConfig_Init(&g_displayConfig_518C30);

    // Get module instance and show title menu window
    hInstance = (HINSTANCE)GetWindowLongA(hWnd, GWL_HINSTANCE);
    bRunGame = ShowTitleMenuWnd_428927(hInstance);

    // Resize window to full desktop
    hDesktopWindow = GetDesktopWindow();
    GetWindowRect(hDesktopWindow, &rect);
    MoveWindow(hWnd, 0, 0, rect.right - rect.left, rect.bottom - rect.top, TRUE);
    UpdateWindow(hWnd);

    // Cleanup display config
    DisplayConfig_Destroy(&g_displayConfig_518C30);

    // User cancelled menu
    if (!bRunGame)
        return FALSE;

    // Initialize D3D parameters
    memset(&d3dParams, 0, sizeof(d3dParams));

    if (bFullScreen)
    {
        // Fullscreen mode
        d3dParams.dwCreationFlags = 1;           // Fullscreen flag
        d3dParams.hWndFocus = hWnd;
        g_nColorBits_518C2C = g_nBitness;        // Set color depth
        d3dParams.nNumColorBits = g_nBitness;
        g_nZBufDepthSelValue_518C98 = 0;
    }
    else
    {
        // Windowed mode
        d3dParams.dwCreationFlags = 0;
        d3dParams.hWndFocus = (HWND)0x2;         // HWND_MESSAGE | 0x2
        d3dParams.nNumColorBits = -1;            // Use desktop color depth
        d3dParams.nNumDepthBits = -1;            // Use desktop depth buffer
    }

    // Common D3D parameters
    g_nRenderHALMode_518CA0 = g_nModeSetIndex;
    d3dParams.nDeviceIndex = g_nModeSetIndex;
    d3dParams.hWnd = hWnd;
    d3dParams.nNumAlphaBits = -1;
    d3dParams.nNumStencilBits = 0;
    d3dParams.nNumBackBuffers = 1;
    g_nWidth2 = g_nWidth;
    d3dParams.nWidth = g_nWidth;
    g_nHeight2 = g_nHeight;
    d3dParams.nHeight = g_nHeight;
    d3dParams.nRefreshRate = -1;                 // Default refresh rate
    d3dParams.bUseCreateEx = 1;

    // Depth buffer bit depths to try (16, 24, 32, then default)
    dwDepthBitsTable[1] = 16;
    dwDepthBitsTable[2] = 24;
    dwDepthBitsTable[3] = 32;
    dwDepthBitsTable[4] = -1;
    memset(&dwDepthBitsTable[5], 0, 20);         // Clear remaining entries
    dwDepthBitsTable[0] = g_nBitness;            // First try current bitness

    // Try initializing D3D with different depth buffer formats
    for (i = 0; i < 5; ++i)
    {
        d3dParams.nNumDepthBits = dwDepthBitsTable[i];
        hr = D3DInit_40A894(&d3dParams);
        if (hr >= 0)
        {
            D3DSetRenderStates_4276C0();         // Setup render states
            return TRUE;                          // Success
        }
    }

    return FALSE;                                // All attempts failed
}

void __cdecl hook_DisplayConfig_Destroy(CDisplayConfig *pDisplayConfig)
{
  DebugLog("[HNH:SUB] DisplayConfig_Destroy_427CC9\n");

  if (!pDisplayConfig) return;
  
  if ( pDisplayConfig->parrVidModeDesc )
  {
    if ( pDisplayConfig->parrVidModeDesc->pModeDesc )
      MemFree_40DF64(pDisplayConfig->parrVidModeDesc->pModeDesc);
    if ( pDisplayConfig->parrVidModeDesc->pD3DCaps )
      MemFree_40DF64(pDisplayConfig->parrVidModeDesc->pD3DCaps);
    if ( pDisplayConfig->parrVidModeDesc->pDDHALCaps )
      MemFree_40DF64(pDisplayConfig->parrVidModeDesc->pDDHALCaps);
    if ( pDisplayConfig->parrVidModeDesc->pDDHELCaps )
      MemFree_40DF64(pDisplayConfig->parrVidModeDesc->pDDHELCaps);
    if ( pDisplayConfig->parrVidModeDesc->parrbActive )
      MemFree_40DF64(pDisplayConfig->parrVidModeDesc->parrbActive);
    MemFree_40DF64(pDisplayConfig->parrVidModeDesc);
  }
  if ( pDisplayConfig->parrD3DXDevDesc )
    MemFree_40DF64(pDisplayConfig->parrD3DXDevDesc);

  D3DXUninitialize();
}

void hook_D3DSetRenderStates_4276C0()
{
  D3DDEVICEDESC7 d3dDevCaps_v1; // [esp+0h] [ebp-ECh] BYREF

  DebugLog("[HNH:SUB] D3DSetRenderStates_4276C0\n");

  g_pD3DDevice_4D6C78->GetCaps(&d3dDevCaps_v1);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SHADEMODE, 2);
  g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_LIGHTING, 1);
  if ( g_bSpecularEnable_4B3474 == 1 )
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, 1);
  else
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, 0);
  g_nLightningEnabled = g_bLightEnable_4B3484 == 1;
  g_dwShadowOff = g_bShadowEnable_518CA8 != 1;
  if ( g_bAntiAliasEnable == 1 )
  {
    if ( g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 2) != S_OK )
      g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MINFILTER  , 2);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MAGFILTER , 2);
  }
  else
  {
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MINFILTER  , 2);
    g_pD3DDevice_4D6C78->SetTextureStageState(0, D3DTSS_MAGFILTER , 2);
  }
  if ( g_bDitherEnable == 1 )
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DITHERENABLE, 1);
  else
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DITHERENABLE, 0);
  g_bForceDirectionalLights = (d3dDevCaps_v1.dwVertexProcessingCaps & 0x10) == 0;
}

int __cdecl hook_D3DInit_40A894(CD3DInitParam *pDevParam)
{
  //int result; // eax
  int is_r5g5b5; // [esp+18h] [ebp-1Ch]
  unsigned int cur_surface_fmt; // [esp+1Ch] [ebp-18h]
  int hr; // [esp+20h] [ebp-14h]
  int hr2; // [esp+20h] [ebp-14h]
  int max_surface_fmts; // [esp+24h] [ebp-10h]
  int is_r8g8b8; // [esp+2Ch] [ebp-8h]
  int is_r5g6b5; // [esp+30h] [ebp-4h]

  DebugLog("[HNH:SUB] D3DInit_40A894\n");

  g_nMemCnt_4D6CF0 = 0;

  *&g_pM2.m[3][1] = 0;
  *&g_pM2.m[2][3] = 0;
  *&g_pM2.m[2][0] = 0;
  *&g_pM2.m[1][2] = 0;
  *&g_pM2.m[0][3] = 0;
  *&g_pM2.m[0][1] = 0;
  g_pM2._44 = 1.0;
  g_pM2._33 = 1.0;
  g_pM2._22 = 1.0;
  g_pM2._11 = 1.0;

  if ( pDevParam != &g_D3DDevParams )
    qmemcpy(&g_D3DDevParams, pDevParam, sizeof(g_D3DDevParams));
  g_nObjectTableCapacity_4B009C = 256;
  g_nObjectTableCount = 1;
  g_pObjectTable = 0;
  g_bInited_4D6C6C = 0;
  g_pD3DDevice_4D6C78 = 0;
  g_pRenderCtx = 0;
  g_pDD_4D6C7C = 0;
  g_pD3D_4D6C84 = 0;
  g_bNonShowMenu_4D6C70 = 1;
  g_bitsLightIndexes_4D6CB8 = 0;
  g_pCurTexObj = 0;
  g_pCurMatObj = 0;
  g_bAlphaBlendEnabled_4D6CC0 = 0;
  g_nAlphaOpState_4D6CC4 = 0;
  g_bCreateFlags_4D6CD4 = pDevParam->dwCreationFlags == 1;
  g_hWndDev_4D6C74 = pDevParam->hWnd;
  memset(&g_d3dDevDesc_4D6980, 0, sizeof(g_d3dDevDesc_4D6980));
  hr = D3DXInitialize();
  if ( hr < 0 )
    return hr;
  AllocateDevicesDescArray();
  if ( pDevParam->bUseCreateEx )
    hr2 = D3DXCreateContextEx(
      pDevParam->nDeviceIndex,
      pDevParam->dwCreationFlags,
      pDevParam->hWnd,
      pDevParam->hWndFocus,
      pDevParam->nNumColorBits,
      pDevParam->nNumAlphaBits,
      pDevParam->nNumDepthBits,
      pDevParam->nNumStencilBits,
      pDevParam->nNumBackBuffers,
      pDevParam->nWidth,
      pDevParam->nHeight,
      pDevParam->nRefreshRate,
      &g_pRenderCtx);
  else
    hr2 = D3DXCreateContext(
      pDevParam->nDeviceIndex,
      pDevParam->dwCreationFlags,
      pDevParam->hWnd,
      pDevParam->nNumColorBits,
      pDevParam->nNumDepthBits,
      &g_pRenderCtx);
  if ( hr2 >= 0 )
  {
    memset(&g_d3dDevDesc_4D6980, 0, sizeof(g_d3dDevDesc_4D6980));
    g_pObjectTable = (CObjItemRef_S208*) MemAlloc_40DEF7(sizeof(CObjItemRef_S208) * g_nObjectTableCapacity_4B009C);
    if ( g_pObjectTable )
    {
      memset(g_pObjectTable, 0, 520 * g_nObjectTableCapacity_4B009C);
      g_pObjectTable->szName_0[0] = '%';
      g_nObjCount = 0;
      g_nCapObjects_4CD780 = 1024;
      g_pObjPool = (CObjPoolMgr_S58*) MemAlloc_40DEF7(0x16000u);
      g_bInited_4D6C6C = 1;
      max_surface_fmts =  (-1, 0, 4);
      cur_surface_fmt = 0;
      is_r5g5b5 = 0;
      is_r5g6b5 = 0;
      is_r8g8b8 = 0;
      do
      {
        D3DXGetSurfaceFormat(-1, 0, 4u, cur_surface_fmt, &g_dwSurfaceFormat_4B00B4);
        if ( g_dwSurfaceFormat_4B00B4 == D3DX_SF_R5G5B5 )
          is_r5g5b5 = 1;
        if ( g_dwSurfaceFormat_4B00B4 == D3DX_SF_R5G6B5 )
          is_r5g6b5 = 1;
        if ( g_dwSurfaceFormat_4B00B4 == D3DX_SF_R8G8B8 )
          is_r8g8b8 = 1;
        ++cur_surface_fmt;
      }
      while ( cur_surface_fmt != max_surface_fmts );
      if ( is_r8g8b8 )
      {
        g_dwSurfaceFormat_4B00B4 = D3DX_SF_R8G8B8;
      }
      else if ( is_r5g5b5 )
      {
        g_dwSurfaceFormat_4B00B4 = D3DX_SF_R5G5B5;
      }
      else if ( is_r5g6b5 )
      {
        g_dwSurfaceFormat_4B00B4 = D3DX_SF_R5G6B5;
      }
      else
      {
        g_dwSurfaceFormat_4B00B4 = D3DX_SF_A4R4G4B4;
      }
      return D3DInitBuffers_40AF60();
    }
    else
    {
      D3DDeinit();
      return DDERR_GENERIC;
    }
  } else {
    D3DDeinit();
    return hr2;
  }
}

// Register hook if needed
void register_ShowTitleMenu_427DA8() {
    hnh_Hook(0x427DA8, hook_ShowTitleMenu_427DA8, "ShowTitleMenu_427DA8");
    hnh_Hook(0x40A894, hook_D3DInit_40A894, "D3DInit_40A894");
    hnh_Hook(0x427CC9, hook_DisplayConfig_Destroy, "DisplayConfig_Destroy_427CC9");
    hnh_Hook(0x4276C0, hook_D3DSetRenderStates_4276C0, "D3DSetRenderStates_4276C0");
    hnh_Hook(0x40AEFB, hook_AllocateDevicesDescArray, "AllocateDevicesDescArray_40AEFB");
}