#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// ParseODS_483932 -- the ODS dialogue-scene parser; only xref is
// LoadOds_4843CF (which reads the file image into memory first). Four
// sections, each scanning byte-wise until its end marker (the Shift-JIS
// strings are escape-encoded to stay codepage-safe):
//
// SECTION 1 (scene header, until SIZE):
//   TEXT [8-byte hdr][name]   scene name -> pScene->szName_160; the same name
//                             resolved through the Str dir (1004) is loaded by
//                             ReadStrFile_483567 (the dialogue text table)
//   SIZE [4][DWORD n]         object count -> five parallel arrays allocated
//                             (frames, animsets, sek bodies, sizes, item props)
//
// SECTION 2 (per object, until "サウンド" aJpSound@0x4C73F0 / terminator /
//            EYESANIM):
//   NAME [4][name]            ParseODFFile_441F19 on the ODF-dir (1005) path;
//                             frame + animset registered by name (nIndex++)
//   ANIM                      frame anim on the PREVIOUS object's frame
//   SEKN [4][name][DWORD n][n x {float, DWORD index}]
//                             ParseSekFile_401039 sek (event timeline) body +
//                             item-prop table; sek header linked to the animset
//                             and frame (AddRef on both), PlayAnim(0) started
//
// SECTION 3 (camera/light, until "サウンド" / terminator):
//   EYESANIM                  anim applied to g_pCameraFrame (eye tracking)
//   EYE2                      first frame whose NAME contains "eye" (case-
//                             insensitive, scanning up to len-6): the frame is
//                             temporarily renamed "XXXX" while FindFrameByName
//                             searches its subtree (so it cannot match itself),
//                             then restored; the found frame becomes
//                             pScene->pCamFrame_478 with by_474 = 1
//   Light [5][D3DLIGHT7][+96B][4][anim]
//                             new light on g_pLightFrame + its animation
//
// SECTION 4 (3D sounds, until terminator):
//   3DON                      a temp frame is created under g_pSceneFrame and
//                             all object frames are re-attached under it (so
//                             FindByName inside ParseOds3DON_483704 sees them),
//                             then re-attached back and the temp freed. (If
//                             pAnimSet_148 were NULL the loop would not
//                             advance -- original quirk, kept.)
//
// Any existing light on g_pLightFrame is freed first. Returns 1.

// === PINNED GAME FUNCTIONS ===
int (__cdecl*ParseODS_483932)(LPBYTE pBuf, CHanaSceneT458_S504 *pScene) =
    reinterpret_cast<int(__cdecl*)(LPBYTE, CHanaSceneT458_S504 *)>(0x483932);

DWORD (__cdecl*ReadStrFile_483567)(CHanaSceneT458_S504 *pScene, const char *szFileName) =
    reinterpret_cast<DWORD(__cdecl*)(CHanaSceneT458_S504 *, const char *)>(0x483567);
int (__cdecl*ParseODFFile_441F19)(LPCSTR lpFileName, CHanaFrameT3EC_S354 **pOutRootFrameT3EC,
    CHanaAnimSetT3F1_S150 **pOutT3F1) =
    reinterpret_cast<int(__cdecl*)(LPCSTR, CHanaFrameT3EC_S354 **, CHanaAnimSetT3F1_S150 **)>(0x441F19);
CSekBody *(__cdecl*ParseSekFile_401039)(LPCSTR lpFileName) =
    reinterpret_cast<CSekBody *(__cdecl*)(LPCSTR)>(0x401039);
int (__cdecl*CSekBody__PlayAnim_40179D)(CSekBody *pSekBody, int nMode) =
    reinterpret_cast<int(__cdecl*)(CSekBody *, int)>(0x40179D);
CHanaFrameT3EC_S354 *(__cdecl*FindFrameByName_47E999)(CHanaFrameT3EC_S354 *pRootFrame,
    LPCSTR pszName) =
    reinterpret_cast<CHanaFrameT3EC_S354 *(__cdecl*)(CHanaFrameT3EC_S354 *, LPCSTR)>(0x47E999);
int (__cdecl*ParseOds3DON_483704)(LPBYTE *ppBuf, CHanaAnimSetT3F1_S150 *pAnimSet,
    CHanaFrameT3EC_S354 *pTempFrame) =
    reinterpret_cast<int(__cdecl*)(LPBYTE *, CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *)>(0x483704);

// === FUNCTIONS ===
int __cdecl hook_ParseODS_483932(LPBYTE pBuf, CHanaSceneT458_S504 *pScene)
{
    CHanaFrameT3EC_S354 *pTempFrame;        // [esp+4h] [ebp-2A4h]
    CHanaLightT3EF_S1A0 *pHanaLight;        // [esp+8h] [ebp-2A0h]
    D3DLIGHT7 d3dLight;                     // [esp+Ch] [ebp-29Ch]
    CHanaFrameT3EC_S354 *pEyeFrame;         // [esp+74h] [ebp-234h]
    CHAR szFrameNameSaved[256];             // [esp+78h] [ebp-230h]
    unsigned int j, m, n;                   // [ebp-130h] / m,n reuse the slot
    int nScanLen, bFoundEye, k;             // [ebp-12Ch] / [ebp-128h] / [ebp-124h]
    DWORD i;                                // [esp+188h] [ebp-120h]
    CSekBody *pSekBody;                     // [esp+18Ch] [ebp-11Ch]
    int nIndex, nCount;                     // [ebp-114h] / [ebp-8h]
    CHAR szFileName[256];                   // [esp+198h] [ebp-110h]

    DebugLog("[HNH:SUB] ParseODS_483932\n");

    nIndex = 0;
    MtxStackStartMultLocal_4074EF();
    if (g_pLightFrame && g_pLightFrame->pHanaLight_210)
    {
        CHanaObj__Free_40C1E4((CHanaBase_S130 *)g_pLightFrame->pHanaLight_210);
        g_pLightFrame->pHanaLight_210 = NULL;
    }

    // --- SECTION 1: scene header -------------------------------------------
    for (;;)
    {
        if (!memcmp(pBuf, "TEXT", 4))
        {
            pBuf += 8;
            wsprintfA(pScene->szName_160, "%s", (const char *)pBuf);
            wsprintfA(szFileName, "%s", (const char *)pBuf);
            CHanaObj__GetRealPath_486D9A(szFileName, 1004);       // 1004 = Str dir
            pBuf += lstrlenA((LPCSTR)pBuf);   // no +1: the rescan below eats the NUL
            ReadStrFile_483567(pScene, szFileName);
        }
        else if (!memcmp(pBuf, "SIZE", 4))
            break;
        ++pBuf;
    }

    pBuf += 4;                                 // chunk == 'SIZE'
    nIndex = 0;
    nCount = *(int *)pBuf;
    pScene->dwCount_130 = nCount;
    pScene->refsFrames_134    = (CHanaFrameT3EC_S354 **)MemAlloc_40DEF7(4 * nCount);
    pScene->refsAnimeSets_138 = (CHanaAnimSetT3F1_S150 **)MemAlloc_40DEF7(4 * nCount);
    pScene->refsSekBody_13C   = (CSekBody **)MemAlloc_40DEF7(4 * nCount);
    pScene->arrSizes_144      = (DWORD *)MemAlloc_40DEF7(4 * nCount);
    pScene->arrSekItemProps_140 = (CSekItemProp **)MemAlloc_40DEF7(4 * nCount);

    // --- SECTION 2: objects (NAME / ANIM / SEKN) ---------------------------
    for (;;)
    {
        for (;;)
        {
            while (!memcmp(pBuf, "NAME", 4))
            {
                pBuf += 4;
                wsprintfA(szFileName, "%s", (const char *)pBuf);
                CHanaObj__GetRealPath_486D9A(szFileName, 1005);   // 1005 = ODF dir
                ParseODFFile_441F19(szFileName,
                                    &pScene->refsFrames_134[nIndex],
                                    &pScene->refsAnimeSets_138[nIndex]);
                AttachHanaObj_40BAB8((CHanaBase_S130 *)pScene->refsFrames_134[nIndex], (LPCSTR)pBuf);
                AttachHanaObj_40BAB8((CHanaBase_S130 *)pScene->refsAnimeSets_138[nIndex++], (LPCSTR)pBuf);
                pBuf += lstrlenA((LPCSTR)pBuf) + 1;
            }
            if (memcmp(pBuf, "ANIM", 4))
            {
                if (memcmp(pBuf, "SEKN", 4))
                    break;
                // chunk == 'SEKN': sek event body + item props
                pBuf += 4;
                wsprintfA(szFileName, "%s", (const char *)pBuf);
                CHanaObj__GetRealPath_486D9A(szFileName, 1005);
                pSekBody = ParseSekFile_401039(szFileName);
                if (pSekBody)
                {
                    wsprintfA(pSekBody->szName_0, "%s", (const char *)pBuf);
                    pScene->refsSekBody_13C[nIndex - 1] = pSekBody;
                    pBuf += lstrlenA((LPCSTR)pBuf) + 1;
                    // (the array is always allocated here; original re-tests)
                    pScene->arrSizes_144[nIndex - 1] = *(DWORD *)pBuf;
                    pBuf += 4;
                    pScene->arrSekItemProps_140[nIndex - 1] =
                        (CSekItemProp *)MemAlloc_40DEF7(8 * pScene->arrSizes_144[nIndex - 1] + 256);
                    for (i = 0; i < pScene->arrSizes_144[nIndex - 1]; ++i)
                    {
                        pScene->arrSekItemProps_140[nIndex - 1][i].flt_0 = *(float *)pBuf;
                        pScene->arrSekItemProps_140[nIndex - 1][i].index = *((DWORD *)pBuf + 1);
                        pBuf += 8;
                    }
                    // link the sek header to this object's animset + frame
                    pScene->refsSekBody_13C[nIndex - 1]->pSekHeader_220->pAnimSetT3F1 =
                        pScene->refsAnimeSets_138[nIndex - 1];
                    pScene->refsSekBody_13C[nIndex - 1]->pSekHeader_220->pFrameT3EC =
                        pScene->refsFrames_134[nIndex - 1];
                    CHanaObj__AddRef_40BFC3((CHanaBase_S130 *)pScene->refsAnimeSets_138[nIndex - 1]);
                    CHanaObj__AddRef_40BFC3((CHanaBase_S130 *)pScene->refsFrames_134[nIndex - 1]);
                    CSekBody__PlayAnim_40179D(pScene->refsSekBody_13C[nIndex - 1], 0);
                }
                else
                {
                    pBuf += lstrlenA((LPCSTR)pBuf) + 1;           // skip the name only
                }
            }
            else
            {
                // chunk == 'ANIM': animates the PREVIOUS object's frame
                pBuf += 4;
                if (pScene->refsFrames_134[nIndex - 1])
                    ParseOdfANIM_4414EB(&pBuf, pScene->pAnimSet_148,
                                        pScene->refsFrames_134[nIndex - 1]);
            }
        }
        // Shift-JIS "サウンド" (aJpSound @0x4C73F0) / "ファイルは終了"
        // (@0x4B416C) / EYESANIM end this section
        if (!memcmp(pBuf, "サウンド", 8)
            || !memcmp(pBuf, "ファイルは終了", 14)
            || !memcmp(pBuf, "EYESANIM", 8))
            break;
        ++pBuf;
    }

    // --- SECTION 3: camera + light (EYESANIM / EYE2 / Light) ---------------
    for (;;)
    {
        for (;;)
        {
            while (!memcmp(pBuf, "EYESANIM", 8))
            {
                pBuf += 8;
                if (pScene->pAnimSet_148)
                    ParseOdfANIM_4414EB(&pBuf, pScene->pAnimSet_148, g_pCameraFrame);
            }
            if (!memcmp(pBuf, "EYE2", 4))
            {
                // find the first "eye"-named frame -> scene camera
                bFoundEye = 0;
                for (j = 0; j < pScene->dwCount_130; ++j)
                {
                    nScanLen = lstrlenA(pScene->refsFrames_134[j]->szName_8) - 6;
                    for (k = 0; k < nScanLen; ++k)
                    {
                        if (!_memicmp(&pScene->refsFrames_134[j]->szName_8[k], "eye", 3))
                        {
                            pEyeFrame = NULL;
                            wsprintfA(szFrameNameSaved, "%s", pScene->refsFrames_134[j]->szName_8);
                            wsprintfA(pScene->refsFrames_134[j]->szName_8, "XXXX"); // hide self
                            pEyeFrame = FindFrameByName_47E999(pScene->refsFrames_134[j], "eye");
                            wsprintfA(pScene->refsFrames_134[j]->szName_8, "%s", szFrameNameSaved);
                            if (pEyeFrame)
                            {
                                bFoundEye = 1;
                                pScene->by_474 = 1;
                                pScene->pCamFrame_478 = pEyeFrame;
                                break;
                            }
                        }
                    }
                    if (bFoundEye == 1)
                        break;
                }
                pBuf += 4;
            }
            else if (!memcmp(pBuf, "Light", 5))
            {
                // new light on the light frame + its animation
                pBuf += 5;
                memcpy(&d3dLight, pBuf, sizeof(d3dLight));
                CHanaObj__NewLightT3EF(&pHanaLight, &d3dLight);
                AssignLigthtToFrame_40539D(g_pLightFrame, pHanaLight);
                if (pHanaLight)
                {
                    CHanaObj__Free_40C1E4((CHanaBase_S130 *)pHanaLight);
                    pHanaLight = NULL;
                }
                pBuf += 104;                   // light payload stride
                pBuf += 4;
                ParseOdfANIM_4414EB(&pBuf, pScene->pAnimSet_148, g_pLightFrame);
            }
            else
                break;
        }
        if (!memcmp(pBuf, "サウンド", 8)
            || !memcmp(pBuf, "ファイルは終了", 14))
            break;
        ++pBuf;
    }

    // --- SECTION 4: 3D sound emitters (3DON) -------------------------------
    for (;;)
    {
        while (!memcmp(pBuf, "3DON", 4))
        {
            if (pScene->pAnimSet_148)
            {
                pTempFrame = NULL;
                CHanaObj__NewFrameT3EC_S354((CHanaFrameT3EC_S354 *)g_pSceneFrame, &pTempFrame);
                for (m = 0; m < pScene->dwCount_130; ++m)        // park frames under temp
                    AttachChild_4055FD(pTempFrame, pScene->refsFrames_134[m]);
                ParseOds3DON_483704(&pBuf, pScene->pAnimSet_148, pTempFrame);
                for (n = 0; n < pScene->dwCount_130; ++n)        // restore to scene root
                    AttachChild_4055FD(g_pSceneFrame, pScene->refsFrames_134[n]);
                if (pTempFrame)
                {
                    CHanaObj__Free_40C1E4((CHanaBase_S130 *)pTempFrame);
                    pTempFrame = NULL;
                }
            }
        }
        if (!memcmp(pBuf, "ファイルは終了", 14))
            break;
        ++pBuf;
    }
    return 1;
}

void __cdecl hook_ReadStrFile_483567(CHanaSceneT458_S504 *pScene, const char *szFileName)
{
  CHAR FileName[256]; // [esp+0h] [ebp-10Ch] BYREF
  HANDLE hFile; // [esp+100h] [ebp-Ch]
  DWORD nNumberOfBytesToReadAlign50; // [esp+104h] [ebp-8h]
  DWORD NumberOfBytesRead; // [esp+108h] [ebp-4h] BYREF

  DebugLog("[HNH:SUB] ReadStrFile_483567\n");

  wsprintfA(FileName, "%s", szFileName);
  hFile = CreateFileA(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
  if ( hFile && hFile != (HANDLE)INVALID_HANDLE_VALUE )
  {
    nNumberOfBytesToReadAlign50 = GetFileSize(hFile, 0);
    if ( nNumberOfBytesToReadAlign50 && nNumberOfBytesToReadAlign50 != (unsigned int)INVALID_FILE_SIZE )
    {
      pScene->dwPtrSize_44C = nNumberOfBytesToReadAlign50;
      nNumberOfBytesToReadAlign50 = 50 * (nNumberOfBytesToReadAlign50 / 50 + 1);
      pScene->ptrStrData_260 = MemAlloc_40DEF7(nNumberOfBytesToReadAlign50);
      ReadFile(hFile, pScene->ptrStrData_260, nNumberOfBytesToReadAlign50, &NumberOfBytesRead, 0);
      CloseHandle(hFile);
    }
    else
    {
      CloseHandle(hFile);
    }
  }
}

void register_ParseODS_483932() {
    hnh_Hook(0x483932, hook_ParseODS_483932, "ParseODS_483932"); // [OK]
    // [OK] hnh_Hook(0x483567, hook_ReadStrFile_483567, "ReadStrFile_483567");
}
