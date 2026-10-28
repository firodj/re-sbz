#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// InGameMenu_4799E2 / MainMenu_47DF0D -- the two menu shells around the scene
// chapters (both called only from GameMainLoop_47F013).
//
// InGameMenu is the four-option title menu over Title.bmp/TitleBt.bmp:
//   0 Start from Beginning -> scene step := 1, returns 0
//   1 Continue             -> SystemMenu_4655AB (load/config); -1 or 2 there
//                             returns 0 here, other values fall back to idle
//   2 (locked while g_sceneProgressFlag_530269 == 0) -> returns 999, which
//                             GameMainLoop maps to MainMenu + op.avi replay
//   3 End                  -> returns 1 = quit
// It keeps rendering while its BGM (s001.wav) plays; when the BGM stops it
// exits with -1 (GameMainLoop then replays op.avi). Option rows are hit-tested
// from the mouse; row 2 unlocks after the backdrop/ending was seen.
//
// MainMenu is the 3-state extra menu entered via option 2 (mouse.bmp cursor +
// s004.wav BGM): -3 character select -> -2 gallery -> -1 music/sound options;
// any state >= 0 exits back to InGameMenu.
//
// Menu BGM volume = g_fVolumeDbTable[g_nAviVolumeIndex_530266] - 1000, clamped
// to [-10000, 0] dB (note: unlike the UI effects, these buffers CAN be
// volume-controlled -- LoadWav_422E7B creates them without DSBCAPS_CTRLVOLUME,
// so the clamp silently fails too; kept faithful).

// === PINNED GAME FUNCTIONS ===
void  (__cdecl*LoadBmpWithMask_413A5E)(CHanaSpriteT3F6_S1FC **ppOut, LPCSTR szBmpName, LPCSTR szMaskName) =
    reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC **, LPCSTR, LPCSTR)>(0x413A5E);
void(__cdecl* GetSurfaceWH)(LPDIRECTDRAWSURFACE7 pSurface, LPDWORD pWidth, LPDWORD pHeight)
= reinterpret_cast<void(__cdecl *)(LPDIRECTDRAWSURFACE7 pSurface, LPDWORD pWidth, LPDWORD pHeight)>(0x419A1A);
void (__cdecl*DSBufferPlay)(LPDIRECTSOUNDBUFFER pDSBuf, int dwPlayFlags) =
    reinterpret_cast<void(__cdecl*)(LPDIRECTSOUNDBUFFER, int)>(0x423BAF);
BOOL (__cdecl*DoPeekMessages)(void) = reinterpret_cast<BOOL(__cdecl*)()>(0x481550);
LPDIRECTDRAWSURFACE7(__cdecl*CreateTextureFromSurfaceWithAlphaMask)(
  LPDIRECTDRAWSURFACE7 surface,
  LPDIRECTDRAWSURFACE7 maskSurface)
  = reinterpret_cast<LPDIRECTDRAWSURFACE7(__cdecl*)(
    LPDIRECTDRAWSURFACE7 surface,
    LPDIRECTDRAWSURFACE7 maskSurface)>(0x43EA05);

int  (__cdecl*GetMouseBtn_45A41B)(WORD *pBtnMouse) =
    reinterpret_cast<int(__cdecl*)(WORD *)>(0x45A41B);
void (__cdecl*GetMouseXY_45A4D8)(DWORD *pXmouse, DWORD *pYmouse) =
    reinterpret_cast<void(__cdecl*)(DWORD *, DWORD *)>(0x45A4D8);
int  (__cdecl*MenuMusicSelect)(LPDIRECTSOUNDBUFFER *pDSndBuf, CHanaSpriteT3F6_S1FC *pSprite) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER *, CHanaSpriteT3F6_S1FC *)>(0x47D163);
int  (__cdecl*MenuGallery)(LPDIRECTSOUNDBUFFER *pDSndBuf, CHanaSpriteT3F6_S1FC *pSprite) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER *, CHanaSpriteT3F6_S1FC *)>(0x47AD8D);
int  (__cdecl*MenuCharSelect)(LPDIRECTSOUNDBUFFER pDSndBuf, CHanaSpriteT3F6_S1FC *pSprite) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER, CHanaSpriteT3F6_S1FC *)>(0x47A324);
struct IDirectDrawSurface7 *(__cdecl*PlayScreenFade)(int bisGo, COLORREF colorref,
    struct IDirectDrawSurface7 *pDdSurface, float fFade, char bFlags) =
    reinterpret_cast<struct IDirectDrawSurface7 *(__cdecl*)(int, COLORREF, struct IDirectDrawSurface7 *, float, char)>(0x4874DE);
int  (__cdecl*BltSurface)(LPDIRECTDRAWSURFACE7 pSurface, int left, int top, int right, int bottom, int fillColor) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTDRAWSURFACE7, int, int, int, int, int)>(0x419D89);
LPDIRECTDRAWSURFACE7(__cdecl*LoadBmpScaled_4203B3)(LPCSTR lpFileName, int xRight, int yBottom) =
    reinterpret_cast<LPDIRECTDRAWSURFACE7(__cdecl*)(LPCSTR, int, int)>(0x4203B3);
int  (__cdecl*FlushD3DAndTouchSurface)(LPDIRECTDRAWSURFACE7 pSurface) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTDRAWSURFACE7)>(0x43BF6F);
void(__cdecl* SetPos_414A5D)(CHanaSpriteT3F6_S1FC *pSprite, float fX, float fY) =
    reinterpret_cast<void(__cdecl*)(CHanaSpriteT3F6_S1FC *, float, float)>(0x414A5D);
int  (__cdecl*DrawSprite2D)(CHanaSpriteT3F6_S1FC *pSprite) =
    reinterpret_cast<int(__cdecl*)(CHanaSpriteT3F6_S1FC *)>(0x414191);
//void (__cdecl*nop_4863C3)(void) = reinterpret_cast<void(__cdecl*)()>(0x4863C3);
void (__cdecl*PresentFrame)(void) = reinterpret_cast<void(__cdecl*)()>(0x40BF01);
HRESULT(__cdecl*HanaVert_DrawBatch)(int nParam) = reinterpret_cast<HRESULT(__cdecl*)(int)>(0x40CEEC);
int  (__cdecl*SystemMenu_4655AB)(LPDIRECTSOUNDBUFFER pBgm, int nVolOffset, int nMenuMode,
    float fProgA, float fProgB, CSceneMenuState_9C pSceneFlags) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTSOUNDBUFFER, int, int, float, float, CSceneMenuState_9C)>(0x4655AB);

void (__cdecl*MainMenu_47DF0D)(void) = reinterpret_cast<void(__cdecl*)()>(0x47DF0D);
int  (__cdecl*InGameMenu_4799E2)(void) = reinterpret_cast<int(__cdecl*)()>(0x4799E2);

// === PINNED GLOBALS ===
float (&g_fVolumeDbTable)[10] = *(float(*)[10])0x4C6F4C;   // READ-ONLY
int   (&g_Src)[10]            = *(int(*)[10])0x530238;     // WRITABLE
DWORD &g_dwDSStatus_5185C8    = *(DWORD*)0x5185C8;         // WRITABLE

// clamp g_fVolumeDbTable[g_nAviVolumeIndex_530266] - 1000 to [-10000, 0] dB
static void ApplyMenuBgmVolume(LPDIRECTSOUNDBUFFER pBgmBuffer)
{
    int nVol = (int)(g_fVolumeDbTable[g_nAviVolumeIndex_530266] - 1000.0f);
    if (nVol < -10000)
        nVol = -10000;
    else if (nVol > 0)
        nVol = 0;
    pBgmBuffer->SetVolume(nVol);
}

// === FUNCTIONS ===
void __cdecl hook_MainMenu_47DF0D(void)
{
    CHAR szFileName[256];                  // [esp+4h] [ebp-11Ch]
    CHanaSpriteT3F6_S1FC *pMouseSprite;    // [esp+114h] [ebp-Ch]
    LPDIRECTSOUNDBUFFER pBgmBuffer;        // [esp+108h] [ebp-18h]
    int nMenuState, xMouse, yMouse;        // [ebp-120h] / [ebp-8h] / [ebp-10h]
    WORD nMouseBtn;                        // [esp+11Ch] [ebp-4h]

    DebugLog("[HNH:SUB] MainMenu_47DF0D\n");

    nMenuState = -3;                       // entry state: character select
    pMouseSprite = NULL;
    pBgmBuffer = NULL;

    wsprintfA(szFileName, "mouse.bmp");
    CHanaObj__GetRealPath_486D9A(szFileName, 1003);         // 1003 = SPR dir
    LoadBmpWithMask_413A5E(&pMouseSprite, szFileName, szFileName);

    wsprintfA(szFileName, "s004.wav");
    CHanaObj__GetRealPath_486D9A(szFileName, 1006);         // 1006 = WAV dir
    pBgmBuffer = LoadWav_422E7B(szFileName);
    if (pBgmBuffer)
    {
        ApplyMenuBgmVolume(pBgmBuffer);
        DSBufferPlay(pBgmBuffer, 1);                         // loop BGM
    }

    // carry the persistent scene flags into the scene state block
    memcpy(g_sceneState_52F7E0.menuState.pSceneFlags, g_Src, 0x28);

    // dispatch until a sub-menu answers with a non-negative state; the mouse
    // is pumped each round (the previous-button read in the original is dead)
    for (;;)
    {
        DoPeekMessages();
        GetMouseBtn_45A41B(&nMouseBtn);
        GetMouseXY_45A4D8((DWORD *)&xMouse, (DWORD *)&yMouse);

        if (nMenuState == -1)
            nMenuState = MenuMusicSelect(&pBgmBuffer, pMouseSprite);
        else if (nMenuState == -2)
            nMenuState = MenuGallery(&pBgmBuffer, pMouseSprite);
        else if (nMenuState == -3)
            nMenuState = MenuCharSelect(pBgmBuffer, pMouseSprite);
        else
            break;
    }

    if (pMouseSprite)
    {
        CHanaObj__Free_40C1E4(&pMouseSprite->base_0);
        pMouseSprite = NULL;
    }
    if (pBgmBuffer)
    {
        pBgmBuffer->Stop();
        if (pBgmBuffer)                                     // original re-tests
        {
            pBgmBuffer->Stop();
            pBgmBuffer->Release();
            pBgmBuffer = NULL;
        }
    }
    g_sceneState_52F7E0.menuState.bActiveSceneStep = 0;
    memset(g_sceneState_52F7E0.menuState.pSceneFlags, 0, 0x28);
    if (nMenuState < 0)                                     // exited via sub-menu
        PlayScreenFade(0, 0, g_pPrimaryBuffer_4D6C88, 25.0f, 0);
}

int __cdecl hook_InGameMenu_4799E2(void)
{
    CHAR szFileName[256];                  // [esp+D0h] [ebp-174h]
    LPDIRECTDRAWSURFACE7 pTitleSurface;    // [esp+C8h] [ebp-17Ch]
    LPDIRECTDRAWSURFACE7 pButtonStrip;     // [esp+CCh] [ebp-178h]
    CHanaSpriteT3F6_S1FC *pMouseSprite;    // [esp+234h] [ebp-10h]
    LPDIRECTSOUNDBUFFER pBgmBuffer;        // [esp+1D4h] [ebp-70h]
    RECT arrHoverRects[5];                 // [esp+1E4h] [ebp-60h] TitleBt frames
    CSceneState sceneScratch;              // [esp-ECh] [ebp-330h] SystemMenu copy
    DWORD dwAntiAliasSaved, dwAntiAliasCur;
    int nMenuSelected, i;
    int mouseX, mouseY;
    WORD nMouseBtn = 0, nPrevMouseBtn;

    DebugLog("[HNH:SUB] InGameMenu_4799E2\n");

    nMenuSelected = 0;
    pMouseSprite = NULL;
    pBgmBuffer = NULL;

    // hover-highlight source rects into the TitleBt button strip (frame 4 = 0)
    arrHoverRects[0] = { 0, 0,                                      (int)ConvertFrom640AsFloat(200.0f) - 1,  ConvertFrom1024AsInt(42)  };
    arrHoverRects[1] = { 0, (int)ConvertFrom640AsFloat(26.25f) + 1, (int)ConvertFrom640AsFloat(200.0f) - 1,  ConvertFrom1024AsInt(85)  };
    arrHoverRects[2] = { 0, (int)ConvertFrom640AsFloat(52.5f) + 1,  (int)ConvertFrom640AsFloat(200.0f) - 1,  ConvertFrom1024AsInt(127) };
    arrHoverRects[3] = { 0, (int)ConvertFrom640AsFloat(78.75f) + 1, (int)ConvertFrom640AsFloat(200.0f) - 1,  ConvertFrom1024AsInt(168) };
    arrHoverRects[4] = { 0, 0, 0, 0 };

    // background + button strip, both scaled to the current mode
    wsprintfA(szFileName, "Title.bmp");                     // aTitleBmp @0x4C6CA4
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);         // 1007 = GRP dir
    pTitleSurface = LoadBmpScaled_4203B3(szFileName, ConvertFrom640AsInt(640), ConvertFrom640AsInt(480));
    wsprintfA(szFileName, "TitleBt.bmp");                   // aTitlebtBmp @0x4C6CB0
    CHanaObj__GetRealPath_486D9A(szFileName, 1007);
    pButtonStrip = LoadBmpScaled_4203B3(szFileName, (int)ConvertFrom640AsFloat(200.0f), (int)ConvertFrom640AsFloat(105.0f));

    // fresh game: splash the lower-right area of the title art
    if (!g_sceneProgressFlag_530269 && pTitleSurface)
        BltSurface(pTitleSurface, ConvertFrom1024AsInt(600), ConvertFrom1024AsInt(470),
                   ConvertFrom1024AsInt(960), ConvertFrom1024AsInt(525), 0);

    wsprintfA(szFileName, "mouse.bmp");                     // aMouseBmp_2 @0x4C6CBC
    CHanaObj__GetRealPath_486D9A(szFileName, 1003);         // 1003 = SPR dir
    LoadBmpWithMask_413A5E(&pMouseSprite, szFileName, szFileName);

    wsprintfA(szFileName, "s001.wav");                      // aS001Wav @0x4C6CC8
    CHanaObj__GetRealPath_486D9A(szFileName, 1006);         // 1006 = WAV dir
    pBgmBuffer = LoadWav_422E7B(szFileName);
    if (pBgmBuffer)
    {
        ApplyMenuBgmVolume(pBgmBuffer);
        DSBufferPlay(pBgmBuffer, 0);
    }

    g_sceneState_52F7E0.menuState.bActiveSceneStep = 0;
    PlayScreenFade(1, 0xFF000000, pTitleSurface, 50.0f, 0); // fade in
    StopAndRestartThread(2);
    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasSaved);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);

    for (;;)                                 // title menu loop
    {
        DoPeekMessages();
        nPrevMouseBtn = nMouseBtn;
        GetMouseBtn_45A41B(&nMouseBtn);
        GetMouseXY_45A4D8((DWORD *)&mouseX, (DWORD *)&mouseY);

        // hit-test the 4 option rows (x 381.25..581.25, row i y 60i+360..60i+402,
        // design space scaled by 0.625 then converted); row 2 stays locked
        // until the backdrop/ending was seen
        nMenuSelected = -1;
        for (i = 0; i < 4; ++i)
        {
            if ((int)ConvertFrom640AsFloat((60 * i + 360) * 0.625f) < mouseY
                && mouseY < (int)ConvertFrom640AsFloat((60 * i + 402) * 0.625f)
                && (int)ConvertFrom640AsFloat(381.25f) < mouseX
                && mouseX < (int)ConvertFrom640AsFloat(581.25f))
            {
                if (i != 2 || g_sceneProgressFlag_530269)
                    nMenuSelected = i;
                break;
            }
        }

        if (nMenuSelected != -1 && nMouseBtn != nPrevMouseBtn && nMouseBtn == 1)
        {
            // click accepted: play the "On" UI effect (arrUIEffect[2] = 0x52F170)
            if (g_arrUIEffectDSBuffer_52F168[2])
                DSBufferPlay(g_arrUIEffectDSBuffer_52F168[2], 0);

            if (nMenuSelected == 0)                         // Start from Beginning
            {
                g_sceneState_52F7E0.menuState.bActiveSceneStep = 1;
                PlayScreenFade(0, 0xFFFFFFFF, pTitleSurface, 25.0f, 0);
                break;
            }
            if (nMenuSelected == 1)                         // Continue (load/config)
            {
                StopAndRestartThread(0);
                memcpy(&sceneScratch, &g_sceneState_52F7E0, sizeof(sceneScratch));
                nMenuSelected = SystemMenu_4655AB(pBgmBuffer, -1000, 1,
                                                  g_sceneState_52F7E0.field_C0,
                                                  g_sceneState_52F7E0.field_C4,
                                                  sceneScratch.menuState);
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
                StopAndRestartThread(2);
                if (nMenuSelected == -1 || nMenuSelected == 2)
                {
                    nMenuSelected = 0;
                    break;
                }
            }
            if (nMenuSelected == 2)                         // -> MainMenu (999)
            {
                nMenuSelected = 999;
                PlayScreenFade(0, 0xFFFFFFFF, pTitleSurface, 25.0f, 0);
                break;
            }
            if (nMenuSelected == 3)                         // End -> quit (1)
            {
                nMenuSelected = 1;
                PlayScreenFade(0, 0xFFFFFFFF, pTitleSurface, 25.0f, 0);
                break;
            }
        }

        // idle: keep rendering only while the BGM plays; when it stops the
        // menu auto-exits with -1 (GameMainLoop replays op.avi)
        if (pBgmBuffer && (pBgmBuffer->GetStatus(&g_dwDSStatus_5185C8) != 0
                           || !(g_dwDSStatus_5185C8 & DSBSTATUS_PLAYING)))
        {
            nMenuSelected = -1;
            break;
        }

        if (pTitleSurface)
        {
            g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &dwAntiAliasCur);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pBackBuffer_4D6C90->Blt(NULL, pTitleSurface, NULL, DDBLT_WAIT, NULL);
            if (pButtonStrip && nMenuSelected >= 0)        // hover highlight
                g_pBackBuffer_4D6C90->BltFast((int)ConvertFrom640AsFloat(381.25f),
                                              (int)ConvertFrom640AsFloat((60 * nMenuSelected + 360) * 0.625f),
                                              pButtonStrip, &arrHoverRects[nMenuSelected],
                                              DDBLTFAST_WAIT);
            FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasCur);
        }
        if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
        {
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // D3DBLEND_SRCALPHA
            g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // D3DBLEND_INVSRCALPHA
            if (pMouseSprite)
            {
                SetPos_414A5D(pMouseSprite, (float)(mouseX + 16), (float)(mouseY + 16));
                DrawSprite2D(pMouseSprite);
            }
            g_pD3DDevice_4D6C78->EndScene();
        }
        // nop_4863C3();
        PresentFrame();
        HanaVert_DrawBatch(0);
    }

    // teardown
    if (pTitleSurface)
    {
        pTitleSurface->Release();
        pTitleSurface = NULL;
    }
    if (pButtonStrip)
    {
        pButtonStrip->Release();
        pButtonStrip = NULL;
    }
    if (pMouseSprite)
    {
        CHanaObj__Free_40C1E4(&pMouseSprite->base_0);
        pMouseSprite = NULL;
    }
    if (pBgmBuffer)
    {
        pBgmBuffer->Stop();
        if (pBgmBuffer)                                     // original re-tests
        {
            pBgmBuffer->Stop();
            pBgmBuffer->Release();
            pBgmBuffer = NULL;
        }
    }
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, dwAntiAliasSaved);
    StopAndRestartThread(0);
    return nMenuSelected;
}

#define WM_YOMICHAR 0x108

BOOL hook_DoPeekMessages()
{
  MSG Msg; // [esp+0h] [ebp-1Ch] BYREF

  static bool _debugged = true;
  if (_debugged) {
    DebugLog("[HNH:SUB] DoPeekMessages_481550\n");
    _debugged = false;
  }

  PeekMessageA(&Msg, 0, WM_KEYDOWN, WM_YOMICHAR, PM_REMOVE);
  PeekMessageA(&Msg, 0, WM_MOUSEFIRST, WM_MBUTTONDBLCLK, PM_REMOVE);
  PeekMessageA(&Msg, 0, 0, 0, PM_REMOVE);
  PeekMessageA(&Msg, g_hwndParent, WM_KEYDOWN, WM_YOMICHAR, PM_REMOVE);
  PeekMessageA(&Msg, g_hwndParent, WM_MOUSEFIRST, WM_MBUTTONDBLCLK, PM_REMOVE);

  return PeekMessageA(&Msg, g_hwndParent, 0, 0, PM_REMOVE);
}

void hook_DSBufferPlay(LPDIRECTSOUNDBUFFER pDSBuf, int dwPlayFlags)
{
  int dwStatus; // [esp+0h] [ebp-4h] BYREF

  static bool _debugged = true;
  if (_debugged) {
    DebugLog("[HNH:SUB] DSBufferPlay_00423BAF\n");
    _debugged = false;
  }

  if ( g_bDSInited_5185C4 )
  {
    if ( pDSBuf->GetStatus((LPDWORD)&dwStatus) == S_OK )
    {
      pDSBuf->SetCurrentPosition(0);
      pDSBuf->Play(0, 0, dwPlayFlags);
    }
  }
}

void __cdecl hook_GetSurfaceWH(LPDIRECTDRAWSURFACE7 pSurface, _DWORD *pWidth, _DWORD *pHeight)
{
  DDSURFACEDESC2 ddSurfacDesc; // [esp+0h] [ebp-7Ch] BYREF

  DebugLog("[HNH:SUB] GetSurfaceWH_419A1A");

  ddSurfacDesc.dwSize = sizeof(DDSURFACEDESC2); // = 124
  pSurface->GetSurfaceDesc(&ddSurfacDesc);
  *pWidth = ddSurfacDesc.dwWidth;
  *pHeight = ddSurfacDesc.dwHeight;
}

void __cdecl hook_LoadBmpWithMask_413A5E(CHanaSpriteT3F6_S1FC **ppOut, LPCSTR szBmpName, LPCSTR szMaskName)
{
  CHAR szSpriteName[256]; // [esp+10h] [ebp-194h] BYREF
  DWORD width; // [esp+110h] [ebp-94h] BYREF
  CHanaSpriteT3F6_S1FC *pSprite; // [esp+114h] [ebp-90h]
  DWORD height; // [esp+118h] [ebp-8Ch] BYREF
  LPDIRECTDRAWSURFACE7 pSurfaceMask; // [esp+11Ch] [ebp-88h]
  LPDIRECTDRAWSURFACE7 pTexture; // [esp+120h] [ebp-84h]
  LPDIRECTDRAWSURFACE7 pSurfaceBmp; // [esp+124h] [ebp-80h]
  DDSURFACEDESC2 ddSurfaceDesc; // [esp+128h] [ebp-7Ch] BYREF

  DebugLog("[HNH:SUB] LoadBmpWithMask_413A5E\n");

  pSprite = 0;
  pTexture = 0;
  pSurfaceBmp = GetSurfaceFromBmp_419DE5(szBmpName);
  pSurfaceMask = GetSurfaceFromBmp_419DE5(szMaskName);
  wsprintfA(szSpriteName, "%s & %s", szBmpName, szMaskName);
  if ( pSurfaceBmp )
  {
    pTexture = CreateTextureFromSurfaceWithAlphaMask(pSurfaceBmp, pSurfaceMask);
    GetSurfaceWH(pTexture, &width, &height);
    pSprite = (CHanaSpriteT3F6_S1FC *)MemAlloc_40DEF7(0x1FCu);
    memset(pSprite, 0, sizeof(CHanaSpriteT3F6_S1FC));
    pSprite->base_0.dwType = 0x3F6;
    pSprite->pTexture_134 = pTexture;
    ddSurfaceDesc.dwSize = 124;
    ddSurfaceDesc.dwFlags = 4102;
    pTexture->GetSurfaceDesc( &ddSurfaceDesc);
    pSprite->ddSurfaceDesc_170.dwSize = 124;
    pSprite->ddSurfaceDesc_170.dwFlags = 4102;
    pSprite->pTexture_134->GetSurfaceDesc(&pSprite->ddSurfaceDesc_170);
    pSprite->dwRGBBitCount_1EC = pSprite->ddSurfaceDesc_170.ddpfPixelFormat.dwRGBBitCount;
    pSprite->dwWidth_1F0 = pSprite->ddSurfaceDesc_170.dwWidth;
    pSprite->dwHeight_1F4 = pSprite->ddSurfaceDesc_170.dwHeight;
    pSprite->lPitch_1F8 = pSprite->ddSurfaceDesc_170.lPitch;
    pSprite->fAlpha_144 = 1.0;
    pSprite->fScale_148 = 1.0;
    pSprite->pCenter_158 = 0;
    pSprite->fHalfWidth_150 = (float)(width >> 1);
    pSprite->fHalfHeight_154 = (float)(height >> 1);
    pSprite->pRect_16C = 0;
    pSprite->rect_15C.top = 0;
    pSprite->rect_15C.left = 0;
    pSprite->rect_15C.bottom = height;
    pSprite->rect_15C.right = width;
    CHanaObj__AddRef_40BFC3(&pSprite->base_0);
    AttachHanaObj_40BAB8(&pSprite->base_0, szSpriteName);
  }
  else
  {
    *ppOut = 0;
  }
  if ( pSurfaceBmp )
  {
    pSurfaceBmp->Release();
    pSurfaceBmp = 0;
  }
  if ( pSurfaceMask )
  {
    pSurfaceMask->Release();
    pSurfaceMask = 0;
  }
  *ppOut = pSprite;
}

LPDIRECTDRAWSURFACE7 __cdecl hook_CreateTextureFromSurfaceWithAlphaMask(
  LPDIRECTDRAWSURFACE7 surface,
  LPDIRECTDRAWSURFACE7 maskSurface)
{
  WORD pixelARGB4_A; // [esp+0h] [ebp-158h]
  WORD *pixelsARGB4_2A; // [esp+4h] [ebp-154h]
  WORD *pixelsARGB4_1A; // [esp+8h] [ebp-150h]
  WORD pixelRGB5; // [esp+Ch] [ebp-14Ch]
  WORD *pixelsRGB5_2; // [esp+10h] [ebp-148h]
  WORD *pixelsARGB4_1; // [esp+14h] [ebp-144h]
  DWORD pixelARGB8; // [esp+18h] [ebp-140h]
  DWORD *pixelsARGB8_2; // [esp+1Ch] [ebp-13Ch]
  DWORD *pixelsARGB8_1; // [esp+20h] [ebp-138h]
  LONG lPitch2; // [esp+24h] [ebp-134h]
  LONG lPitch; // [esp+28h] [ebp-130h]
  _D3DX_SURFACEFORMAT surfaceFormat2; // [esp+2Ch] [ebp-12Ch] BYREF
  DDSURFACEDESC2 ddSurfaceDesc2; // [esp+30h] [ebp-128h] BYREF
  DWORD nAntialias; // [esp+ACh] [ebp-ACh] BYREF
  unsigned int j; // [esp+B0h] [ebp-A8h]
  DDSURFACEDESC2 ddSurfaceDesc; // [esp+B4h] [ebp-A4h] BYREF
  unsigned int i; // [esp+130h] [ebp-28h]
  HRESULT hr; // [esp+134h] [ebp-24h]
  _D3DX_SURFACEFORMAT surfaceFormat; // [esp+138h] [ebp-20h] BYREF
  DWORD width; // [esp+13Ch] [ebp-1Ch] BYREF
  DWORD dwRGBBitCount; // [esp+140h] [ebp-18h]
  DWORD height; // [esp+144h] [ebp-14h] BYREF
  DWORD flags; // [esp+148h] [ebp-10h] BYREF
  LPDIRECTDRAWSURFACE7 texture; // [esp+14Ch] [ebp-Ch] BYREF
  DWORD bDitherEnable; // [esp+150h] [ebp-8h] BYREF
  LPDIRECTDRAWSURFACE7 texture2; // [esp+154h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] CreateTextureFromSurfaceWithAlphaMask_43EA05\n");

  surfaceFormat = D3DX_SF_A8R8G8B8;
  texture = 0;
  flags = 256;
  dwRGBBitCount = g_ddPixFormat.dwRGBBitCount;
  memset(&ddSurfaceDesc, 0, sizeof(ddSurfaceDesc));
  memset(&ddSurfaceDesc2, 0, sizeof(ddSurfaceDesc2));
  if ( !surface )
    return 0;
  if ( maskSurface )
  {
    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS, &nAntialias);
    g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_DITHERENABLE, &bDitherEnable);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DITHERENABLE, 0);
    GetSurfaceWH(surface, &width, &height);
    surfaceFormat = D3DX_SF_A8R8G8B8;
    hr = D3DXCheckTextureRequirements(g_pD3DDevice_4D6C78, &flags, &width, &height, &surfaceFormat);
    if ( hr != S_OK ) // hr
    {
      surfaceFormat = D3DX_SF_A4R4G4B4;
      hr = D3DXCheckTextureRequirements(g_pD3DDevice_4D6C78, &flags, &width, &height, &surfaceFormat);
    }
    if ( hr == S_OK ) // !hr 
    {
      GetSurfaceWH(surface, &width, &height);
      hr = D3DXCreateTexture(g_pD3DDevice_4D6C78, &flags, &width, &height, &surfaceFormat, 0, &texture, 0);
      if ( hr != S_OK ) // hr
      {
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, nAntialias);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DITHERENABLE, bDitherEnable);
        return 0;
      }
      D3DXLoadTextureFromSurface(g_pD3DDevice_4D6C78, texture, 0xFFFFFFFF, surface, 0, 0, D3DX_FT_DEFAULT);
      if ( surfaceFormat == D3DX_SF_A8R8G8B8 )
        surfaceFormat2 = D3DX_SF_A8R8G8B8;
      else
        surfaceFormat2 = D3DX_SF_R5G6B5;
      GetSurfaceWH(surface, &width, &height);
      hr = D3DXCheckTextureRequirements(g_pD3DDevice_4D6C78, &flags, &width, &height, &surfaceFormat2);
      if ( hr != S_OK ) // hr
      {
        if ( surfaceFormat == D3DX_SF_A8R8G8B8 )
          surfaceFormat2 = D3DX_SF_A8R8G8B8;
        else
          surfaceFormat2 = D3DX_SF_A4R4G4B4;
      }
      hr = D3DXCreateTexture(g_pD3DDevice_4D6C78, &flags, &width, &height, &surfaceFormat2, 0, &texture2, 0);
      if ( hr == S_OK ) // !hr
      {
        D3DXLoadTextureFromSurface(g_pD3DDevice_4D6C78, texture2, 0xFFFFFFFF, maskSurface, 0, 0, D3DX_FT_DEFAULT);
        GetSurfaceWH(texture, &width, &height);
        ddSurfaceDesc.dwSize = 124;
        ddSurfaceDesc.dwFlags = 2056;
        texture->GetSurfaceDesc(&ddSurfaceDesc);
        ddSurfaceDesc2.dwSize = 124;
        ddSurfaceDesc2.dwFlags = 2056;
        texture2->GetSurfaceDesc(&ddSurfaceDesc2);
        lPitch = ddSurfaceDesc.lPitch;
        lPitch2 = ddSurfaceDesc2.lPitch;
        texture->Lock(0, &ddSurfaceDesc, 1, 0);
        texture2->Lock(0, &ddSurfaceDesc2, 1, 0);
        if ( surfaceFormat == D3DX_SF_A8R8G8B8 && surfaceFormat2 == D3DX_SF_A8R8G8B8 )
        {
          pixelsARGB8_1 = (DWORD *)ddSurfaceDesc.lpSurface;
          pixelsARGB8_2 = (DWORD *)ddSurfaceDesc2.lpSurface;
          for ( i = 0; i < height; ++i )
          {
            for ( j = 0; j < width; ++j )
            {
              pixelARGB8 = ((unsigned __int8)pixelsARGB8_2[j]
                + ((pixelsARGB8_2[j] & 0xFF00) >> 8)
                + ((pixelsARGB8_2[j] & 0xFF0000) >> 16))
                / 3;
              if ( pixelARGB8 > 0xFF )
                pixelARGB8 = 255;
              if ( pixelARGB8 < 0x11 )
                pixelARGB8 = 0;
              pixelsARGB8_1[j] &= 0xFFFFFFu;
              pixelsARGB8_1[j] += pixelARGB8 << 24;
            }
            pixelsARGB8_1 += lPitch / 4;
            pixelsARGB8_2 += lPitch2 / 4;
          }
        }
        else if ( surfaceFormat == D3DX_SF_A4R4G4B4 && surfaceFormat2 == D3DX_SF_R5G6B5 )
        {
          pixelsARGB4_1 = (WORD *)ddSurfaceDesc.lpSurface;
          pixelsRGB5_2 = (WORD *)ddSurfaceDesc2.lpSurface;
          for ( i = 0; i < height; ++i )
          {
            for ( j = 0; j < width; ++j )
            {
              pixelRGB5 = (unsigned __int16)((pixelsRGB5_2[j] & 0x1F)
                + ((pixelsRGB5_2[j] & 0x7E0) >> 5)
                + ((pixelsRGB5_2[j] & 0xF800) >> 11))
                / 3;
              if ( pixelRGB5 > 0xFu )
                pixelRGB5 = 15;
              pixelsARGB4_1[j] &= 0xFFFu;
              pixelsARGB4_1[j] += pixelRGB5 << 12;
            }
            pixelsARGB4_1 += lPitch / 2;
            pixelsRGB5_2 += lPitch2 / 2;
          }
        }
        else if ( surfaceFormat == D3DX_SF_A4R4G4B4 && surfaceFormat2 == D3DX_SF_A4R4G4B4 )
        {
          pixelsARGB4_1A = (WORD *)ddSurfaceDesc.lpSurface;
          pixelsARGB4_2A = (WORD *)ddSurfaceDesc2.lpSurface;
          for ( i = 0; i < height; ++i )
          {
            for ( j = 0; j < width; ++j )
            {
              pixelARGB4_A = (unsigned __int16)((pixelsARGB4_2A[j] & 0xF)
                + ((pixelsARGB4_2A[j] & 0xF0) >> 4)
                + ((pixelsARGB4_2A[j] & 0xF00) >> 8))
                / 3;
              if ( pixelARGB4_A > 0xFu )
                pixelARGB4_A = 15;
              pixelsARGB4_1A[j] &= 0xFFFu;
              pixelsARGB4_1A[j] += pixelARGB4_A << 12;
            }
            pixelsARGB4_1A += lPitch / 2;
            pixelsARGB4_2A += lPitch2 / 2;
          }
        }
        texture->Unlock(0);
        texture2->Unlock(0);
        if ( texture2 )
          texture2->Release();
        texture2 = 0;
      }
    }
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, nAntialias);
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DITHERENABLE, bDitherEnable);
    return texture;
  }
  else
  {
    GetSurfaceWH(surface, &width, &height);
    surfaceFormat = D3DX_SF_R8G8B8;
    hr = D3DXCheckTextureRequirements(g_pD3DDevice_4D6C78, &flags, &width, &height, &surfaceFormat);
    if ( hr != S_OK ) // hr
    {
      surfaceFormat = D3DX_SF_R5G6B5;
      hr = D3DXCheckTextureRequirements(g_pD3DDevice_4D6C78, &flags, &width, &height, &surfaceFormat);
    }
    if ( hr != S_OK ) // hr
    {
      surfaceFormat = D3DX_SF_R5G5B5;
      hr = D3DXCheckTextureRequirements(g_pD3DDevice_4D6C78, &flags, &width, &height, &surfaceFormat);
    }
    if ( hr != S_OK ) // hr
      surfaceFormat = D3DX_SF_UNKNOWN;
    hr = D3DXCreateTexture(g_pD3DDevice_4D6C78, &flags, &width, &height, &surfaceFormat, 0, &texture, 0);
    if ( hr != S_OK ) // hr
    {
      return 0;
    }
    else
    {
      D3DXLoadTextureFromSurface(g_pD3DDevice_4D6C78, texture, 0xFFFFFFFF, surface, 0, 0, D3DX_FT_DEFAULT);
      return texture;
    }
  }
}

void register_InGameMenu_4799E2() {
    hnh_Hook(0x4799E2, hook_InGameMenu_4799E2, "InGameMenu_4799E2");
    hnh_Hook(0x47DF0D, hook_MainMenu_47DF0D, "MainMenu_47DF0D");
    hnh_Hook(0x481550, hook_DoPeekMessages, "DoPeekMessages_481550");
    hnh_Hook(0x423BAF, hook_DSBufferPlay, "DSBufferPlay_423BAF");
    hnh_Hook(0x413A5E, hook_LoadBmpWithMask_413A5E, "LoadBmpWithMask_413A5E");
    hnh_Hook(0x419A1A, hook_GetSurfaceWH, "GetSurfaceWH_419A1A");
    hnh_Hook(0x43EA05, hook_CreateTextureFromSurfaceWithAlphaMask, "CreateTextureFromSurfaceWithAlphaMask_43EA05");
}
