#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// LoadSoundEffects_47EB7F / ReleaseSoundEffects_47EEEC -- the UI sound-effect
// set plus the "Darken.BMP" overlay surface. Load is called from GameMainLoop
// at intro and after every op.avi replay; Release wraps every video playback
// (and the final quit). Both functions are referenced only by GameMainLoop.
//
// Resources (pinned globals):
//   g_arrUIEffectDSBuffer_52F168[5]  plain LPDIRECTSOUNDBUFFER, volume -1000
//                                     (~ -10 dB): door, door2, On, click, click2
//                                     (<SE>\*.wav, path code 1009)
//   g_arrFootstepSound_52F18C[4] \   two parallel CSoundStruct_8* sets loaded
//   g_arrFootstepSound_52F17C[4] /   from foot1, foot2, foot3, foot5 -- there
//                                     is no "foot4". Each file is loaded TWICE
//                                     (one independent handle per array) so the
//                                     two movement systems can play it at
//                                     separate 3D positions.
//   g_pSurfaceDarkenBmp_52F1A8       darken overlay from <SPR>\Darken.BMP
//                                     (aDarkenBmp @0x4C72EC, path code 1003)

// === PINNED GAME FUNCTIONS ===
LPDIRECTSOUNDBUFFER(__cdecl* LoadWav_422E7B)(LPSTR pszFileName) =
    reinterpret_cast<LPDIRECTSOUNDBUFFER(__cdecl*)(LPSTR)>(0x422E7B);
CSoundStruct_8 *(__cdecl*LoadWavToDS3DBuffer_42277E)(LPSTR pszFileName) =
    reinterpret_cast<CSoundStruct_8 *(__cdecl*)(LPSTR)>(0x42277E);
LPDIRECTDRAWSURFACE7(__cdecl*LoadSoundEffects_47EB7F)(void) =
    reinterpret_cast<LPDIRECTDRAWSURFACE7(__cdecl*)()>(0x47EB7F);
void (__cdecl*ReleaseSoundEffects)(void) = reinterpret_cast<void(__cdecl*)()>(0x47EEEC);

// === PINNED GLOBALS ===
LPDIRECTDRAWSURFACE7 &g_pSurfaceDarkenBmp_52F1A8 = *(LPDIRECTDRAWSURFACE7 *)0x52F1A8; // WRITABLE

// === FUNCTIONS ===
LPDIRECTDRAWSURFACE7 __cdecl hook_LoadSoundEffects_47EB7F(void)
{
    char szDoors[5][50]  = { "door", "door2", "On", "click", "click2" };
    char szFoots[10][50] = { "foot1", "foot2", "foot3", "foot5" }; // rows 4-9 stay empty
    CHAR szFileName[256];              // [esp+3ECh] [ebp-108h]
    int i;

    DebugLog("[HNH:SUB] LoadSoundEffects_47EB7F\n");

    // 5 UI-effect wavs -> plain 2D DirectSound buffers, quieted to -1000
    for (i = 0; i < 5; ++i)
    {
        wsprintfA(szFileName, "%s.wav", szDoors[i]);
        CHanaObj__GetRealPath_486D9A(szFileName, 1009);          // 1009 = SE dir
        g_arrUIEffectDSBuffer_52F168[i] = LoadWav_422E7B(szFileName);
        if (g_arrUIEffectDSBuffer_52F168[i])
            g_arrUIEffectDSBuffer_52F168[i]->SetVolume(-1000);
    }

    // 4 footstep wavs -> two independent CSoundStruct_8 handles each
    // (DS buffer + IDirectSound3DBuffer per handle)
    for (i = 0; i < 4; ++i)
    {
        wsprintfA(szFileName, "%s.wav", szFoots[i]);
        CHanaObj__GetRealPath_486D9A(szFileName, 1009);
        g_arrFootstepSound_52F18C[i] = LoadWavToDS3DBuffer_42277E(szFileName);
        g_arrFootstepSound_52F17C[i] = LoadWavToDS3DBuffer_42277E(szFileName);
    }

    // darken overlay surface: <SPR>\Darken.BMP
    wsprintfA(szFileName, "Darken.BMP");                         // aDarkenBmp @0x4C72EC
    CHanaObj__GetRealPath_486D9A(szFileName, 1003);              // 1003 = SPR dir
    g_pSurfaceDarkenBmp_52F1A8 = GetSurfaceFromBmp_419DE5(szFileName);
    return g_pSurfaceDarkenBmp_52F1A8;
    // NOTE: the original also zeroes a third, never-used 250-byte local table
    // between szDoors and szFileName (dead store, dropped here).
}

void __cdecl hook_ReleaseSoundEffects(void)
{
    int i;

    DebugLog("[HNH:SUB] ReleaseSoundEffects_47EEEC\n");

    // 1) UI-effect buffers: stop, release, clear
    for (i = 0; i < 5; ++i)
    {
        if (g_arrUIEffectDSBuffer_52F168[i])
        {
            g_arrUIEffectDSBuffer_52F168[i]->Stop();
            g_arrUIEffectDSBuffer_52F168[i]->Release();
            g_arrUIEffectDSBuffer_52F168[i] = NULL;
        }
    }

    // 2) both footstep sets: FreeSoundStruct_422C72 stops+releases the DS and
    //    3D buffers and GlobalFrees the struct; then clear the slots
    for (i = 0; i < 4; ++i)
    {
        if (g_arrFootstepSound_52F18C[i])
            FreeSoundStruct_422C72(g_arrFootstepSound_52F18C[i]);
        if (g_arrFootstepSound_52F17C[i])
            FreeSoundStruct_422C72(g_arrFootstepSound_52F17C[i]);
        g_arrFootstepSound_52F18C[i] = NULL;
        g_arrFootstepSound_52F17C[i] = NULL;
    }

    // 3) darken overlay surface
    if (g_pSurfaceDarkenBmp_52F1A8)
        g_pSurfaceDarkenBmp_52F1A8->Release();
    g_pSurfaceDarkenBmp_52F1A8 = NULL;
}

void register_LoadSoundEffects_47EB7F() {
    hnh_Hook(0x47EB7F, hook_LoadSoundEffects_47EB7F, "LoadSoundEffects_47EB7F");
}

void register_ReleaseSoundEffects_47EEEC() {
    hnh_Hook(0x47EEEC, hook_ReleaseSoundEffects, "ReleaseSoundEffects_47EEEC");
}
