#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// == GLOBALS ===

void (__cdecl*SetupHeroEntry_4593E0)(CHanaSceneT458_S504 *pScene,
  CHanaFrameT3EC_S354 *pHeroFrame, int nSubIndex) =
  reinterpret_cast<void(__cdecl*)(CHanaSceneT458_S504 *, CHanaFrameT3EC_S354 *, int)>(0x4593E0);

// === FUNCTIONS ===

// Place the hero at its entry position for this sub-stage. Scans the ODS scene's children for nodes named like aJump (spawn/entry markers), and when the match index equals nSubIndex, loads that node's position/rotation into the hero frame.
void __cdecl hook_SetupHeroEntry_4593E0(CHanaSceneT458_S504 *pScene, CHanaFrameT3EC_S354 *pHeroFrame, int nSubIndex)
{
  int idx; // edx
  float outZ; // [esp+0h] [ebp-2Ch] BYREF
  float outY; // [esp+4h] [ebp-28h] BYREF
  int j; // [esp+8h] [ebp-24h]
  float outX; // [esp+Ch] [ebp-20h] BYREF
  unsigned int i; // [esp+10h] [ebp-1Ch]
  float pZXrot; // [esp+14h] [ebp-18h] BYREF
  float dirForwardZ; // [esp+18h] [ebp-14h] BYREF
  float pYXrot; // [esp+1Ch] [ebp-10h] BYREF
  float pYpos; // [esp+20h] [ebp-Ch] BYREF
  float pXYrot; // [esp+24h] [ebp-8h] BYREF
  float dirForwardX; // [esp+28h] [ebp-4h] BYREF
  ONCELOG("[HNH:SUB] SetupHeroEntry_4593E0\n");
  
  j = 0;
  if ( pScene )
  {
    for ( i = 0; i < pScene->dwCount_130; ++i )
    {
      if ( pScene->refsFrames_134[i] )
      {
        if ( !_strnicmp(pScene->refsFrames_134[i]->szName_8, "JUMP", 4u) )
        {
          idx = j++;
          if ( nSubIndex == idx )
          {
            HanaFrame_GetRelPosition(pScene->refsFrames_134[i], g_pSceneFrame, &outX, &outY, &outZ);
            MtxMult_4069DD(pHeroFrame, (CHanaFrameT3EC_S354 *)g_pSceneFrame, outX, outY, outZ);
            CalculateLocalRotationMatrix(
              pScene->refsFrames_134[i],
              (CHanaFrameT3EC_S354 *)g_pSceneFrame,
              &dirForwardX,
              &pYpos,
              &dirForwardZ,
              &pXYrot,
              &pYXrot,
              &pZXrot);
            UpdateBoneLookAtTransform(
              pHeroFrame,
              (CHanaFrameT3EC_S354 *)g_pSceneFrame,
              dirForwardX,
              0.0,
              dirForwardZ,
              0.0,
              1.0,
              0.0);
            MtxMult_4069DD(pHeroFrame, pHeroFrame, 0.0, 0.0, -5.0);
            return;
          }
        }
      }
    }
  }
}

void register_SetupHeroEntry_4593E0() {
  hnh_Hook(0x4593e0, hook_SetupHeroEntry_4593E0, "SetupHeroEntry_4593E0");
}
