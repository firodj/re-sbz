#include "dx7.hpp"
#include <mmiscapi.h>
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

// LoadWav_422E7B -- classic mmio WAV -> DirectSound secondary-buffer loader
// (the DX7 SDK "WAVE stream" pattern), used game-wide (12 xrefs: menus,
// battles, ParseExecStr, LoadSoundEffects, ...). PCM only: reads the 18-byte
// WAVEFORMATEX from "fmt ", requires wFormatTag == WAVE_FORMAT_PCM, creates
// the buffer with the format pointer, then streams the "data" chunk in via
// Lock/mmioRead/Unlock. Every failure path returns NULL after its own cleanup
// (mmioClose, SafeFree of the format block, Release of a half-built buffer);
// HanaDS_SetErrStr records the DS error on the create/lock paths only.
//
// dwFlags = 0x10102 = DSBCAPS_STATIC | DSBCAPS_CTRLPOSITIONNOTIFY |
// DSBCAPS_GETCURRENTPOSITION2 -- NOTE: no DSBCAPS_CTRLVOLUME, so the
// SetVolume(-1000) in hook_LoadSoundEffects_47EB7F above fails with
// DSERR_CONTROLUNAVAIL (return value ignored): the door/click UI effects
// actually play at full volume in the original game.
LPDIRECTSOUNDBUFFER __cdecl hook_LoadWav_422E7B(LPSTR pszFileName)
{
    MMCKINFO ckParent;                 // [esp+24h] [ebp-4Ch] "WAVE" RIFF
    MMCKINFO ckSubchunk;               // [esp+0h]  [ebp-70h] "fmt " / "data"
    DSBUFFERDESC dsbDesc;              // [esp+40h] [ebp-30h]
    HMMIO hmmio;                       // [esp+18h] [ebp-58h]
    LPWAVEFORMATEX pwfx;               // [esp+3Ch] [ebp-34h] 18 bytes
    LPVOID pAudio1, pAudio2;           // [ebp-38h] / [ebp-54h] Lock() targets
    DWORD cbAudio1, cbAudio2;          // [ebp-8h]  / [ebp-50h]
    DWORD cbDataSize;                  // [esp+14h] [ebp-5Ch] "data" chunk size
    LPDIRECTSOUNDBUFFER pDsBufferOut;  // [esp+6Ch] [ebp-4h]
    HRESULT hr;

    DebugLog("[HNH:SUB] LoadWav_422E7B\n");

    if (!g_bDSInited_5185C4)
        return NULL;
    hmmio = mmioOpenA(pszFileName, NULL, 0);
    if (!hmmio)
        return NULL;
    mmioSetBuffer(hmmio, NULL, 0, 0);

    // descend into the "WAVE" chunk of the RIFF file
    ckParent.fccType = mmioStringToFOURCCA("WAVE", 0);        // aWave_0 @0x4B2760
    if (mmioDescend(hmmio, &ckParent, NULL, MMIO_FINDRIFF))
    {
        mmioClose(hmmio, 0);
        return NULL;
    }

    // read the 18-byte WAVEFORMATEX out of "fmt " and require plain PCM
    ckSubchunk.ckid = mmioStringToFOURCCA("fmt ", 0);         // aFmt_0 @0x4B2768
    if (mmioDescend(hmmio, &ckSubchunk, &ckParent, MMIO_FINDCHUNK))
    {
        mmioClose(hmmio, 0);
        return NULL;
    }
    pwfx = (LPWAVEFORMATEX)AllocMem(sizeof(WAVEFORMATEX));    // 0x12 = 18 bytes
    if (!pwfx)
    {
        mmioClose(hmmio, 0);
        return NULL;
    }
    if (mmioRead(hmmio, (HPSTR)pwfx, sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX)
        || pwfx->wFormatTag != WAVE_FORMAT_PCM)
    {
        mmioClose(hmmio, 0);
        SafeFree(pwfx);
        return NULL;
    }
    mmioAscend(hmmio, &ckSubchunk, 0);

    // locate "data"; its chunk size drives the buffer allocation
    ckSubchunk.ckid = mmioStringToFOURCCA("data", 0);         // aData_0 @0x4B2770
    if (mmioDescend(hmmio, &ckSubchunk, &ckParent, MMIO_FINDCHUNK))
    {
        mmioClose(hmmio, 0);
        SafeFree(pwfx);
        return NULL;
    }
    cbDataSize = ckSubchunk.cksize;

    // create the secondary buffer sharing our format block
    memset(&dsbDesc, 0, sizeof(dsbDesc));
    dsbDesc.dwSize = sizeof(DSBUFFERDESC);                    // 36
    dsbDesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
    dsbDesc.dwBufferBytes = cbDataSize;
    dsbDesc.lpwfxFormat = pwfx;
    hr = g_ppDS->CreateSoundBuffer(&dsbDesc, &pDsBufferOut, NULL);
    SafeFree(pwfx);                                           // done with the format block
    if (hr != S_OK)
    {
        mmioClose(hmmio, 0);
        HanaDS_SetErrStr(hr);
        return NULL;
    }

    // stream the sample data through the Lock() write pair
    hr = pDsBufferOut->Lock(0, cbDataSize, &pAudio1, &cbAudio1, &pAudio2, &cbAudio2, 0);
    if (hr != S_OK)
    {
        mmioClose(hmmio, 0);
        pDsBufferOut->Release();
        HanaDS_SetErrStr(hr);
        return NULL;
    }
    if (mmioRead(hmmio, (HPSTR)pAudio1, cbAudio1) != (LONG)cbAudio1
        || (pAudio2 && mmioRead(hmmio, (HPSTR)pAudio2, cbAudio2) != (LONG)cbAudio2))
    {
        mmioClose(hmmio, 0);
        pDsBufferOut->Release();
        return NULL;
    }
    mmioClose(hmmio, 0);
    if (pDsBufferOut->Unlock(pAudio1, cbAudio1, pAudio2, cbAudio2) != S_OK)
    {
        pDsBufferOut->Release();
        return NULL;
    }
    return pDsBufferOut;
}

// LoadWavToDS3DBuffer_42277E -- 3D-positioned variant of hook_LoadWav_422E7B
// above (9 xrefs: RunDuelBattle, RunPracticeScene, ParseOdf3DON/ParseOds3DON,
// LoadSoundEffects). Same mmio WAVE/"fmt "/"data" walk, but:
//   * mmio opened MMIO_ALLOCBUF with a 512 KB I/O buffer (mmioSetBuffer)
//   * the 18-byte format block comes from GlobalAlloc(GMEM_FIXED), not AllocMem
//   * dwFlags = 0x92 = DSBCAPS_STATIC | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D,
//     and the 3D algorithm is NEGOTIATED by retrying CreateSoundBuffer:
//     DS3DALG_HRTF_FULL -> DS3DALG_DEFAULT (zeroed GUID) -> DS3DALG_HRTF_LIGHT;
//     only if all three attempts fail is it an error.
//   * after Lock/mmioRead/Unlock it QueryInterfaces IDirectSound3DBuffer and
//     returns a GlobalAlloc'd CSoundStruct_8 { pDSBuffer, pDS3DBuffer }.
// The primary buffer's channel count (g_pdsBuffer->GetFormat) picks the 3D min
// distance: mono -> 1499 (above the 500 max distance, i.e. distance
// attenuation effectively disabled), stereo -> 0.1; max distance 500; mode
// DS3DMODE_NORMAL, all applied immediately.
CSoundStruct_8 *__cdecl hook_LoadWavToDS3DBuffer_42277E(LPSTR pszFileName)
{
    MMCKINFO ckParent;                    // [esp+3Ch] [ebp-54h] "WAVE" RIFF
    MMCKINFO ckSubchunk;                  // [esp+0h]  [ebp-90h] "fmt " / "data"
    WAVEFORMATEX wfxPrimary;              // [esp+28h] [ebp-68h] primary-mix format
    DSBUFFERDESC dsbDesc;                 // [esp+5Ch] [ebp-34h]
    HMMIO hmmio;                          // [esp+1Ch] [ebp-74h]
    LPWAVEFORMATEX pwfx;                  // [esp+54h] [ebp-3Ch] GlobalAlloc'd
    LPVOID pAudio1, pAudio2;              // [ebp-40h] / [ebp-70h] Lock() targets
    DWORD cbAudio1, cbAudio2;             // [ebp-8h]  / [ebp-6Ch]
    DWORD cbDataSize;                     // [esp+14h] [ebp-7Ch] "data" chunk size
    IDirectSound3DBuffer *pDs3DBuffer;    // [esp+58h] [ebp-38h]
    LPDIRECTSOUNDBUFFER pDsBuffer;        // [esp+8Ch] [ebp-4h]
    CSoundStruct_8 *pSnd;                 // [esp+84h] [ebp-Ch]
    HRESULT hr;

    DebugLog("[HNH:SUB] LoadWavToDS3DBuffer_42277E\n");

    pSnd = NULL;
    if (!g_bDSInited_5185C4)
        return NULL;

    // primary mix format; its channel count steers the 3D min distance below.
    // (The original reads HIWORD of the raw first dword == nChannels.)
    g_pdsBuffer->GetFormat(&wfxPrimary, sizeof(WAVEFORMATEX), 0);

    hmmio = mmioOpenA(pszFileName, NULL, MMIO_ALLOCBUF);
    if (!hmmio)
        return NULL;
    mmioSetBuffer(hmmio, NULL, 0x80000, 0);                    // 512 KB I/O buffer

    // descend into the "WAVE" chunk of the RIFF file
    ckParent.fccType = mmioStringToFOURCCA("WAVE", 0);         // sz @0x4B2748
    if (mmioDescend(hmmio, &ckParent, NULL, MMIO_FINDRIFF))
    {
        mmioClose(hmmio, 0);
        return NULL;
    }

    // read the 18-byte WAVEFORMATEX out of "fmt " and require plain PCM
    ckSubchunk.ckid = mmioStringToFOURCCA("fmt ", 0);          // aFmt @0x4B2750
    if (mmioDescend(hmmio, &ckSubchunk, &ckParent, MMIO_FINDCHUNK))
    {
        mmioClose(hmmio, 0);
        return NULL;
    }
    pwfx = (LPWAVEFORMATEX)GlobalAlloc(GMEM_FIXED, sizeof(WAVEFORMATEX));
    if (!pwfx)
    {
        mmioClose(hmmio, 0);
        return NULL;
    }
    if (mmioRead(hmmio, (HPSTR)pwfx, sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX)
        || pwfx->wFormatTag != WAVE_FORMAT_PCM)
    {
        mmioClose(hmmio, 0);
        GlobalFree(pwfx);
        return NULL;
    }
    mmioAscend(hmmio, &ckSubchunk, 0);

    // locate "data"; its chunk size drives the buffer allocation
    ckSubchunk.ckid = mmioStringToFOURCCA("data", 0);          // aData @0x4B2758
    if (mmioDescend(hmmio, &ckSubchunk, &ckParent, MMIO_FINDCHUNK))
    {
        mmioClose(hmmio, 0);
        GlobalFree(pwfx);
        return NULL;
    }
    cbDataSize = ckSubchunk.cksize;

    memset(&dsbDesc, 0, sizeof(dsbDesc));
    dsbDesc.dwSize = sizeof(DSBUFFERDESC);                      // 36
    dsbDesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRL3D;
    dsbDesc.dwBufferBytes = cbDataSize;
    dsbDesc.lpwfxFormat = pwfx;

    // negotiate the 3D algorithm: HRTF_FULL -> DEFAULT -> HRTF_LIGHT
    dsbDesc.guid3DAlgorithm = DS3DALG_HRTF_FULL;
    hr = g_ppDS->CreateSoundBuffer(&dsbDesc, &pDsBuffer, NULL);
    if (hr != S_OK)
    {
        dsbDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
        hr = g_ppDS->CreateSoundBuffer(&dsbDesc, &pDsBuffer, NULL);
    }
    if (hr != S_OK)
    {
        dsbDesc.guid3DAlgorithm = DS3DALG_HRTF_LIGHT;
        hr = g_ppDS->CreateSoundBuffer(&dsbDesc, &pDsBuffer, NULL);
    }
    if (hr != S_OK)
    {
        HanaDS_SetErrStr(hr);
        mmioClose(hmmio, 0);
        GlobalFree(pwfx);
        return NULL;
    }
    GlobalFree(pwfx);                                           // done with the format block

    // stream the sample data through the Lock() write pair
    hr = pDsBuffer->Lock(0, cbDataSize, &pAudio1, &cbAudio1, &pAudio2, &cbAudio2, 0);
    if (hr != S_OK)
    {
        HanaDS_SetErrStr(hr);
        mmioClose(hmmio, 0);
        pDsBuffer->Release();
        return NULL;
    }
    if (mmioRead(hmmio, (HPSTR)pAudio1, cbAudio1) != (LONG)cbAudio1
        || (pAudio2 && mmioRead(hmmio, (HPSTR)pAudio2, cbAudio2) != (LONG)cbAudio2))
    {
        mmioClose(hmmio, 0);
        pDsBuffer->Release();
        return NULL;
    }
    mmioClose(hmmio, 0);
    hr = pDsBuffer->Unlock(pAudio1, cbAudio1, pAudio2, cbAudio2);
    if (hr != S_OK)
    {
        HanaDS_SetErrStr(hr);
        pDsBuffer->Release();
        return NULL;
    }

    // grab the 3D interface and wrap both in a CSoundStruct_8
    hr = pDsBuffer->QueryInterface(IID_IDirectSound3DBuffer, (LPVOID *)&pDs3DBuffer);
    if (hr != S_OK)
    {
        pDsBuffer->Release();
        return NULL;
    }
    pSnd = (CSoundStruct_8 *)GlobalAlloc(GMEM_FIXED, sizeof(CSoundStruct_8));
    if (!pSnd)
    {
        pDs3DBuffer->Release();
        pDsBuffer->Release();
        return NULL;
    }

    pDs3DBuffer->SetMode(DS3DMODE_NORMAL, DS3D_IMMEDIATE);
    if (wfxPrimary.nChannels == 1)                              // mono primary mix
        pDs3DBuffer->SetMinDistance(1499.0f, DS3D_IMMEDIATE);   // > max dist: flatten attenuation
    else
        pDs3DBuffer->SetMinDistance(0.1f, DS3D_IMMEDIATE);
    pDs3DBuffer->SetMaxDistance(500.0f, DS3D_IMMEDIATE);

    pSnd->pDSBuffer_0 = pDsBuffer;
    pSnd->pDS3DBuffer_4 = pDs3DBuffer;
    return pSnd;
}

void register_LoadSoundEffects_47EB7F() {
  hnh_HookOff();
  hnh_Hook(0x47EB7F, hook_LoadSoundEffects_47EB7F, "LoadSoundEffects_47EB7F"); // [OK] 
  hnh_Hook(0x47EEEC, hook_ReleaseSoundEffects, "ReleaseSoundEffects_47EEEC"); // [OK] 
  hnh_Hook(0x422E7B, hook_LoadWav_422E7B, "LoadWav_422E7B"); // [OK] 
  hnh_Hook(0x42277E, hook_LoadWavToDS3DBuffer_42277E, "LoadWavToDS3DBuffer_42277E"); // [OK] 
  hnh_HookOn();
}
