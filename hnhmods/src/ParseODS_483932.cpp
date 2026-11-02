#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

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
void (__cdecl*HanaMesh_AddVertexReference)(CHanaMeshT3F5_S138 *pMeshT3F5, CHanaVertT3EA_S1FC *pVertT3EA)
  = reinterpret_cast<void(__cdecl*)(CHanaMeshT3F5_S138 *, CHanaVertT3EA_S1FC *)>(0x40FCEB);
void (__cdecl * HanaMorpT457_SetupFunsSubtree)(CHanaFrameT3EC_S354 *pFrame, CHanaFrameT3EC_S354 *pRootFrame) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *pFrame, CHanaFrameT3EC_S354 *pRootFrame)>(0x4375B3);
void (__cdecl * CHanaObj__NewMeshT3F5)(CHanaMeshT3F5_S138 **pOut)
  = reinterpret_cast<void(__cdecl*)(CHanaMeshT3F5_S138 **pOut)>(0x40FCA0);
CHanaMeshT3F5_S138 * (__cdecl * GetMeshT3F5FromObj_434FE0)(CHanaMorpT457_S40C *pVerOrMeshOrMorp)
  = reinterpret_cast<CHanaMeshT3F5_S138 *(__cdecl*)(CHanaMorpT457_S40C *pVerOrMeshOrMorp)>(0x434FE0);
void (__cdecl * HanaMorpT457_SetupFuns)(CHanaMorpT457_S40C *pMorpT457, CHanaFrameT3EC_S354 *pFrame)
  = reinterpret_cast<void(__cdecl*)(CHanaMorpT457_S40C *pMorpT457, CHanaFrameT3EC_S354 *pFrame)>(0x4372BA);
void (__cdecl * CHanaObj__NewAnimSetT3F1_S150)(CHanaAnimSetT3F1_S150 **ppOut) = reinterpret_cast<void(__cdecl*)(CHanaAnimSetT3F1_S150 **ppOut)>(0x403A00);
void (__cdecl * HanaFrame_PropagateFogToChildren)(CHanaFrameT3EC_S354 *pFrame) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *pFrame)>(0x441E7E);

// === FUNCTIONS ===


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

CHanaFrameT3EC_S354 *__cdecl hook_FindFrameByName_47E999(CHanaFrameT3EC_S354 *pRootFrame, LPCSTR pszName)
{
  int nNameLen; // [esp+0h] [ebp-1Ch]
  int i; // [esp+4h] [ebp-18h]
  CHanaFrameT3EC_S354 *pFound; // [esp+8h] [ebp-14h]
  unsigned int j; // [esp+Ch] [ebp-10h]
  int nThisNameLen; // [esp+14h] [ebp-8h]
  CHanaNode *pChildNode; // [esp+18h] [ebp-4h]

  ONCELOG("[HNH:SUB] FindFrameByName_47E999\n");

  pFound = 0;
  nNameLen = lstrlenA(pszName);
  nThisNameLen = lstrlenA(pRootFrame->szName_8);
  for ( i = 0; i <= nThisNameLen - nNameLen; ++i )
  {
    if ( !_strnicmp(&pRootFrame->szName_8[i], pszName, nNameLen) )
      return pRootFrame;
  }
  if ( pRootFrame->nCountChild )
  {
    pChildNode = pRootFrame->pFirstChildNode;
    for ( j = 0; j < pRootFrame->nCountChild; ++j )
    {
      if ( pChildNode->pData )
      {
        pFound = FindFrameByName_47E999(pChildNode->pData, pszName);
        if ( pFound )
          break;
      }
      pChildNode = pChildNode->pNext;
    }
  }
  return pFound;
}

void __cdecl hook_CHanaObj__NewAnimSetT3F1_S150(CHanaAnimSetT3F1_S150 **ppOut)
{
  CHanaAnimSetT3F1_S150 *pAnimSet; // edi

  ONCELOG("[HNH:SUB] CHanaObj__NewAnimSetT3F1_S150_403A00\n");

  pAnimSet = (CHanaAnimSetT3F1_S150 *)MemAlloc_40DEF7(0x150u);
  memset(pAnimSet, 0, sizeof(CHanaAnimSetT3F1_S150));
  pAnimSet->dwType = 0x3F1;
  CHanaObj__AddRef_40BFC3(pAnimSet);
  *ppOut = pAnimSet;
}

void __cdecl hook_HanaFrame_PropagateFogToChildren(CHanaFrameT3EC_S354 *pFrame)
{
  CHanaFrameT3EC_S354 *pSubFrame; // [esp+0h] [ebp-Ch]
  unsigned int i; // [esp+4h] [ebp-8h]
  CHanaNode *pFirstChildNode; // [esp+8h] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaFrame_PropagateFogToChildren_441E7E\n");

  if ( pFrame->nCountChild )
  {
    pFirstChildNode = pFrame->pFirstChildNode;
    for ( i = 0; i < pFrame->nCountChild; ++i )
    {
      pSubFrame = pFirstChildNode->pData;
      pSubFrame->dwFogColor = pFrame->dwFogColor;
      pSubFrame->fFogStart = pFrame->fFogStart;
      pSubFrame->fFogEnd = pFrame->fFogEnd;
      HanaFrame_PropagateFogToChildren(pSubFrame);
      pFirstChildNode = pFirstChildNode->pNext;
    }
  }
}

const CHAR (&g_szUnkTxtRootFrame_519D3C)[4] = *(const CHAR (*)[4])0x519D3C; // WRITABLE
const CHAR (&g_szUnkTxtAnimSet_519D40)[8] = *(const CHAR (*)[8])0x519D40; // WRITABLE

BOOL __cdecl hook_ParseODFFile_441F19(
  LPCSTR lpFileName,
  CHanaFrameT3EC_S354 **pOutRootFrame,
  CHanaAnimSetT3F1_S150 **ppAnimSet)
{
  int name_len_v4; // eax
  CHanaFrameT3EC_S354 *pRootFrame; // [esp-8h] [ebp-4A4h]
  CHanaAnimSetT3F1_S150 *pHanaT3F1; // [esp-4h] [ebp-4A0h]
  int rgbDiffuse; // [esp+18h] [ebp-484h]
  D3DLIGHT7 d3dLight; // [esp+1Ch] [ebp-480h] BYREF
  CHAR Buffer[256]; // [esp+84h] [ebp-418h] BYREF
  HANDLE hFile; // [esp+184h] [ebp-318h]
  CHAR String1[512]; // [esp+188h] [ebp-314h] BYREF
  int i; // [esp+388h] [ebp-114h]
  DWORD nNumberOfBytesToRead; // [esp+38Ch] [ebp-110h]
  CHAR String[256]; // [esp+390h] [ebp-10Ch] BYREF
  LPVOID lpBuffer; // [esp+490h] [ebp-Ch]
  void *pBuf1; // [esp+494h] [ebp-8h]
  DWORD NumberOfBytesRead; // [esp+498h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] ParseODFFile_441F19\n");

  lpBuffer = 0;
  CHanaObj__NewFrameT3EC_S354(0, pOutRootFrame);
  CHanaObj__NewAnimSetT3F1_S150(ppAnimSet);
  AttachHanaObj_40BAB8(*pOutRootFrame, g_szUnkTxtRootFrame_519D3C);
  AttachHanaObj_40BAB8(*ppAnimSet, g_szUnkTxtAnimSet_519D40);
  GetCurrentDirectoryA(0xFFu, Buffer);
  wsprintfA(String, "%s", lpFileName);
  for ( i = lstrlenA(String); i > 0; --i )
  {
    if ( String[i] == 92 )
    {
      String[i] = 0;
      break;
    }
  }
  SetCurrentDirectoryA(String);
  hFile = CreateFileA(lpFileName, 0x80000000, 0, 0, 3u, 0, 0);
  NumberOfBytesRead = 0;
  if ( !hFile )
    return 0;
  if ( hFile == (HANDLE)-1 )
    return 0;
  nNumberOfBytesToRead = GetFileSize(hFile, 0);
  if ( !nNumberOfBytesToRead )
    return 0;
  if ( nNumberOfBytesToRead == -1 )
    return 0;
  lpBuffer = MemAlloc_40DEF7(nNumberOfBytesToRead + 128);
  wsprintfA(String1, "オリジナル☆フォーマット%c%c%c", 13, 10, 0);
  ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, &NumberOfBytesRead, 0);
  CloseHandle(hFile);
  if ( !lstrcmpA(String1, (LPCSTR)lpBuffer) )
  {
    pHanaT3F1 = *ppAnimSet;
    pRootFrame = *pOutRootFrame;
    name_len_v4 = lstrlenA(String1);
    ParseODFContent_441A5D((LPBYTE)lpBuffer + name_len_v4 + 1, pRootFrame, pHanaT3F1);
  }
  pBuf1 = (char *)lpBuffer + nNumberOfBytesToRead - 25;// Scan for AMBI and FOGG start from -25
  for ( i = 0; i < 25; ++i )
  {
    if ( !memcmp(pBuf1, "AMBI", 4u) )
    {
      pBuf1 = (char *)pBuf1 + 4;
      rgbDiffuse = *(_DWORD *)pBuf1;
      memset(&d3dLight, 0, sizeof(d3dLight));
      d3dLight.dltType = (D3DLIGHTTYPE)(D3DLIGHT_FORCE_DWORD|0x80000000);
      d3dLight.dcvDiffuse.b = (double)(unsigned __int8)rgbDiffuse / 255.0;
      d3dLight.dcvDiffuse.g = (double)BYTE1(rgbDiffuse) / 255.0;
      d3dLight.dcvDiffuse.r = (double)BYTE2(rgbDiffuse) / 255.0;
      CHanaObj__NewLightT3EF(&(*pOutRootFrame)->pHanaLight_210, &d3dLight);
      pBuf1 = (char *)pBuf1 + 4;
    }
    if ( !memcmp(pBuf1, "FOGG", 4u) )
    {
      (*pOutRootFrame)->bHasFog = 1;
      pBuf1 = (char *)pBuf1 + 4;
      (*pOutRootFrame)->dwFogColor = *(_DWORD *)pBuf1;
      pBuf1 = (char *)pBuf1 + 4;
      g_pD3DDevice_4D6C78->SetRenderState(
        D3DRENDERSTATE_FOGCOLOR,
        (*pOutRootFrame)->dwFogColor);
      (*pOutRootFrame)->fFogStart = *(float *)pBuf1;
      pBuf1 = (char *)pBuf1 + 4;
      g_pD3DDevice_4D6C78->SetRenderState(
        D3DRENDERSTATE_FOGTABLESTART,
        LODWORD((*pOutRootFrame)->fFogStart));
      (*pOutRootFrame)->fFogEnd = *(float *)pBuf1;
      pBuf1 = (char *)pBuf1 + 4;
      g_pD3DDevice_4D6C78->SetRenderState(
        D3DRENDERSTATE_FOGTABLEEND,
        LODWORD((*pOutRootFrame)->fFogEnd));
      HanaFrame_PropagateFogToChildren(*pOutRootFrame);
    }
    pBuf1 = (char *)pBuf1 + 1;
  }
  if ( lpBuffer )
  {
    MemFree_40DF64(lpBuffer);
    lpBuffer = 0;
  }
  AttachHanaObj_40BAB8(*pOutRootFrame, lpFileName);
  AttachHanaObj_40BAB8(*ppAnimSet, lpFileName);
  MtxStackStartMultLocal_4074EF();
  HanaMorpT457_SetupFunsSubtree(*pOutRootFrame, *pOutRootFrame);
  SetCurrentDirectoryA(Buffer);
  return 1;
}

// // HanaMorpT457_SetupFunsSubtree - recursively rebuild the per-vertex morph-function tables for every T457 morph in the frame subtree rooted at pFrame (pre-order: node, then children). For each frame it calls HanaMorpT457_SetupFuns(frame->pMorpT457_20C, pRootFrame); pRootFrame is the constant starting point for HanaFrame_FindByName when a sub-object's source frame has not yet been resolved. Children are enumerated via CHanaObj::NewT3EDFromChildT3EC (child T3ED list), iterated with GetFrameAtIndexT3ED; each child is recursed into then released, and the T3ED list itself is freed at the end. Both call sites (ParseODFFile_441F19 and HanaScene_LoadFile/sub_42AE94) pass the same root frame as both pFrame and pRootFrame.
void __cdecl hook_HanaMorpT457_SetupFunsSubtree(CHanaFrameT3EC_S354 *pFrame, CHanaFrameT3EC_S354 *pRootFrame)
{
  unsigned int nChildIndex; // [esp+4h] [ebp-10h]
  CHanaFrameT3EC_S354 *pChildFrame; // [esp+8h] [ebp-Ch] BYREF
  CHanaFrameArrayT3ED_S138 *pChildrenT3ED; // [esp+Ch] [ebp-8h] BYREF
  unsigned int nChildCount; // [esp+10h] [ebp-4h]

  ONCELOG("[HNH:SUB] HanaMorpT457_SetupFunsSubtree_4375B3\n");

  pChildFrame = 0;
  pChildrenT3ED = 0;
  nChildCount = 0;
  if ( pFrame )
  {
    HanaMorpT457_SetupFuns(pFrame->pAttachedObj_20C, pRootFrame);
    CHanaObj__NewFrameArrayT3EDFromChildT3EC(pFrame, &pChildrenT3ED);
    nChildCount = CHanaObj__GetFramesCountT3ED_408E75(pChildrenT3ED);
    for ( nChildIndex = 0; nChildIndex < nChildCount; ++nChildIndex )
    {
      CHanaObj__GetFrameAtIndexT3ED(pChildrenT3ED, nChildIndex, &pChildFrame);
      if ( pChildFrame )
      {
        HanaMorpT457_SetupFunsSubtree(pChildFrame, pRootFrame);
        if ( pChildFrame )
        {
          CHanaObj__Free_40C1E4(pChildFrame);
          pChildFrame = 0;
        }
      }
    }
    if ( pChildrenT3ED )
      CHanaObj__Free_40C1E4(pChildrenT3ED);
  }
}

void __cdecl hook_CHanaObj__NewMeshT3F5(CHanaMeshT3F5_S138 **pOut)
{
  CHanaMeshT3F5_S138 *pMesh; // [esp+0h] [ebp-4h]

  ONCELOG("[HNH:SUB] CHanaObj__NewMeshT3F5_40FCA0\n");

  pMesh = (CHanaMeshT3F5_S138 *)MemAlloc_40DEF7(0x138u);
  memset(pMesh, 0, sizeof(CHanaMeshT3F5_S138));
  pMesh->dwType = 0x3F5;
  CHanaObj__AddRef_40BFC3(pMesh);
  *pOut = pMesh;
}


CHanaMeshT3F5_S138 *__cdecl hook_GetMeshT3F5FromObj_434FE0(CHanaMorpT457_S40C *pVerOrMeshOrMorp)
{
  CHanaMeshT3F5_S138 *pHanaT3F5; // [esp+4h] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] GetMeshT3F5FromObj_434FE0\n");

  if ( !pVerOrMeshOrMorp )
    return 0;
  if ( CHanaObj__GetType_40BBF4(pVerOrMeshOrMorp) == 0x3EA )
  {
    pHanaT3F5 = 0;
    CHanaObj__NewMeshT3F5(&pHanaT3F5);
    HanaMesh_AddVertexReference(pHanaT3F5, (CHanaVertT3EA_S1FC *)pVerOrMeshOrMorp);
    return pHanaT3F5;
  }
  else if ( CHanaObj__GetType_40BBF4(pVerOrMeshOrMorp) == 0x3F5 )
  {
    CHanaObj__AddRef_40BFC3(pVerOrMeshOrMorp);
    return (CHanaMeshT3F5_S138 *)pVerOrMeshOrMorp;
  }
  else if ( CHanaObj__GetType_40BBF4(pVerOrMeshOrMorp) == 0x457 )
  {
    CHanaObj__AddRef_40BFC3(pVerOrMeshOrMorp->pMeshT3F5_138);
    return pVerOrMeshOrMorp->pMeshT3F5_138;
  }
  else
  {
    return 0;
  }
}

void __cdecl hook_HanaMesh_AddVertexReference(CHanaMeshT3F5_S138 *pMeshT3F5, CHanaVertT3EA_S1FC *pVertT3EA)
{
  CHanaVertT3EA_S1FC **ppOldVertices; // [esp+14h] [ebp-8h]

  ++pMeshT3F5->nCountVertT3EA_130;
  if ( pMeshT3F5->refsVertT3EA )
  {
    ppOldVertices = pMeshT3F5->refsVertT3EA;
    pMeshT3F5->refsVertT3EA = (CHanaVertT3EA_S1FC **)MemAlloc_40DEF7(4 * pMeshT3F5->nCountVertT3EA_130 + 20);// // spare 5 pointers ?
    qmemcpy(pMeshT3F5->refsVertT3EA, ppOldVertices, 4 * ((unsigned int)(4 * pMeshT3F5->nCountVertT3EA_130 - 4) >> 2));
    MemFree_40DF64(ppOldVertices);
  }
  else
  {
    pMeshT3F5->refsVertT3EA = (CHanaVertT3EA_S1FC **)MemAlloc_40DEF7(0x84u);// inited wit 33 pointers ?
  }
  pMeshT3F5->refsVertT3EA[pMeshT3F5->nCountVertT3EA_130 - 1] = pVertT3EA;
  CHanaObj__AddRef_40BFC3(pVertT3EA);
}

FnHanaMorp HanaVert_CopyMorph = reinterpret_cast<FnHanaMorp>(0x4363A3);
FnHanaMorp HanaVert_LerpMorph = reinterpret_cast<FnHanaMorp>(0x436300);

// HanaVert_CopyMorph - per-vertex RIGID morph callback (the weight == 1.0 case). Copies the source's 6 vertex components straight over the target's: pTarget[i] = pSource[i] for i in 0..5 (position xyz + normal xyz). Fast path installed by HanaMorpT457_SetupFuns when flt_8==1.0; called the same way as HanaVert_LerpMorph, so the extra 3rd (weight) argument is simply ignored. Returns pTarget.
void __cdecl hook_HanaVert_CopyMorph(float *Target, float *pSource, float fWeight)
{
  ONCELOG("[HNH:SUB] HanaVert_CopyMorph_4363A3\n");
  *Target = *pSource;
  Target[1] = pSource[1];
  Target[2] = pSource[2];
  Target[3] = pSource[3];
  Target[4] = pSource[4];
  Target[5] = pSource[5];
}

// HanaVert_LerpMorph - per-vertex BLENDED morph callback (the weight != 1.0 case). Linearly interpolates the target's 6 vertex components toward the source's by fWeight: pTarget[i] += (pSource[i] - pTarget[i]) * fWeight, for i in 0..5 (position xyz + normal xyz; texcoords untouched). This is the arrFun_4 entry HanaMorpT457_SetupFuns installs for blended sub-object correspondences, and is invoked from HanaMorpT457_ApplySubs / ApplySubsFixed as (pTarget, pSource, flt_8). Returns pTarget.
void __cdecl hook_HanaVert_LerpMorph(float *pTarget, float *pSource, float fWeight)
{
  ONCELOG("[HNH:SUB] HanaVert_LerpMorph_436300\n");
  *pTarget = (*pSource - *pTarget) * fWeight + *pTarget;
  pTarget[1] = (pSource[1] - pTarget[1]) * fWeight + pTarget[1];
  pTarget[2] = (pSource[2] - pTarget[2]) * fWeight + pTarget[2];
  pTarget[3] = (pSource[3] - pTarget[3]) * fWeight + pTarget[3];
  pTarget[4] = (pSource[4] - pTarget[4]) * fWeight + pTarget[4];
  pTarget[5] = (pSource[5] - pTarget[5]) * fWeight + pTarget[5];
}

// HanaMorpT457_SetupFuns - resolve each subobject's source vertex object and (re)build the per-vertex morph-function table. For every pMorp->arrS8_144[i]: if the subobject's source is not yet resolved, resolve it from its source name (sub_4417EB) and GetT3F5FromObj, setting pVert_14 to the referenced vertex object. Then reallocate arrS8_144[i].arrFun_4 and fill each entry with HanaVert_CopyMorph (weight==1.0, rigid) or HanaVert_LerpMorph (blended). Returns 0 when all resolved, -1 on null/wrong-type morph. Called after a sub-object binding is registered.
int __cdecl hook_HanaMorpT457_SetupFuns(CHanaMorpT457_S40C *pMorpT457, CHanaFrameT3EC_S354 *pFrame)
{
  CHanaSubObj_S2C *parrS2C; // [esp+4h] [ebp-2Ch]
  unsigned int k; // [esp+8h] [ebp-28h]
  unsigned int i; // [esp+Ch] [ebp-24h]
  unsigned int j; // [esp+14h] [ebp-1Ch]
  unsigned int nCountS2C_0; // [esp+18h] [ebp-18h]
  int nAllResolved; // [esp+1Ch] [ebp-14h]
  CHanaMeshT3F5_S138 *pMesh; // [esp+20h] [ebp-10h]

  ONCELOG("[HNH:SUB] HanaMorpT457_SetupFuns_4372BA\n");

  nAllResolved = 1;
  if ( !pMorpT457 )
    return -1;
  if ( CHanaObj__GetType_40BBF4(pMorpT457) != 0x457 )
    return -1;
  for ( i = 0; i < pMorpT457->nCount_344; ++i )
  {
    if ( pMorpT457->arrSubObj_144[i].pSubObj_0 )
    {
      for ( j = 0; j < pMorpT457->pMeshT3F5_138->nCountVertT3EA_130; ++j )
      {
        if ( pMorpT457->arrSubObj_144[i].pSubObj_0 )
        {
          if ( !pMorpT457->arrSubObj_144[i].pSubObj_0->pFrame_118 )
          {
            nAllResolved = 0;
            pMorpT457->arrSubObj_144[i].pSubObj_0->pFrame_118 = HanaFrame_FindByName(
              pMorpT457->arrSubObj_144[i].pSubObj_0->szName_18,
              pFrame);
            if ( !pMorpT457->arrSubObj_144[i].pSubObj_0->pFrame_118 )
              continue;
            CHanaObj__Free_40C1E4(pMorpT457->arrSubObj_144[i].pSubObj_0->pFrame_118);
            pMesh = GetMeshT3F5FromObj_434FE0(pMorpT457->arrSubObj_144[i].pSubObj_0->pFrame_118->pAttachedObj_20C);
            if ( pMesh && pMesh->refsVertT3EA )
              pMorpT457->arrSubObj_144[i].pSubObj_0->pVert_14 = pMesh->refsVertT3EA[pMorpT457->arrSubObj_144[i].pSubObj_0->n0];
            if ( pMesh )
              CHanaObj__Free_40C1E4(pMesh);
          }
          nCountS2C_0 = pMorpT457->arrSubObj_144[i].pSubObj_0->nCountS2C_0;
          if ( pMorpT457->arrSubObj_144[i].arrFun_4 )
          {
            MemFree_40DF64(pMorpT457->arrSubObj_144[i].arrFun_4);
            pMorpT457->arrSubObj_144[i].arrFun_4 = 0;
          }
          pMorpT457->arrSubObj_144[i].arrFun_4 = (FnHanaMorp*)MemAlloc_40DEF7(4 * nCountS2C_0 + 128);
          parrS2C = pMorpT457->arrSubObj_144[i].pSubObj_0->parrS2C;
          for ( k = 0; k < nCountS2C_0; ++k )
          {
            if ( parrS2C[k].fWeight_8 == 1.0 )
              pMorpT457->arrSubObj_144[i].arrFun_4[k] = HanaVert_CopyMorph;
            else
              pMorpT457->arrSubObj_144[i].arrFun_4[k] = HanaVert_LerpMorph;
          }
        }
      }
    }
  }
  return nAllResolved;
}

void register_ParseODS_483932() {
    hnh_HookOff();
    hnh_Hook(0x483567, hook_ReadStrFile_483567, "ReadStrFile_483567"); // [OK]
    hnh_Hook(0x47E999, hook_FindFrameByName_47E999, "FindFrameByName_47E999"); // [OK]
    hnh_Hook(0x403A00, hook_CHanaObj__NewAnimSetT3F1_S150, "CHanaObj__NewAnimSetT3F1_S150_403A00"); // [OK]
    hnh_Hook(0x40FCA0, hook_CHanaObj__NewMeshT3F5, "CHanaObj__NewMeshT3F5_40FCA0"); // [OK]
    hnh_Hook(0x434FE0, hook_GetMeshT3F5FromObj_434FE0, "GetMeshT3F5FromObj_434FE0"); // [OK]
    hnh_Hook(0x441E7E, hook_HanaFrame_PropagateFogToChildren, "HanaFrame_PropagateFogToChildren_441E7E"); // [OK]
    hnh_Hook(0x4375B3, hook_HanaMorpT457_SetupFunsSubtree, "HanaMorpT457_SetupFunsSubtree_4375B3"); // [OK]
    hnh_Hook(0x4372BA, hook_HanaMorpT457_SetupFuns, "HanaMorpT457_SetupFuns_4372BA"); // [OK]
    hnh_Hook(0x4363A3, hook_HanaVert_CopyMorph, "HanaVert_CopyMorph_4363A3"); // [OK]
    hnh_Hook(0x436300, hook_HanaVert_LerpMorph, "HanaVert_LerpMorph_436300"); // [OK]
    hnh_Hook(0x441F19, hook_ParseODFFile_441F19, "ParseODFFile_441F19"); // [OK]
    hnh_Hook(0x483932, hook_ParseODS_483932, "ParseODS_483932"); // [OK]
    hnh_HookOn();
}
