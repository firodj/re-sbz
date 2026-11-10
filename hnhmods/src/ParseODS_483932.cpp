// GAME: [011019][TEATIME] 華の悲鳴　～壊れた硝子の心～
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"
#include "hnh_hook.hpp"

// === PINNED GAME FUNCTIONS ===
int (__cdecl*ParseODS_483932)(LPBYTE pBuf, CHanaSceneT458_S504 *pScene) =
    reinterpret_cast<int(__cdecl*)(LPBYTE, CHanaSceneT458_S504 *)>(0x483932);
BOOL (__cdecl*LoadOdsEx_48564B)(LPCSTR lpFileName, CHanaSceneT458_S504 **ppScene,
  CHanaFrameT3EC_S354 **arrFrames, CHanaAnimSetT3F1_S150 **arrAnimSets,
  int nCount) =
  reinterpret_cast<BOOL(__cdecl*)(LPCSTR, CHanaSceneT458_S504 **,
    CHanaFrameT3EC_S354 **, CHanaAnimSetT3F1_S150 **, int)>(0x48564B);

BOOL (__cdecl*LoadSekAndOdf_4445DD)(LPCSTR pszSekFile, LPCSTR pszOdfFile, CSekBody **ppSekBody) =
reinterpret_cast<int(__cdecl*)(LPCSTR, LPCSTR, CSekBody **)>(0x4445DD);

void (__cdecl*ReadStrFile_483567)(CHanaSceneT458_S504 *pScene, const char *szFileName) =
    reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, const char *)>(0x483567);
int (__cdecl*ParseODFFile_441F19)(LPCSTR lpFileName, CHanaFrameT3EC_S354 **pOutRootFrameT3EC,
    CHanaAnimSetT3F1_S150 **pOutT3F1) =
    reinterpret_cast<int(__cdecl*)(LPCSTR, CHanaFrameT3EC_S354 **, CHanaAnimSetT3F1_S150 **)>(0x441F19);
CSekBody *(__cdecl*ParseSekFile_401039)(LPCSTR lpFileName) =
    reinterpret_cast<CSekBody *(__cdecl*)(LPCSTR)>(0x401039);
void (__cdecl*CSekBody__PlayAnim_40179D)(CSekBody *pSekBody, int nMode) =
    reinterpret_cast<void(__cdecl*)(CSekBody *, int)>(0x40179D);
CHanaFrameT3EC_S354 *(__cdecl*FindFrameByName_47E999)(CHanaFrameT3EC_S354 *pRootFrame,
    LPCSTR pszName) =
    reinterpret_cast<CHanaFrameT3EC_S354 *(__cdecl*)(CHanaFrameT3EC_S354 *, LPCSTR)>(0x47E999);
void (__cdecl*ParseOds3DON_483704)(LPBYTE *ppBuf, CHanaAnimSetT3F1_S150 *pAnimSet, CHanaFrameT3EC_S354 *pTempFrame)
  = reinterpret_cast<void(__cdecl*)(LPBYTE *, CHanaAnimSetT3F1_S150 *, CHanaFrameT3EC_S354 *)>(0x483704);
void (__cdecl*HanaMesh_AddVertexReference)(CHanaMeshT3F5_S138 *pMeshT3F5, CHanaVertT3EA_S1FC *pVertT3EA)
  = reinterpret_cast<void(__cdecl*)(CHanaMeshT3F5_S138 *, CHanaVertT3EA_S1FC *)>(0x40FCEB);
void (__cdecl * HanaMorpT457_SetupFunsSubtree)(CHanaFrameT3EC_S354 *pFrame, CHanaFrameT3EC_S354 *pRootFrame) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *pFrame, CHanaFrameT3EC_S354 *pRootFrame)>(0x4375B3);
void (__cdecl * CHanaObj__NewMeshT3F5)(CHanaMeshT3F5_S138 **pOut)
  = reinterpret_cast<void(__cdecl*)(CHanaMeshT3F5_S138 **pOut)>(0x40FCA0);
CHanaMeshT3F5_S138 * (__cdecl * GetMeshT3F5FromObj_434FE0)(CHanaMorpT457_S40C *pVerOrMeshOrMorp)
  = reinterpret_cast<CHanaMeshT3F5_S138 *(__cdecl*)(CHanaMorpT457_S40C *pVerOrMeshOrMorp)>(0x434FE0);
int (__cdecl * HanaMorpT457_SetupFuns)(CHanaMorpT457_S40C *pMorpT457, CHanaFrameT3EC_S354 *pFrame)
  = reinterpret_cast<int(__cdecl*)(CHanaMorpT457_S40C *pMorpT457, CHanaFrameT3EC_S354 *pFrame)>(0x4372BA);
void (__cdecl * CHanaObj__NewAnimSetT3F1_S150)(CHanaAnimSetT3F1_S150 **ppOut) = reinterpret_cast<void(__cdecl*)(CHanaAnimSetT3F1_S150 **ppOut)>(0x403A00);
void (__cdecl * HanaFrame_PropagateFogToChildren)(CHanaFrameT3EC_S354 *pFrame) = reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *pFrame)>(0x441E7E);
CSekHeader * (__cdecl *SekHeader_CloneClean)(const CSekHeader *pSrcHdr)
  = reinterpret_cast<CSekHeader * (__cdecl*)(const CSekHeader *pSrcHdr)>(0x40d875);
CHanaFrameT3EC_S354 * (__cdecl *CHanaFrameT3EC__FindByName_483650)(CHanaFrameT3EC_S354 *, LPCSTR lpString)
  = reinterpret_cast<CHanaFrameT3EC_S354 * (__cdecl*)(CHanaFrameT3EC_S354 *, LPCSTR lpString)>(0x483650);
void (__cdecl *ParseOdsEx_484A38)(LPCSTR lpBuffer, CHanaSceneT458_S504 *pScene, CHanaFrameT3EC_S354 **arrFrames, CHanaAnimSetT3F1_S150 **arrAnimeSets, int dwCount)
= reinterpret_cast<void (__cdecl*)(LPCSTR lpBuffer, CHanaSceneT458_S504 *pScene, CHanaFrameT3EC_S354 **arrFrames, CHanaAnimSetT3F1_S150 **arrAnimeSets, int dwCount)>(0x484a38);
void (__cdecl *SekBody__UpdateTo_4014E4)(CSekBody *pSekBody, int nItem)
= reinterpret_cast<void (__cdecl *)(CSekBody *, int)>(0x4014E4);

FnHanaMorp HanaVert_CopyMorph = reinterpret_cast<FnHanaMorp>(0x4363A3);
FnHanaMorp HanaVert_LerpMorph = reinterpret_cast<FnHanaMorp>(0x436300);

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

    ONCELOG("[HNH:SUB] ParseODS_483932\n");

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

  ONCELOG("[HNH:SUB] ReadStrFile_483567\n");

  wsprintfA(FileName, "%s", szFileName);
  hFile = CreateFileA(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
  if ( hFile && hFile != (HANDLE)INVALID_HANDLE_VALUE )
  {
    nNumberOfBytesToReadAlign50 = GetFileSize(hFile, 0);
    if ( nNumberOfBytesToReadAlign50 && nNumberOfBytesToReadAlign50 != (unsigned int)INVALID_FILE_SIZE )
    {
      pScene->dwPtrSize_44C = nNumberOfBytesToReadAlign50;
      nNumberOfBytesToReadAlign50 = 50 * (nNumberOfBytesToReadAlign50 / 50 + 1);
      pScene->ptrStrData_260 = (char*)MemAlloc_40DEF7(nNumberOfBytesToReadAlign50);
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
  ONCELOG("[HNH:SUB] HanaMesh_AddVertexReference_40FCEB\n");
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

CSekBody *__cdecl hook_ParseSekFile_401039(LPCSTR lpFileName)
{
  CSekHeader *v2; // eax
  DWORD NumberOfBytesRead; // [esp+0h] [ebp-348h] BYREF
  CSekBody *pOut; // [esp+4h] [ebp-344h]
  HANDLE hObject; // [esp+8h] [ebp-340h]
  CSekHeader sekHeader; // [esp+Ch] [ebp-33Ch] BYREF
  BYTE Buffer[256]; // [esp+148h] [ebp-200h] BYREF
  BYTE v8[256]; // [esp+248h] [ebp-100h] BYREF

  ONCELOG("[HNH:SUB] ParseSekFile_401039\n");

  hObject = CreateFileA(lpFileName, 0x80000000, 1u, 0, 3u, 0x8000000u, 0);
  if ( hObject == (HANDLE)-1 || !hObject )
    return 0;
  pOut = (CSekBody *)GlobalAlloc(0, 0x5054u);
  if ( pOut )
  {
    memset(pOut, 0, sizeof(CSekBody));
    memset(&sekHeader, 0, sizeof(sekHeader));
    ReadFile(hObject, Buffer, 0x100u, &NumberOfBytesRead, 0);
    ReadFile(hObject, v8, 0x100u, &NumberOfBytesRead, 0);
    ReadFile(hObject, pOut, 0x5054u, &NumberOfBytesRead, 0);
    CloseHandle(hObject);
    v2 = SekHeader_CloneClean(&sekHeader);
    pOut->pSekHeader_220 = v2;
    pOut->field_24C = 0;
    pOut->angle_214 = 0;
    pOut->angle_218 = 0;
    pOut->angle_210 = 0;
    pOut->count_5050 = 1;
    wsprintfA(pOut->szName_0, "%s", lpFileName);
    return pOut;
  }
  else
  {
    CloseHandle(hObject);
    return 0;
  }
}

CSekHeader *__cdecl hook_SekHeader_CloneClean(const CSekHeader *pSrc)
{
  CSekHeader *pDst; // [esp+10h] [ebp-4h]
  ONCELOG("[HNH:SUB] SekHeader_CloneClean_40D875\n");
  pDst = (CSekHeader *)MemAlloc_40DEF7(0x13Cu);
  if ( !pDst )
    return 0;
  qmemcpy(pDst, pSrc, sizeof(CSekHeader));
  pDst->pFrameT3EC = 0;
  pDst->pAnimSetT3F1 = 0;
  pDst->pObj_138 = 0;
  pDst->pObj_130 = 0;
  pDst->pObj_12C = 0;
  return pDst;
}

// CSekBody::PlayAnim(pSekBody, nItemIndex) - select & start animation item nItemIndex on the body.
// Sets field_200/field_208 = nItemIndex, resets that item's field_44/field_6C/field_64 (loop counter) to 0,
// computes its per-unit speed float_5C = (float_58 - float_54)/field_50, and starts playback at float_54
// (float_60). Clears the body's field_244/field_248 flags; if the item's field_7C (blend) flag is set,
// starts a crossfade (float_21C = 1.0). Note: the trailing `return 156 * nItemIndex` is just the item's
// byte offset left in eax - callers ignore it. items_250 is a 0x9C-byte-per-entry SEK item array.
void __cdecl hook_CSekBody__PlayAnim_40179D(CSekBody *pSekBody, int nItemIndex)
{
  ONCELOG("[HNH:SUB] CSekBody__PlayAnim_40179D\n");
  pSekBody->field_200 = nItemIndex;
  pSekBody->field_208 = nItemIndex;
  pSekBody->items_250[nItemIndex].field_44 = 0;
  pSekBody->items_250[nItemIndex].field_6C = 0;
  pSekBody->items_250[nItemIndex].field_64 = 0;
  if ( pSekBody->items_250[nItemIndex].field_50 )
    pSekBody->items_250[nItemIndex].float_5C = (pSekBody->items_250[nItemIndex].float_58
      - pSekBody->items_250[nItemIndex].float_54)
    / (double)pSekBody->items_250[nItemIndex].field_50;
  else
    pSekBody->items_250[nItemIndex].float_5C = 0.0;
  pSekBody->items_250[nItemIndex].float_60 = pSekBody->items_250[nItemIndex].float_54;
  pSekBody->field_244 = 0;
  pSekBody->field_248 = 0;
  if ( !pSekBody->items_250[nItemIndex].field_7C )
    pSekBody->float_21C = 1.0;
}

void __cdecl hook_ParseOds3DON_483704(LPBYTE *ppBuf, CHanaAnimSetT3F1_S150 *pAnimSet, CHanaFrameT3EC_S354 *pFrame)
{
  int len_name1a; // eax
  int len_name1b; // eax
  CSoundStruct_8 *pSound; // eax
  CHanaFrameT3EC_S354 *pTheFrame; // eax
  int len_name2; // eax
  CHAR pszFileName[256]; // [esp+0h] [ebp-10Ch] BYREF
  unsigned int i; // [esp+100h] [ebp-Ch]
  LPCSTR pBuf; // [esp+104h] [ebp-8h]
  int v11; // [esp+108h] [ebp-4h]

  ONCELOG("[HNH:SUB] ParseOds3DON_483704\n");
  v11 = 0;
  pBuf = (LPCSTR)*ppBuf;
  if ( !pAnimSet->nCount_148 )
  {
    pBuf += 4;
    pAnimSet->nCount_148 = *(_DWORD *)pBuf;
    pBuf += 4;
    pAnimSet->arroS118_14C = (CHanaObj_S118 *)MemAlloc_40DEF7(280 * pAnimSet->nCount_148 + 128);
    for ( i = 0; i < pAnimSet->nCount_148; ++i )
    {
      pAnimSet->arroS118_14C[i].flt_104 = *(float *)pBuf;
      pBuf += 4;
      pAnimSet->arroS118_14C[i].flt_108 = *(float *)pBuf;
      pBuf += 4;
      pAnimSet->arroS118_14C[i].field_10C = *(_DWORD *)pBuf;
      pBuf += 4;
      len_name1a = lstrlenA(pBuf);
      memcpy(pAnimSet->arroS118_14C[i].sz_name_4, pBuf, len_name1a + 1);
      wsprintfA(pszFileName, "%s", pBuf);
      len_name1b = lstrlenA(pBuf);
      pBuf += len_name1b + 1;
      CHanaObj__GetRealPath_486D9A(pszFileName, 1009);
      pSound = LoadWavToDS3DBuffer_42277E(pszFileName);
      pAnimSet->arroS118_14C[i].pSnd_0 = pSound;
      pTheFrame = CHanaFrameT3EC__FindByName_483650(pFrame, pBuf);
      pAnimSet->arroS118_14C[i].pFrame_114 = pTheFrame;
      if ( pAnimSet->arroS118_14C[i].pFrame_114 )
        CHanaObj__AddRef_40BFC3(pAnimSet->arroS118_14C[i].pFrame_114);
      len_name2 = lstrlenA(pBuf);
      pBuf += len_name2 + 1;
    }
  }
  *ppBuf = (LPBYTE)pBuf;
}

// CHanaFrameT3EC::FindByName - recursive search of a CHanaFrame node tree; returns the first descendant whose szName_8 matches lpName (case-insensitive prefix of its length), else NULL.
CHanaFrameT3EC_S354 *__cdecl hook_CHanaFrameT3EC__FindByName_483650(CHanaFrameT3EC_S354 *pRoot, LPCSTR lpString)
{
  size_t namelen; // eax
  CHanaFrameT3EC_S354 *pFoundFrame; // [esp+0h] [ebp-10h]
  unsigned int i; // [esp+4h] [ebp-Ch]
  CHanaNode *pFirstChildNode; // [esp+Ch] [ebp-4h]
  ONCELOG("[HNH:SUB] CHanaFrameT3EC__FindByName_483650\n");
  pFoundFrame = 0;
  namelen = lstrlenA(lpString);
  if ( !_strnicmp(pRoot->szName_8, lpString, namelen) )
    return pRoot;
  if ( pRoot->nCountChild )
  {
    pFirstChildNode = pRoot->pFirstChildNode;
    for ( i = 0; i < pRoot->nCountChild; ++i )
    {
      if ( pFirstChildNode->pData )
      {
        pFoundFrame = CHanaFrameT3EC__FindByName_483650(pFirstChildNode->pData, lpString);
        if ( pFoundFrame )
          break;
      }
      pFirstChildNode = pFirstChildNode->pNext;
    }
  }
  return pFoundFrame;
}

BOOL __cdecl hook_LoadOdsEx_48564B(
  LPCSTR lpFileName,
  CHanaSceneT458_S504 **ppScene,
  CHanaFrameT3EC_S354 **arrFrames,
  CHanaAnimSetT3F1_S150 **arrAnimeSets,
  int dwCount)
{
  int skipHeader; // eax
  CHanaSceneT458_S504 *pScene; // [esp-10h] [ebp-220h]
  HANDLE hFile; // [esp+0h] [ebp-210h]
  CHAR szODSHeader[512]; // [esp+4h] [ebp-20Ch] BYREF
  DWORD nNumberOfBytesToRead; // [esp+204h] [ebp-Ch]
  LPBYTE lpBuffer; // [esp+208h] [ebp-8h]
  DWORD NumberOfBytesRead; // [esp+20Ch] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] LoadOdsEx_48564B\n");

  lpBuffer = 0;
  CHanaObj__NewSceneT458_S504(ppScene);
  AttachHanaObj_40BAB8((*ppScene)->pAnimSet_148, lpFileName);
  hFile = CreateFileA(lpFileName, GENERIC_READ, 0, 0, 3u, 0, 0);
  NumberOfBytesRead = 0;
  if ( !hFile )
    return 0;
  if ( hFile == (HANDLE)INVALID_HANDLE_VALUE )
    return 0;
  nNumberOfBytesToRead = GetFileSize(hFile, 0);
  if ( !nNumberOfBytesToRead )
    return 0;
  if ( nNumberOfBytesToRead == -1 )
    return 0;
  lpBuffer = (LPBYTE)MemAlloc_40DEF7(nNumberOfBytesToRead + 128);
  wsprintfA(szODSHeader, "新フォーマットＯＤＳ%c%c%c", 13, 10, 0);
  ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, &NumberOfBytesRead, 0);
  CloseHandle(hFile);
  if ( !lstrcmpA(szODSHeader, (LPCSTR)lpBuffer) )
  {
    pScene = *ppScene;
    skipHeader = lstrlenA(szODSHeader);
    ParseOdsEx_484A38((LPCSTR)&lpBuffer[skipHeader + 1], pScene, arrFrames, arrAnimeSets, dwCount);
  }
  if ( lpBuffer )
  {
    MemFree_40DF64(lpBuffer);
    lpBuffer = 0;
  }
  if ( *ppScene )
    ResetSceneBodies_4857CA(*ppScene);
  MtxStackStartMultLocal_4074EF();
  return 1;
}


// Binary ODS scene-script parser (tag-driven, lpBuffer walks the raw bytes). Stages:
// (1) TEXT: scene name + loads the dialogue string file (GetRealPath id 1004 -> ReadStrFile).
// (2) SIZE: dwCount_130 = N; allocs the 5 scene arrays refsFrames_134 / refsAnimeSets_138 / refsSekBody_13C / arrSizes_144 / arrSekItemProps_140 (4*N+64 each).
// (3) NAME (per entry, nSlot++): reuses a PRELOADED frame+animset when the tag string case-insensitively equals a frame name in arrFrames[0..dwCount) (AddRef + attach to scene root, bReused=1), else loads a fresh .odf (ParseODFFile + name both objects).
// (4) ANIM: track data into pAnimSet_148 for the PREVIOUS entry's frame. SEKN: ParseSekFile -> CSekBody for the previous entry; reads the item-prop table (dword count + 8-byte {flt,index} props) and wires body->pSekHeader_220 {pAnimSetT3F1, pFrameT3EC} to the entry's pair, AddRefs, PlayAnim(0). Stage ends at 'サウンド' (sound) / 'ファイルは終了' (EOF) / EYESANIM.
// (5) EYESANIM: camera track on g_pCameraFrame. EYE2: finds an eye node - scans each entry frame name for substring 'eye' (mChar loop, limit len-6); the frame is TEMPORARILY renamed 'XXXX' so FindFrameByName(subtree, 'eye') cannot match the root itself, backup restored after; hit => pScene->by_474=1, pCamFrame_478=pEyeFrame. 'Light': memcpy D3DLIGHT7 (buffer +104), NewLightT3EF + AssignLigthtToFrame(g_pLightFrame) + light anim track; named 'Light_%d_%d' from two rand()s (unique key). Stage ends at 3DON / サウンド / EOF.
// (6) 3DON: group-anim trick - creates temp pGroupFrame under the scene root, reparents ALL entry frames under it (nReparent loop), parses the 3DON track against the group, re-attaches frames to the scene root (iiReparent) and frees the temp frame.
// Final terminator: 'ファイルは終了'. v27/v30/v31/v33 zero-inits are dead.
void __cdecl hook_ParseOdsEx_484A38(
  LPCSTR lpBuffer,
  CHanaSceneT458_S504 *pScene,
  CHanaFrameT3EC_S354 **arrFrames,
  CHanaAnimSetT3F1_S150 **arrAnimeSets,
  int dwCount)
{
  int v5; // eax
  int v6; // eax
  int v7; // eax
  int v8; // eax
  int v9; // eax
  int v10; // eax
  int v11; // [esp-4h] [ebp-280h]
  unsigned int nReparent; // [esp+4h] [ebp-278h]
  unsigned int iiReparent; // [esp+4h] [ebp-278h]
  CHanaFrameT3EC_S354 *pGroupFrame; // [esp+8h] [ebp-274h] BYREF
  CHanaLightT3EF_S1A0 *pLight; // [esp+Ch] [ebp-270h] BYREF
  D3DLIGHT7 pD3Dlight; // [esp+10h] [ebp-26Ch] BYREF
  CHanaFrameT3EC_S354 *pEyeFrame; // [esp+78h] [ebp-204h]
  CHAR szNameBackup[200]; // [esp+7Ch] [ebp-200h] BYREF
  unsigned int kFrame; // [esp+144h] [ebp-138h]
  int nNameScanLen; // [esp+148h] [ebp-134h]
  int bEyeFound; // [esp+14Ch] [ebp-130h]
  int mChar; // [esp+150h] [ebp-12Ch]
  DWORD jItem; // [esp+154h] [ebp-128h]
  CSekBody *pSekBody; // [esp+158h] [ebp-124h]
  int bReused; // [esp+15Ch] [ebp-120h]
  int iPreload; // [esp+160h] [ebp-11Ch]
  int v27; // [esp+164h] [ebp-118h]
  int nSlot; // [esp+168h] [ebp-114h]
  CHAR szPath[256]; // [esp+16Ch] [ebp-110h] BYREF
  int v30; // [esp+26Ch] [ebp-10h]
  int v31; // [esp+270h] [ebp-Ch]
  int nCount; // [esp+274h] [ebp-8h]
  int v33; // [esp+278h] [ebp-4h]

  ONCELOG("[HNH:SUB] ParseOdsEx_484A38\n");

  nSlot = 0;
  v27 = 0;
  v30 = 0;
  v33 = 0;
  v31 = 0;
  MtxStackStartMultLocal_4074EF();
  while ( 1 )
  {
    if ( !memcmp(lpBuffer, "TEXT", 4u) )
    {
      lpBuffer += 8;
      wsprintfA(pScene->szName_160, "%s", lpBuffer);
      wsprintfA(szPath, "%s", lpBuffer);
      CHanaObj__GetRealPath_486D9A(szPath, 1004);
      v5 = lstrlenA(lpBuffer);
      lpBuffer += v5;
      ReadStrFile_483567(pScene, szPath);
      goto LABEL_6;
    }
    if ( !memcmp(lpBuffer, "SIZE", 4u) )
      break;
  LABEL_6:
    ++lpBuffer;
  }
  lpBuffer += 4;
  nSlot = 0;
  nCount = *(_DWORD *)lpBuffer;
  pScene->dwCount_130 = nCount;
  pScene->refsFrames_134 = (CHanaFrameT3EC_S354 **)MemAlloc_40DEF7(4 * nCount + 64);
  pScene->refsAnimeSets_138 = (CHanaAnimSetT3F1_S150 **)MemAlloc_40DEF7(4 * nCount + 64);
  pScene->refsSekBody_13C = (CSekBody **)MemAlloc_40DEF7(4 * nCount + 64);
  pScene->arrSizes_144 = (DWORD *)MemAlloc_40DEF7(4 * nCount + 64);
  pScene->arrSekItemProps_140 = (CSekItemProp **)MemAlloc_40DEF7(4 * nCount + 64);
  while ( 1 )
  {
    while ( 1 )
    {
      while ( 1 )
      {
        while ( !memcmp(lpBuffer, "NAME", 4u) )
        {
          bReused = 0;
          lpBuffer += 4;
          wsprintfA(szPath, "%s", lpBuffer);
          CHanaObj__GetRealPath_486D9A(szPath, 1005);
          for ( iPreload = 0; iPreload < dwCount; ++iPreload )
          {
            v6 = lstrlenA(lpBuffer);
            if ( !_memicmp(lpBuffer, arrFrames[iPreload]->szName_8, v6 + 1) )
            {
              pScene->refsFrames_134[nSlot] = arrFrames[iPreload];
              pScene->refsAnimeSets_138[nSlot] = arrAnimeSets[iPreload];
              CHanaObj__AddRef_40BFC3(pScene->refsFrames_134[nSlot]);
              CHanaObj__AddRef_40BFC3(pScene->refsAnimeSets_138[nSlot]);
              AttachChild_4055FD(g_pSceneFrame, pScene->refsFrames_134[nSlot]);
              bReused = 1;
              break;
            }
          }
          if ( !bReused )
          {
            ParseODFFile_441F19(szPath, &pScene->refsFrames_134[nSlot], &pScene->refsAnimeSets_138[nSlot]);
            AttachHanaObj_40BAB8(pScene->refsFrames_134[nSlot], lpBuffer);
            AttachHanaObj_40BAB8(pScene->refsAnimeSets_138[nSlot], lpBuffer);
          }
          ++nSlot;
          v7 = lstrlenA(lpBuffer);
          lpBuffer += v7 + 1;
        }
        if ( memcmp(lpBuffer, "ANIM", 4u) )
          break;
        lpBuffer += 4;
        if ( pScene->refsFrames_134[nSlot - 1] )
          ParseOdfANIM_4414EB((LPBYTE *)&lpBuffer, pScene->pAnimSet_148, pScene->refsFrames_134[nSlot - 1]);
      }
      if ( memcmp(lpBuffer, "SEKN", 4u) )
        break;
      lpBuffer += 4;
      wsprintfA(szPath, "%s", lpBuffer);
      CHanaObj__GetRealPath_486D9A(szPath, 1005);
      pSekBody = ParseSekFile_401039(szPath);
      if ( pSekBody )
      {
        wsprintfA(pSekBody->szName_0, "%s", lpBuffer);
        pScene->refsSekBody_13C[nSlot - 1] = pSekBody;
        v9 = lstrlenA(lpBuffer);
        lpBuffer += v9 + 1;
        if ( pScene->refsSekBody_13C )
        {
          pScene->arrSizes_144[nSlot - 1] = *(_DWORD *)lpBuffer;
          lpBuffer += 4;
          pScene->arrSekItemProps_140[nSlot - 1] = (CSekItemProp *)MemAlloc_40DEF7(8 * pScene->arrSizes_144[nSlot - 1] + 256);
          for ( jItem = 0; jItem < pScene->arrSizes_144[nSlot - 1]; ++jItem )
          {
            pScene->arrSekItemProps_140[nSlot - 1][jItem].flt_0 = *(float *)lpBuffer;
            pScene->arrSekItemProps_140[nSlot - 1][jItem].index = *((_DWORD *)lpBuffer + 1);
            lpBuffer += 8;
          }
          pScene->refsSekBody_13C[nSlot - 1]->pSekHeader_220->pAnimSetT3F1 = pScene->refsAnimeSets_138[nSlot - 1];
          pScene->refsSekBody_13C[nSlot - 1]->pSekHeader_220->pFrameT3EC = pScene->refsFrames_134[nSlot - 1];
          CHanaObj__AddRef_40BFC3(pScene->refsAnimeSets_138[nSlot - 1]);
          CHanaObj__AddRef_40BFC3(pScene->refsFrames_134[nSlot - 1]);
          CSekBody__PlayAnim_40179D(pScene->refsSekBody_13C[nSlot - 1], 0);
        }
      }
      else
      {
        v8 = lstrlenA(lpBuffer);
        lpBuffer += v8 + 1;
      }
    }
    if ( !memcmp(lpBuffer, "サウンド", 8u)
      || !memcmp(lpBuffer, "ファイルは終了", 0xEu)
      || !memcmp(lpBuffer, "EYESANIM", 8u) )
    {
      break;
    }
    ++lpBuffer;
  }
  while ( 1 )
  {
    while ( 1 )
    {
      while ( 1 )
      {
        while ( !memcmp(lpBuffer, "EYESANIM", 8u) )
        {
          lpBuffer += 8;
          if ( pScene->pAnimSet_148 )
            ParseOdfANIM_4414EB((LPBYTE *)&lpBuffer, pScene->pAnimSet_148, g_pCameraFrame);
        }
        if ( memcmp(lpBuffer, "EYE2", 4u) )
          break;
        bEyeFound = 0;
        for ( kFrame = 0; kFrame < pScene->dwCount_130; ++kFrame )
        {
          nNameScanLen = lstrlenA(pScene->refsFrames_134[kFrame]->szName_8) - 6;
          for ( mChar = 0; mChar < nNameScanLen; ++mChar )
          {
            if ( !_memicmp(&pScene->refsFrames_134[kFrame]->szName_8[mChar], "eye", 3u) )
            {
              pEyeFrame = 0;
              wsprintfA(szNameBackup, "%s", pScene->refsFrames_134[kFrame]->szName_8);
              wsprintfA(pScene->refsFrames_134[kFrame]->szName_8, "XXXX");
              pEyeFrame = FindFrameByName_47E999(pScene->refsFrames_134[kFrame], "eye");
              wsprintfA(pScene->refsFrames_134[kFrame]->szName_8, "%s", szNameBackup);
              if ( pEyeFrame )
              {
                bEyeFound = 1;
                pScene->by_474 = 1;
                pScene->pCamFrame_478 = pEyeFrame;
                break;
              }
            }
          }
          if ( bEyeFound == 1 )
            break;
        }
        lpBuffer += 4;
      }
      if ( memcmp(lpBuffer, "Light", 5u) )
        break;
      lpBuffer += 5;
      memcpy(&pD3Dlight, lpBuffer, sizeof(pD3Dlight));
      CHanaObj__NewLightT3EF(&pLight, &pD3Dlight);
      AssignLigthtToFrame_40539D(g_pLightFrame, pLight);
      if ( pLight )
      {
        CHanaObj__Free_40C1E4(pLight);
        pLight = 0;
      }
      lpBuffer += 104;
      lpBuffer += 4;
      ParseOdfANIM_4414EB((LPBYTE *)&lpBuffer, pScene->pAnimSet_148, g_pLightFrame);
      v11 = rand() % 100;
      v10 = rand();
      wsprintfA(szPath, "Light_%d_%d", v10 % 100, v11);
      AttachHanaObj_40BAB8(g_pLightFrame, szPath);
    }
    if ( !memcmp(lpBuffer, "3DON", 4u)
      || !memcmp(lpBuffer, "サウンド", 8u)
      || !memcmp(lpBuffer, "ファイルは終了", 0xEu) )
    {
      break;
    }
    ++lpBuffer;
  }
  while ( 1 )
  {
    while ( !memcmp(lpBuffer, "3DON", 4u) )
    {
      if ( pScene->pAnimSet_148 )
      {
        pGroupFrame = 0;
        CHanaObj__NewFrameT3EC_S354((CHanaFrameT3EC_S354 *)g_pSceneFrame, &pGroupFrame);
        for ( nReparent = 0; nReparent < pScene->dwCount_130; ++nReparent )
        {
          if ( pScene->refsFrames_134[nReparent] )
            AttachChild_4055FD(pGroupFrame, pScene->refsFrames_134[nReparent]);
        }
        ParseOds3DON_483704((LPBYTE *)&lpBuffer, pScene->pAnimSet_148, pGroupFrame);
        for ( iiReparent = 0; iiReparent < pScene->dwCount_130; ++iiReparent )
        {
          if ( pScene->refsFrames_134[iiReparent] )
            AttachChild_4055FD(g_pSceneFrame, pScene->refsFrames_134[iiReparent]);
        }
        if ( pGroupFrame )
        {
          CHanaObj__Free_40C1E4(pGroupFrame);
          pGroupFrame = 0;
        }
      }
    }
    if ( !memcmp(lpBuffer, "ファイルは終了", 14u) )
      break;
    ++lpBuffer;
  }
}

BOOL __cdecl hook_LoadSekAndOdf_4445DD(LPCSTR pszSekFile, LPCSTR pszOdfFile, CSekBody **ppOutBody)
{
  CSekBody *pSekBody; // [esp+0h] [ebp-4h]
  ONCELOG("[HNH:SUB] LoadSekAndOdf_4445DD\n");
 
  pSekBody = ParseSekFile_401039(pszSekFile);
  if ( pSekBody && pSekBody->pSekHeader_220 )
  {
    SekBody__UpdateTo_4014E4(pSekBody, 0);
    ParseODFFile_441F19(pszOdfFile, &pSekBody->pSekHeader_220->pFrameT3EC, &pSekBody->pSekHeader_220->pAnimSetT3F1);
    if ( pSekBody->pSekHeader_220->pFrameT3EC )
    {
      AttachChild_4055FD(g_pSceneFrame, pSekBody->pSekHeader_220->pFrameT3EC);
      SetFramePosRel_4069DD(pSekBody->pSekHeader_220->pFrameT3EC, (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0, 0.0, 0.0);
      UpdateBoneLookAtTransform(
        pSekBody->pSekHeader_220->pFrameT3EC,
        (CHanaFrameT3EC_S354 *)g_pSceneFrame,
        0.0,
        0.0,
        1.0,
        0.0,
        1.0,
        0.0);
      CHanaAnimSetT3F1__SetTime_403D61(pSekBody->pSekHeader_220->pAnimSetT3F1, 1.0);
      AttachHanaObj_40BAB8(pSekBody->pSekHeader_220->pFrameT3EC, pszOdfFile);
      AttachHanaObj_40BAB8(pSekBody->pSekHeader_220->pAnimSetT3F1, pszOdfFile);
    }
    else
    {
      CSekBody__Release_4012FE(pSekBody);
      pSekBody = 0;
    }
    *ppOutBody = pSekBody;
    return 1;
  }
  else
  {
    *ppOutBody = 0;
    return 0;
  }
}


void __cdecl hook_SekBody__UpdateTo_4014E4(CSekBody *pSekBody, int nItem)
{
  ONCELOG("[HNH:SUB] SekBody__UpdateTo_4014E4\n");

  if ( pSekBody->field_200 != nItem && nItem < 128 )
  {
    if ( !pSekBody->items_250[nItem].field_7C )
      pSekBody->float_21C = 1.0;
    pSekBody->field_200 = nItem;
    pSekBody->field_208 = nItem;
    pSekBody->items_250[nItem].field_44 = 0;
    pSekBody->items_250[nItem].field_6C = 0;
    pSekBody->items_250[nItem].field_64 = 0;
    if ( pSekBody->items_250[nItem].field_50 )
      pSekBody->items_250[nItem].float_5C = (pSekBody->items_250[nItem].float_58 - pSekBody->items_250[nItem].float_54)
      / (double)pSekBody->items_250[nItem].field_50;
    else
      pSekBody->items_250[nItem].float_5C = 0.0;
    pSekBody->items_250[nItem].float_60 = pSekBody->items_250[nItem].float_54;
    pSekBody->field_244 = 0;
    pSekBody->field_248 = 0;
  }
}


void register_ParseODS_483932(int cfg) {
  if (cfg == 1) {
    ParseODS_483932 = hook_ParseODS_483932;
    LoadOdsEx_48564B = hook_LoadOdsEx_48564B;
    LoadSekAndOdf_4445DD = hook_LoadSekAndOdf_4445DD;
    ReadStrFile_483567 = hook_ReadStrFile_483567;
    ParseODFFile_441F19 = hook_ParseODFFile_441F19;
    ParseSekFile_401039 = hook_ParseSekFile_401039;
    CSekBody__PlayAnim_40179D = hook_CSekBody__PlayAnim_40179D;
    FindFrameByName_47E999 = hook_FindFrameByName_47E999;
    ParseOds3DON_483704 = hook_ParseOds3DON_483704;
    HanaMesh_AddVertexReference = hook_HanaMesh_AddVertexReference;
    HanaMorpT457_SetupFunsSubtree = hook_HanaMorpT457_SetupFunsSubtree;
    CHanaObj__NewMeshT3F5 = hook_CHanaObj__NewMeshT3F5;
    GetMeshT3F5FromObj_434FE0 = hook_GetMeshT3F5FromObj_434FE0;
    HanaMorpT457_SetupFuns = hook_HanaMorpT457_SetupFuns;
    CHanaObj__NewAnimSetT3F1_S150 = hook_CHanaObj__NewAnimSetT3F1_S150;
    HanaFrame_PropagateFogToChildren = hook_HanaFrame_PropagateFogToChildren;
    SekHeader_CloneClean = hook_SekHeader_CloneClean;
    CHanaFrameT3EC__FindByName_483650 = hook_CHanaFrameT3EC__FindByName_483650;
    ParseOdsEx_484A38 = hook_ParseOdsEx_484A38;
    SekBody__UpdateTo_4014E4 = hook_SekBody__UpdateTo_4014E4;
    HanaVert_CopyMorph = hook_HanaVert_CopyMorph;
    HanaVert_LerpMorph = hook_HanaVert_LerpMorph;
  }

  if (cfg != 2) hnh_HookOff();
  hnh_Hook(0x48564b, hook_LoadOdsEx_48564B, "LoadOdsEx_48564B");
  hnh_Hook(0x484a38, hook_ParseOdsEx_484A38, "ParseOdsEx_484A38");

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
  hnh_Hook(0x40d875, hook_SekHeader_CloneClean, "SekHeader_CloneClean_40D875"); // [OK]
  hnh_Hook(0x40179d, hook_CSekBody__PlayAnim_40179D, "CSekBody__PlayAnim_40179D"); // [OK]
  hnh_Hook(0x483650, hook_CHanaFrameT3EC__FindByName_483650, "CHanaFrameT3EC__FindByName_483650"); // [OK]
  hnh_Hook(0x40fceb, hook_HanaMesh_AddVertexReference, "HanaMesh_AddVertexReference_40FCEB"); // [OK]
  hnh_Hook(0x401039, hook_ParseSekFile_401039, "ParseSekFile_401039"); // [OK]
  hnh_Hook(0x483704, hook_ParseOds3DON_483704, "ParseOds3DON_483704"); // [OK]
  hnh_Hook(0x4445dd, hook_LoadSekAndOdf_4445DD, "LoadSekAndOdf_4445DD"); // [OK]
  hnh_Hook(0x4014e4, hook_SekBody__UpdateTo_4014E4, "SekBody__UpdateTo_4014E4"); // [OK]
  hnh_HookOn();
}
