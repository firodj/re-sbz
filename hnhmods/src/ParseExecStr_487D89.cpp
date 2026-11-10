// GAME: [011019][TEATIME] ‰Ø‚Ì”ß–Â@`‰ó‚ê‚½ÉŽq‚ÌS`
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"
#include "hnh_hook.hpp"

// ParseExecStr_487D89 -- the ODS scene-script interpreter. Runs chunks from
// pScene->ptrStrData_260 (cursor dwPtrCurrent_448, restart mark dwPtrPos_444,
// end dwPtrSize_44C) until the script clock catches up, a chunk pauses, or
// END;. Returns 0 on END (scene over), 1 normal, >1 if an IF was evaluated.
//
// GATES (loop head):
//   cursor past end, or clock caught up (exec <= WCurNum != 0)  -> return
//   by_460/by_461 (dialogue showing)  -> park WCurNum into
//     pAnimSet->field_130, exec = that, return
//   newlines skipped; W<val>; sets the next wait time WCurNum. The time
//   gate passes when WCurNum >= flt_464 (or flt_464 == 0); otherwise exec
//   is pushed to flt_464 and the loop returns to the gates.
//
// CHUNKS:
//   TEXT=...;         ParseStrChunkText + by_461=by_460=1 (show dialogue)
//   HITEMGET<n>;      set bit n of sceneItemBits_98[bMaxSubStage] AND of
//                     g_sceneItemBits[2*substage] (the persistent copy @0x530238,
//                     IDA type CSceneFlag_28 -- same storage). Cursor +=2
//                     (assumes single digit + ';').
//   GETBOOK<n>;       g_byData0Dat_530170[n]=1; save-slot mask |= 1<<n
//   KUTI<i>,<kw1>,<kw2>;   mouth anim: ApplyAnimationParams(refs[i], ease=1,
//                          0, 0, kw1, kw2) if i < dwCount_130
//   ME<i>,<s>,<e>,<kw1>,<kw2>;  eye anim: (refs[i], 0, s, e, kw1, kw2)
//   M+<v> / M-<v>;    flt_458 += delta (atof keeps the sign); M=<v>;
//                     flt_454 = v (the dialogue text alpha multiplier used
//                     by DrawDialogueText)
//   GO,R,G,B_<t>; / GI,...;  PlayScreenFade(in=GO, R|G<<8|B<<16|0xFF000000,
//                     pSurface_468, t, by_462). The /255*255 double math in
//                     the original is an identity -- kept as plain values.
//   G<NN>;            gallery background switch: load aG00Gp[NN] ("GNN.GP",
//                     String 50[100] @0x4C9FB0, GRP dir); pictures 30/92/71/
//                     51/11 also set bMaxSubStage=bCurrentSubStage=0..4
//                     (the five girls); flt_480=0.1 restarts the crossfade;
//                     pUnk_47C = old bg (released; NULL if NN > 95);
//                     g_arrGalleryUnlocked_5301A2[NN]=1 (gallery unlock!)
//   #<snd>,<vol>;     scene BGM: stop/release old pSndBuf_46C, LoadWav
//                     (WAV dir), field_470 = vol/2 (the volume offset
//                     SystemMenu receives), SetVolume clamped [-10000,0]
//                     via g_fVolumeDbTable[g_nAviVolumeIndex], Play looping
//   N<n>;             by_462 = n & 1 (flush flag: RenderFlush at entry +
//                     bFlush for fades)
//   FLG<ofs><op><v>;  byte write at &sceneState.bCurrentSubStage + ofs
//                     (offset 1 into CSceneState!) with op = /+=/-=/*=//=
//                     (op scanned over the next 20 bytes, 2-byte ops matched
//                     from cur-2)
//   GOTO:<label>;     jump: label captured INCLUDING ";\n" (up to 255
//                     bytes), searched forward for '\n'+label, then
//                     backward if the forward scan hit the NUL; cursor set
//                     past the match; FrameTimer_BeginFrame
//   IF(FLG<i><op><v>)GOTO:<l>;  compare sceneState byte 1+i with
//                     ==/!=/>=/>/<=/< ; true -> same label jump; ++bResult
//   IF(OFLG<i><op><v>)GOTO:<l>; same vs g_byData0Dat_530170[i] (persistent)
//   F<t>;             exec = t
//   SELECT=...*       by_463=1 (selection mode -- ParseExecStr becomes a
//                     no-op until cleared), WCurNum = pAnimSet->field_130,
//                     flt_458 = -0.05, Scene_CreateFontTextureFromStringData,
//                     cursor past '*'; return
//   END; / NUL        return 0
//   LPU;              protagonist HP += 20% of max, clamped
//   anything else: full-width space / A-z skipped one byte; other control
//   bytes (<= 8, outside A-z) return

// === PINNED GAME FUNCTIONS ===
BOOL (__cdecl*ParseExecStr_487D89)(CHanaSceneT458_S504 *pScene, float *pfExecTime) =
    reinterpret_cast<BOOL(__cdecl*)(CHanaSceneT458_S504 *, float *)>(0x487D89);

// === PINNED GLOBALS ===
// "G00.GP" picture-name table, 50-char stride (same layout as String 50)

// String 50 (&g_aG00Gp)[100] =  *(String 50 (*)[100])(0x4C9FB0);  // replaced by compiled-in g_aG00Gp_4C9FB0

void GotoLabel107(CHanaSceneT458_S504 *pScene) {
  int cchLabel; // [esp+2Ch] [ebp-244h]
  BYTE szLabel[256]; // [esp+64h] [ebp-20Ch] BYREF
  int i; // [esp+164h] [ebp-10Ch]

//LABEL_107:
  for ( cchLabel = 0; cchLabel < 255; ++cchLabel )
  {
    szLabel[cchLabel] = pScene->ptrStrData_260[pScene->dwPtrCurrent_448];
    if ( pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 1] == ';' )
      break;
    ++pScene->dwPtrCurrent_448;
  }
  for ( i = pScene->dwPtrCurrent_448; pScene->ptrStrData_260[i]; ++i )
  {
    if ( pScene->ptrStrData_260[i] == 10
      && !memcmp(&pScene->ptrStrData_260[i + 1], szLabel, cchLabel) )
    {
      pScene->dwPtrCurrent_448 = i + 1;
      break;
    }
  }
  if ( !pScene->ptrStrData_260[i] )
  {
    for ( i = pScene->dwPtrCurrent_448; i >= 0; --i )
    {
      if ( pScene->ptrStrData_260[i] == 10
        && !memcmp(&pScene->ptrStrData_260[i + 1], szLabel, cchLabel) )
      {
        pScene->dwPtrCurrent_448 = i + 1;
        break;
      }
    }
  }
  pScene->dwPtrCurrent_448 += cchLabel;
  FrameTimer_BeginFrame();
  pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
}

// WARNING - there seems a mislogic that the bOut is not reseat to 0 before any checking.
void CompareByte(CHanaSceneT458_S504 *pScene, char bValue, bool *bOut) {
  __int16 nCmpVal; // [esp+26Ch] [ebp-4h]
  
  if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "==", 2u) )
  {
    pScene->dwPtrCurrent_448 += 2;
    nCmpVal = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
    if ( bValue == nCmpVal )
      *bOut = 1;
  }
  else if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "!=", 2u) )
  {
    pScene->dwPtrCurrent_448 += 2;
    nCmpVal = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
    if ( bValue != nCmpVal )
      *bOut = 1;
  }
  else if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], ">=", 2u) )
  {
    pScene->dwPtrCurrent_448 += 2;
    nCmpVal = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
    if ( bValue >= nCmpVal )
      *bOut = 1;
  }
  else if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], ">", 1u) )
  {
    nCmpVal = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
    if ( bValue > nCmpVal )
      *bOut = 1;
  }
  else if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "<=", 2u) )
  {
    pScene->dwPtrCurrent_448 += 2;
    nCmpVal = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
    if ( bValue <= nCmpVal )
      *bOut = 1;
  }
  else if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "<", 1u) )
  {
    nCmpVal = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
    if ( bValue < nCmpVal )
      *bOut = 1;
  }
}

BOOL __cdecl hook_ParseExecStr_487D89(CHanaSceneT458_S504 *pScene, float *pfExecTime)
{
  int v2; // eax
  int bMaxSubStage; // ecx
  int v4; // eax
  int v5; // ecx
  __int16 nBookIdx; // ax
  DWORD nBgLoadW; // eax
  const char *pszColR; // [esp-4h] [ebp-274h]
  const char *pszColG; // [esp-4h] [ebp-274h]
  const char *pszColB; // [esp-4h] [ebp-274h]
  const char *pszFadeTime; // [esp-4h] [ebp-274h]
  DWORD nBgLoadH; // [esp-4h] [ebp-274h]
  bool bOfCond; // [esp+24h] [ebp-24Ch]
  bool bFlgCond; // [esp+28h] [ebp-248h]

  __int16 nFlgOp; // [esp+30h] [ebp-240h]
  __int16 nFadeG; // [esp+34h] [ebp-23Ch]
  float fFadeTime; // [esp+38h] [ebp-238h]
  __int16 nFadeR; // [esp+3Ch] [ebp-234h]
  __int16 nFadeB; // [esp+40h] [ebp-230h]
  int nMeKw2; // [esp+44h] [ebp-22Ch]
  int nMeEnd; // [esp+48h] [ebp-228h]
  int nMeKw1; // [esp+4Ch] [ebp-224h]
  int nMeStart; // [esp+50h] [ebp-220h]
  int nKutiKw2; // [esp+54h] [ebp-21Ch]
  int nKutiKw1; // [esp+58h] [ebp-218h]
  __int16 nItemBit; // [esp+5Ch] [ebp-214h]
  __int16 nKutiIdx; // [esp+5Ch] [ebp-214h]
  __int16 nMeIdx; // [esp+5Ch] [ebp-214h]
  _BOOL2 bisGo; // [esp+5Ch] [ebp-214h]
  __int16 nGpIndex; // [esp+5Ch] [ebp-214h]
  __int16 sndFileIndex_v33; // [esp+5Ch] [ebp-214h]
  char nFlgVal; // [esp+5Ch] [ebp-214h]
  __int16 sndVolIndex_v35; // [esp+60h] [ebp-210h]
  __int16 nFlgOfs; // [esp+60h] [ebp-210h]
  __int16 nFlgIdx; // [esp+60h] [ebp-210h]
  __int16 nOFlgIdx; // [esp+60h] [ebp-210h]

  int i; // [esp+164h] [ebp-10Ch]
  CHAR szFileName[256]; // [esp+168h] [ebp-108h] BYREF
  int bResult; // [esp+268h] [ebp-8h]
  __int16 nCmpVal; // [esp+26Ch] [ebp-4h]


  ONCELOG("[HNH:SUB] ParseExecStr_487D89\n");


  bResult = 1;
  if ( pScene && pScene->by_462 )
    RenderFlush_4874A5();

  if (!pScene || !pScene->ptrStrData_260)
    return bResult;

  if ( pScene->by_463 == 1 )
  {
    *pfExecTime = pScene->WCurNum;
    return bResult;
  }

  while ( 1 ) // g
  {
    if ( pScene->dwPtrCurrent_448 >= (unsigned int)pScene->dwPtrSize_44C
      || *pfExecTime <= (double)pScene->WCurNum && pScene->WCurNum != 0.0 )
    {
      return bResult;
    }
    if ( pScene->by_460 || pScene->by_461 )// skip reading further
    {
      pScene->pAnimSet_148->field_130 = pScene->WCurNum;
      *pfExecTime = pScene->pAnimSet_148->field_130;
      return bResult;
    }
    if (pScene->ptrStrData_260[pScene->dwPtrCurrent_448 + 1] == '\n') {
      pScene->dwPtrCurrent_448 += 2;
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      continue;
    }
    if (pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == '\n') {
      pScene->dwPtrPos_444 = ++pScene->dwPtrCurrent_448;
      continue;
    }
    if (pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == 'W') {
      pScene->WCurNum = atof(&pScene->ptrStrData_260[++pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ';',
        0)
        + 1;
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      continue;
    }
    if (pScene->WCurNum < (double)pScene->flt_464 && pScene->flt_464 != 0.0) {
      ++pScene->dwPtrCurrent_448;
      *pfExecTime = pScene->flt_464;
      continue;
    } 
    if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "TEXT=", 5u) ) {
      pScene->dwPtrCurrent_448 += 5;// chunk == 'TEXT='
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      ParseStrChunkText_47FF01(pScene);
      pScene->by_461 = 1;
      pScene->by_460 = 1;
      FrameTimer_EndFrame();
      continue;
    }
    if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "HITEMGET", 8u) ) {
      pScene->dwPtrCurrent_448 += 8;// chunk == 'HITEMGET'
      nItemBit = 1 << atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      v2 = ((unsigned __int64)nItemBit >> 32)
        | g_sceneState_52F7E0.sceneItemBits_98[2 * (unsigned __int8)g_sceneState_52F7E0.bMaxSubStage
        + 1];
      bMaxSubStage = (unsigned __int8)g_sceneState_52F7E0.bMaxSubStage;
      g_sceneState_52F7E0.sceneItemBits_98[2 * (unsigned __int8)g_sceneState_52F7E0.bMaxSubStage] |= nItemBit;
      g_sceneState_52F7E0.sceneItemBits_98[2 * bMaxSubStage + 1] = v2;
      v4 = ((unsigned __int64)nItemBit >> 32)
        | g_sceneItemBits[2 * (unsigned __int8)g_sceneState_52F7E0.bMaxSubStage + 1];
      v5 = (unsigned __int8)g_sceneState_52F7E0.bMaxSubStage;
      g_sceneItemBits[2 * (unsigned __int8)g_sceneState_52F7E0.bMaxSubStage] |= nItemBit;
      g_sceneItemBits[2 * v5 + 1] = v4;
      pScene->dwPtrCurrent_448 += 2;
      continue;
    }
    if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "GETBOOK", 7u) ) {
      pScene->dwPtrCurrent_448 += 7;// chunk == 'GETBOOK'
      nBookIdx = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      g_sysDataSave_530170[nBookIdx] = 1;
      g_dwUnlockedSaveSlots_530260 |= (__int16)(1 << nBookIdx);
      pScene->dwPtrCurrent_448 += 2;
      continue;
    }
    if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "KUTI", 4u) ) {
      pScene->dwPtrCurrent_448 += 4;// chunk == 'KUTI'
      nKutiIdx = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ',',
        0)
        + 1;
      nKutiKw1 = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ',',
        0)
        + 1;
      nKutiKw2 = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ';',
        0)
        + 1;
      if (pScene->dwCount_130 > (unsigned int)nKutiIdx)
        Character_ApplyAnimationParamsToChildren(
          pScene->refsFrames_134[nKutiIdx],
          1,
          0,
          0,
          nKutiKw1,
          nKutiKw2);
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      continue;
    }
    if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "ME", 2u) ) {
      pScene->dwPtrCurrent_448 += 2;// chunk == 'ME'
      nMeIdx = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        44,
        0)
        + 1;
      nMeStart = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ',',
        0)
        + 1;
      nMeEnd = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ',',
        0)
        + 1;
      nMeKw1 = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ',',
        0)
        + 1;
      nMeKw2 = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ';',
        0)
        + 1;
      if (pScene->dwCount_130 > (unsigned int)nMeIdx)
        Character_ApplyAnimationParamsToChildren(
          pScene->refsFrames_134[nMeIdx],
          0,
          nMeStart,
          nMeEnd,
          nMeKw1,
          nMeKw2);
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      continue;
    }
    if ( pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == 'M' )
    {
      ++pScene->dwPtrCurrent_448;
      if (pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == '+'
        || pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == '-')
      {
        pScene->flt_458 = atof(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      }
      else if (pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == '=')
      {
        pScene->flt_454 = atof(&pScene->ptrStrData_260[++pScene->dwPtrCurrent_448]);
      }
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ';',
        0)
        + 1;
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      continue;
    }
    if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "GO,", 3u)
      || !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "GI,", 3u) )
    {
      bisGo = memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "GO,", 3u) == 0;// chunk == 'GO'
      pScene->dwPtrCurrent_448 += 3;
      pszColR = &pScene->ptrStrData_260[pScene->dwPtrCurrent_448++];
      nFadeR = atoi(pszColR);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ',',
        0)
        + 1;
      pszColG = &pScene->ptrStrData_260[pScene->dwPtrCurrent_448++];
      nFadeG = atoi(pszColG);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ',',
        0)
        + 1;
      pszColB = &pScene->ptrStrData_260[pScene->dwPtrCurrent_448++];
      nFadeB = atoi(pszColB);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        '_',
        0)
        + 1;
      pszFadeTime = &pScene->ptrStrData_260[pScene->dwPtrCurrent_448++];
      fFadeTime = atof(pszFadeTime);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ';',
        0)
        + 1;
      PlayScreenFade(
        bisGo,
        (__int64)((double)nFadeR / 255.0 * 255.0)
        | ((unsigned int)(__int64)((double)nFadeG / 255.0 * 255.0) << 8)
        | ((unsigned int)(__int64)((double)nFadeB / 255.0 * 255.0) << 16)
        | 0xFF000000,
        pScene->pSurface_468,
        fFadeTime,
        pScene->by_462);
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      continue;
    }
    if ( pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == 'G'
      && pScene->ptrStrData_260[pScene->dwPtrCurrent_448 + 1] >= '0'
      && pScene->ptrStrData_260[pScene->dwPtrCurrent_448 + 1] <= '9' )
    {
      nGpIndex = atoi(&pScene->ptrStrData_260[++pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ';',
        0)
        + 1;
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      wsprintfA(szFileName, "%s", g_aG00Gp_4C9FB0[nGpIndex]);
      CHanaObj__GetRealPath_486D9A(szFileName, 1007);
      switch (nGpIndex)
      {
      case 30:
        g_sceneState_52F7E0.bMaxSubStage = 0;
        g_sceneState_52F7E0.bCurrentSubStage = 0;
        break;
      case 92:
        g_sceneState_52F7E0.bMaxSubStage = 1;
        g_sceneState_52F7E0.bCurrentSubStage = 1;
        break;
      case 71:
        g_sceneState_52F7E0.bMaxSubStage = 2;
        g_sceneState_52F7E0.bCurrentSubStage = 2;
        break;
      case 51:
        g_sceneState_52F7E0.bMaxSubStage = 3;
        g_sceneState_52F7E0.bCurrentSubStage = 3;
        break;
      case 11:
        g_sceneState_52F7E0.bMaxSubStage = 4;
        g_sceneState_52F7E0.bCurrentSubStage = 4;
        break;
      }
      pScene->flt_480 = 0.1;
      if (pScene->pUnk_47C)
        pScene->pUnk_47C->Release();
      if (nGpIndex <= 95)
        pScene->pUnk_47C = (IUnknown*)pScene->pSurface_468;
      else
        pScene->pUnk_47C = 0;
      g_arrGalleryUnlocked_5301A2[nGpIndex] = 1;
      nBgLoadH = ConvertFrom640AsInt(480);
      nBgLoadW = ConvertFrom640AsInt(640);
      pScene->pSurface_468 = LoadBmpScaled_4203B3(szFileName, nBgLoadW, nBgLoadH);
      FrameTimer_EndFrame();
      continue;
    }
    if ( pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == '#' )
    {
      sndFileIndex_v33 = atoi(&pScene->ptrStrData_260[++pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ',',
        0)
        + 1;
      sndVolIndex_v35 = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ';',
        0)
        + 1;
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      wsprintfA(szFileName, "%s", g_ascSndFileNames_4C78A0[sndFileIndex_v33]);
      CHanaObj__GetRealPath_486D9A(szFileName, 1006);
      if (pScene->pSndBuf_46C)
      {
        pScene->pSndBuf_46C->Stop();
        pScene->pSndBuf_46C->Release();
        pScene->pSndBuf_46C = 0;
      }
      pScene->pSndBuf_46C = LoadWav_422E7B(szFileName);
      if (pScene->pSndBuf_46C)
      {
        pScene->field_470 = sndVolIndex_v35 / 2;
        if (pScene->pSndBuf_46C)
        {
          if ((int)(pScene->field_470 + (__int64)g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266]) >= -10000)
          {
            if ((int)(pScene->field_470 + (__int64)g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266]) <= 0)
              pScene->pSndBuf_46C->SetVolume(
                pScene->field_470 + (__int64)g_fVolumeDbTable[(unsigned __int8)g_nVolumeLevel_530266]);
            else
              pScene->pSndBuf_46C->SetVolume(0);
          }
          else
          {
            pScene->pSndBuf_46C->SetVolume(-10000);
          }
        }
        pScene->pSndBuf_46C->Play(0, 0, 1);
      }
      FrameTimer_EndFrame();
      continue;
    }
    if ( pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == 'N' )
    {
      pScene->by_462 = atoi(&pScene->ptrStrData_260[++pScene->dwPtrCurrent_448]) & 1;
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[pScene->dwPtrCurrent_448],
        ';',
        0)
        + 1;
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      continue;
    }
    if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "FLG", 3u) )
    {
      nFlgOp = 0;                             // chunk == 'FLG'
      pScene->dwPtrCurrent_448 += 3;
      nFlgOfs = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      for (i = 0; i < 20; ++i)
      {
        if (!memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 2], "+=", 2u))
        {
          nFlgOp = 1;
          break;
        }
        if (!memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 2], "-=", 2u))
        {
          nFlgOp = 2;
          break;
        }
        if (!memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 2], "*=", 2u))
        {
          nFlgOp = 3;
          break;
        }
        if (!memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 2], "/=", 2u))
        {
          nFlgOp = 4;
          break;
        }
        if (!memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 1], "=", 1u))
        {
          nFlgOp = 0;
          break;
        }
        ++pScene->dwPtrCurrent_448;
      }
      nFlgVal = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      switch (nFlgOp)
      {
      case 0:
        *(&g_sceneState_52F7E0.bCurrentSubStage + nFlgOfs) = nFlgVal;
        break;
      case 1:
        *(&g_sceneState_52F7E0.bCurrentSubStage + nFlgOfs) += nFlgVal;
        break;
      case 2:
        *(&g_sceneState_52F7E0.bCurrentSubStage + nFlgOfs) -= nFlgVal;
        break;
      case 3:
        *(&g_sceneState_52F7E0.bCurrentSubStage + nFlgOfs) *= nFlgVal;
        break;
      case 4:
        *((char*)&g_sceneState_52F7E0.bCurrentSubStage + nFlgOfs) /= nFlgVal;
        break;
      default:
        break;
      }
      for (i = 0; i < 20 && pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 1] != ';'; ++i)
        ++pScene->dwPtrCurrent_448;
      pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
      continue;
    }
    if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "GOTO:", 5u) )
    {
      pScene->dwPtrCurrent_448 += 5;          // chunk == 'GOTO'
      GotoLabel107(pScene);
    }
    else if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "IF(FLG", 6u) )
    {
      ++bResult;                              // -- chunk == 'IF(FLG'
      pScene->dwPtrCurrent_448 += 6;
      nFlgIdx = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      for ( i = 0;
        i < 20
        && pScene->ptrStrData_260[pScene->dwPtrCurrent_448] >= '0'  // 48
        && pScene->ptrStrData_260[pScene->dwPtrCurrent_448] <= '9'; // 57
        ++i )
      {
        ++pScene->dwPtrCurrent_448;
      }

      CompareByte(pScene, ((char *)&g_sceneState_52F7E0.bCurrentSubStage)[nFlgIdx], &bFlgCond);

      for ( i = 0; i < 20 && pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 1] != ')'; ++i )
        ++pScene->dwPtrCurrent_448;
      if ( bFlgCond == 1 )
        GotoLabel107(pScene);
      else {
        for (i = 0; i < 20 && pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 1] != ';'; ++i)
          ++pScene->dwPtrCurrent_448;
      }
    }
    else if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "IF(OFLG", 7u) )
    {
      ++bResult;                              // -- chunk == 'IF(OFLG'
      pScene->dwPtrCurrent_448 += 7;
      nOFlgIdx = atoi(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      for ( i = 0;
        i < 20
        && pScene->ptrStrData_260[pScene->dwPtrCurrent_448]  >= '0'
        && pScene->ptrStrData_260[pScene->dwPtrCurrent_448]  <= '9';
        ++i )
      {
        ++pScene->dwPtrCurrent_448;
      }

      CompareByte(pScene, g_sysDataSave_530170[nOFlgIdx], &bOfCond);
      
      for ( i = 0; i < 20 && pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 1] != ')'; ++i )
        ++pScene->dwPtrCurrent_448;
      if ( bOfCond == 1 )
        GotoLabel107(pScene);
      else {
        for (i = 0; i < 20 && pScene->ptrStrData_260[pScene->dwPtrCurrent_448 - 1] != ';'; ++i)
          ++pScene->dwPtrCurrent_448;
      }
    }
    else if ( pScene->ptrStrData_260[pScene->dwPtrCurrent_448] == 'F' )
    {
      *pfExecTime = atof(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448]);
      pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
        &pScene->ptrStrData_260[++pScene->dwPtrCurrent_448],
        ';',
        0)
        + 1;
    }
    else
    {
      if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "SELECT=", 7u) )
      {
        pScene->dwPtrCurrent_448 += 7;        // chunk == 'SELECT='
        pScene->by_463 = 1;
        pScene->WCurNum = pScene->pAnimSet_148->field_130;
        pScene->flt_458 = -0.050000001;
        pScene->dwPtrPos_444 = pScene->dwPtrCurrent_448;
        Scene_CreateFontTextureFromStringData(pScene);
        pScene->dwPtrCurrent_448 += FindCharUntil_487D46(
          &pScene->ptrStrData_260[pScene->dwPtrCurrent_448 + 1],
          '*',
          0)
          + 1;
        return bResult;
      }
      if ( !memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "END;", 4u)
        || !pScene->ptrStrData_260[pScene->dwPtrCurrent_448] )
      {
        return 0;                             // chunk == 'END'
      }
      if ( memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "LPU;", 4u)
        && pScene->ptrStrData_260[pScene->dwPtrCurrent_448] )
      {
        if ( memcmp(&pScene->ptrStrData_260[pScene->dwPtrCurrent_448], "@", 2u)
          && pScene->ptrStrData_260[pScene->dwPtrCurrent_448] <= '\b'
          && (pScene->ptrStrData_260[pScene->dwPtrCurrent_448] < 'A'
            || pScene->ptrStrData_260[pScene->dwPtrCurrent_448] > 'z') )
        {
          return bResult;
        }
        ++pScene->dwPtrCurrent_448;
      }
      else
      {
        pScene->dwPtrCurrent_448 += 4;
        g_sceneState_52F7E0.protCurrentHP_C0 = g_sceneState_52F7E0.protMaximumHP_C4 * 20.0 / 100.0
          + g_sceneState_52F7E0.protCurrentHP_C0;
        if ( g_sceneState_52F7E0.protCurrentHP_C0 > (double)g_sceneState_52F7E0.protMaximumHP_C4 )
          g_sceneState_52F7E0.protCurrentHP_C0 = g_sceneState_52F7E0.protMaximumHP_C4;
      }
    }
  } // g 
}

void register_ParseExecStr_487D89(int cfg) {
  if (cfg == 1) {
    ParseExecStr_487D89 = hook_ParseExecStr_487D89;
  }

  if (cfg != 2) hnh_HookOff();
  hnh_Hook(0x487D89, hook_ParseExecStr_487D89, "ParseExecStr_487D89"); // [OK]

  hnh_HookOn();
}
