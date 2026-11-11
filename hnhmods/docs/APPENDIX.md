# Part IX: Appendix - Globals Registry & Reconstruction Status

Two reference tables: where the rebuild stands (what is done, what is knowingly not),
and the address registry of every game global cited across Parts I-VIII. All
addresses are the original `HaNaHiMe.exe` `.data`/`.rdata` locations (image base
0x400000); under `_USRDLL` the rebuild binds them as live references, in the
standalone build they become plain statics (Part I).

## Reconstruction Status

### Coverage

The call tree in [GOALS.md](../GOALS.md) (rooted at `WinMainCRTStartup`, 0x49F0E1)
tracks 460 functions; [CURRENT.md](../CURRENT.md) is regenerated from it by
`tools/gen_current.py` + `tools/scan_cpp.py`. As of its 2026-11-07 generation:

| metric | count |
|--------|-------|
| tree functions | 460 |
| hooked (detour registered) | 433 |
| pinned as function pointer | 426 |
| **fully covered (HOOK+PTR)** | **426** |
| implemented but unregistered | 1 |
| untouched `[todo]` | 26 |

The 26 `[todo]` entries fall into five classes:

- **CRT internals (8)** - `WinMainCRTStartup`, `heap_init_crt`, `setargv`,
  `parse_cmdline`, `malloc`, `nh_malloc`, `setenvp`, `crtGetEnvironmentStringsA`.
  Never needed: the standalone exe links the modern CRT.
- **Import thunks (4)** - `DirectSoundCreate`, `DirectInputCreateA`,
  `VideoForWindowsVersion`, `MCIWndCreateA` - bound to the real system DLLs through
  import libraries instead.
- **The 0x3FB object family (7)** - `CHanaObj__FreeT3FB/T3FC/T3FD/T3FE`,
  `ReleaseHana3FBTexture/Material`, `HanaT3FB_Draw`. An object family whose
  destructors were never reconstructed; this is why freeing types 0x3FB-0x3FE in
  `CHanaObj__Free_40C1E4` logs a WARN and does nothing (Part II).
- **Return stubs (6)** - `return1_18`..`return1_22`, `nop_4863C3`.
- **One duplicate helper** - `IsFileExists_43A8B0` (a second file-probe variant).

### Functionality known un-rebuilt inside covered files

- Texture-coordinate animation: `CHanaTextureCoordsT3F2` exists with a free path, but
  nothing ever writes `tus_134`/`tvs_138` - no parser, no evaluator.
- Light animation keys (`CHanaLightAnimT3F9_S144`) - only the lifecycle is rebuilt;
  animated lights ride ordinary frame ANIM tracks instead.
- Primitive-type switching: nothing writes `nPrimType_154[1]`, so every mesh renders
  as indexed TRIANGLELIST (Part IV).
- `sub_413F78` ("DrawSprite2DCenter") is referenced by comments but unimplemented.

### Dormant infrastructure

`dynstruct` (schema-driven memory inspection) has no users yet; the embedded Python
REPL is compiled but both of its Draw calls are commented out; `DrawHNH`'s Scene
Preview is a placeholder.

### Vestigial and dead

DirectInput is created and acquired but `GetDeviceState` is never called; the GRP800
directory (path code 1008) has no caller; `g_dwFontHeight` is write-only;
`fMatDirty` is set and cleared but never read; the 2D sound buffers carry
`CTRLPOSITIONNOTIFY`/`CTRLFREQUENCY` caps whose interfaces are never acquired; the
`_D_surface` thin-ceiling branch in `HitOverheadSurface_442C1C` is unreachable (the
name compare copies only 2 of 10 chars); `Tea.ex` bytes 0xFE-0xFF are never
initialized; the autosave path format string is always empty; `g_fFoeCdTimer` is
decremented but nothing ever sets it; `g_bFoeCharging` is read but never written by
the rebuild.

### Faithfully replicated original bugs

The light-index mask clear is an always-false expression (freed light slots are
never reclaimed); object-table slot 0 is a permanent `'%'` sentinel that never
unlinks; `FreeTexCoordT3F2` ignores the refcount; `CompareByte` never clears its
output flag and parses `>`/`<` operands at the operator (both Part V); the `.sek`
loader requests 4 bytes more than files contain; `FreeMorpT457` repeats null
assignments after frees; unnamed vertices get names printed from `rand()`; a second
`3DON` tag in one ODS stalls the parser; `SekHeader_CloneClean` forgets to zero one
of five pointers; `UpdateVoicePlayback` fades/stop-checks buffers unguarded.

### Intentional deviations (all flagged in source, originals under `#if 0`)

Victory-cutscene slow-mo `saved/1.5` and `saved/3.0` (original `/3.0`, `/5.0`); the
dim overlay alpha 0.7 (original 0.3); the victory frozen-frame blit inset by 20 px
(original full-frame). Mod-side additions with no original counterpart: the
`IsGameInputOn` input freeze, the icon/cursor resources, `ONCELOG` breadcrumbs.

## Globals Registry

Every entry verified in-source (name and address on the same declaration line; the
`.rdata` tables are rebuilt constants in [src/struct.cpp](../src/struct.cpp) whose
original addresses are pinned by static asserts).

### Engine & device

| address | global | role |
|---------|--------|------|
| 0x52F5C8 | `g_hInstance` | process instance |
| 0x4B3488 | `g_bRunGame` | launcher start flag (Part III) |
| 0x4D6C78 | `g_pD3DDevice_4D6C78` | D3D7 device |
| 0x4D6C7C | `g_pDD_4D6C7C` | DirectDraw7 |
| 0x4D6C84 | `g_pD3D_4D6C84` | Direct3D7 (VB creation) |
| 0x4D6C88 | `g_pPrimaryBuffer_4D6C88` | primary surface |
| 0x4D6C90 | `g_pBackBuffer_4D6C90` | back buffer |
| 0x4D6C94 | `g_pZBuffer_4D6C94` | z surface |
| 0x4D6C6C | `g_bD3Dinited_4D6C6C` | device-up latch |
| 0x4D6CCC | `g_pDevicesDesc` | D3DX device-descriptor array |
| 0x52F19C | `g_nWidth3` | render width (coord scaling, ex `g_nScreenScale`) |
| 0x4D6CC0 | `g_bAlphaBlendEnabled_4D6CC0` | blend-state mirror |
| 0x4D6CC4 | `g_nAlphaOpState_4D6CC4` | stage-0 alpha-op mirror |

### Graphics options (`Tea.ex` image, Part IV)

| address | global | meaning |
|---------|--------|---------|
| 0x4B3480 | `g_bFogIsActive_4B3480` | fog enable |
| 0x4B3474 | `g_bSpecularEnable_4B3474` | SPECULARENABLE |
| 0x4B347C | `g_bAntiAliasEnable` | antialias |
| 0x4B3478 | `g_bDitherEnable` | dither |
| 0x4B3484 | `g_bLightEnable_4B3484` | lighting |
| 0x518CA8 | `g_bShadowEnable_518CA8` | shadows |
| 0x518C80 | `g_nResolutionMode_518C80` | video mode index |
| 0x518C9C | `g_nZBufDepthMode_518C9C` | z-depth order |
| 0x518CA0 | `g_nRenderHALMode_518CA0` | device index (forced 1) |
| 0x4B00A8 | `g_nLightningState` | lighting state |
| 0x4B00AC | `g_nLightningEnabled` | lighting master gate |
| 0x4D6CF4 | `g_dwFogEnable_4D6CF4` | fog render-state mirror |
| 0x4B00C8 | `g_dwFogMode_4B00C8` | fog mode |
| 0x4B00A0 | `g_bSpecularEnable_4B00A0` | runtime material specular gate |
| 0x4B00A4 | `g_bSpecularEnableMirror_4B00A4` | its mirror |
| 0x4B00B0 | `g_dwShadowOff` | shadow-only skip flag |

### Frames, transforms, lights

| address | global | role |
|---------|--------|------|
| 0x4D6CA8 | `g_pSceneFrame` | world root |
| 0x4D6CAC | `g_pCameraFrame` | camera frame |
| 0x4D6CB0 | `g_pLightFrame` | light frame |
| 0x4D6CDC | `g_pMaterial` | default material |
| 0x4D6CE0 | `g_pLight_4D6CE0` | default ambient pseudo-light |
| 0x4D6CB8 | `g_bitsLightIndexes_4D6CB8` | 64-bit light-slot mask |
| 0x4D4780 | `g_mat4x4View_4D4780` | view matrix |
| 0x4CE870 | `g_mat4x4Projection_4CE870` | projection matrix |
| 0x4CE918 | `g_pM2` | viewport matrix (projection-to-screen) |
| 0x5177B4 | `g_SineTable` | 360-entry float sin |
| 0x517D68 | `g_CosineTable` | 360-entry float cos |
| 0x4CD788 | `g_arrFwdByYaw360_4CD788` | yaw direction LUT |
| 0x4D47C0 | `g_arrUpByPitch360_4D47C0` | pitch up LUT |
| 0x4D58A0 | `g_arrFwdByPitch360_4D58A0` | pitch forward LUT |

### Objects & memory

| address | global | role |
|---------|--------|------|
| 0x4D6C98 | `g_pObjectTable` | name-keyed registry (256 x 0x208) |
| 0x4D6C9C | `g_nObjectTableCount` | used slots |
| 0x4B009C | `g_nObjectTableCapacity_4B009C` | capacity (grows +256) |
| 0x4D6CD0 | `g_pObjPool` | draw-batch pool |
| 0x4CE868 | `g_nObjCount` | pool entries this frame |
| 0x4CD780 | `g_nCapObjects_4CD780` | pool capacity |
| 0x4CE958 | `g_nObjZIndexes_4CE958` | sort keys (float[6000]) |
| 0x4D6CF0 | `g_nMemCnt_4D6CF0` | MemAlloc balance (leak sentinel) |
| 0x518388 | `g_nBMPCounts_518388` | BMP load counter |
| 0x4D6D14..0x4D6D50 | `g_nRefVertT3EA`..`g_nRefMorpT457` | the 16 per-type AddRef/Free counters |
| 0x5187F0 | `g_hHeapHandle` | private audio heap |
| 0x5187F4 | `g_nHeapRefCount` | its refcount |

### Input (Part II/V)

| address | global | role |
|---------|--------|------|
| 0x4C7354 | `g_pInputState_4C7354` | published input snapshot |
| 0x52F1C0 | `g_arrInputState1_52F1C0` | history backing A (30 slots) |
| 0x52F2B0 | `g_arrInputState2_52F2B0` | history backing B |
| 0x52F3A4 / 0x52F3A8 | `g_pInputState1_52F3A4` / `g_pInputState2_52F3A8` | aliases |
| 0x52F3AC | `g_bRequestThreadStop` | cooperative stop flag |
| 0x52F3AD | `g_bThreadWait_52F3AD` | pause request |
| 0x52F3AE | `g_bThreadInWaiting_52F3AE` | pause ack |
| 0x4C7330..0x4C734C | `g_vkBtn*Alt_4C73xx`, `g_vkKbd*` | remappable VK bindings |
| 0x4C7350 | `g_keyC` | first-person toggle latch |
| 0x5196D4 | `g_ppDI` | DirectInput (vestigial) |
| 0x5196D8 | `g_pdiMouse_5196D8` | DI mouse |
| 0x5196E0 | `g_bdiAcquire_5196E0` | acquire state |

### Audio (Part V)

| address | global | role |
|---------|--------|------|
| 0x5185C0 | `g_ppDS` | DirectSound object |
| 0x5185C4 | `g_bDSInited_5185C4` | init latch |
| 0x5185C8 | `g_dwDSStatus_5185C8` | shared GetStatus scratch |
| 0x5185D0 | `g_pdsBuffer` | primary buffer |
| 0x52F3C4 | `g_pds3DListener` | 3D listener |
| 0x518560 | `g_dsCaps_518560` | DS caps |
| 0x52F168 | `g_arrUIEffectDSBuffer_52F168` | 5 UI effect buffers |
| 0x52F17C / 0x52F18C | `g_arrFootstepSound_52F17C` / `_52F18C` | two footstep 3D sets |
| 0x52EFD0 | `g_pDSBufAssault_52EFD0` | assault BGM |
| 0x5244AC | `g_pArrSndBuf` | 10-slot UI/attack buffers |
| 0x52EF64 / 0x52EF68 | `g_nCurVoiceIdx_52EF64` / `g_nVoiceRotate` | voice scheduler state |

### Combat (Part VII)

| address | global | role |
|---------|--------|------|
| 0x52ED04 | `g_dwBattleMode` | hero action id (-1 = locked) |
| 0x52ED00..0x52ED10 | `g_fHeroChargeTime`, `g_bHeroAttackHold`, `g_bHeroLightAttack`, `g_bHeroChargeActive`, `g_fHeroLightAttackTime`, `g_fHeroIdleTime` | duel hero latches |
| 0x52EF74..0x52EF7C | `g_bAttackHeld`, `g_bSkillActive`, `g_bSpinActive`, `g_fSkillTimer`, `g_fIdleTimer` | exploring hero latches |
| 0x52EF6C | `g_fSpinTimer` | spin timer |
| 0x52EF70 | `g_fHeroAnimSpeed` | camera-coupled anim rate |
| 0x52ED20 | `g_dwFoeAiState` | foe AI state |
| 0x52ED24 | `g_bFoeCharging` | charge flag (never written by rebuild) |
| 0x52ED28 | `g_fFoeAiTimer` | patrol re-roll timer |
| 0x52ED18 | `g_fFoeCdTimer` | cooldown (never set by rebuild) |
| 0x52EF5C / 0x52EF60 | `g_fFoeSpinDir` / `g_bFoeSpun` | spin memory |
| 0x519D48 | `g_pMapCollGrid` | 200x200 occupancy grid |
| 0x5244F8 / 0x523CAC / 0x52E500 | `g_arrFloodPathXY` / `Len` / `Cnt` | flood-fill path buffers |
| 0x4C6F48 | `g_fHitY_4C6F48` | collision height band (16.2) |

### Scene & save state (Part III/VI)

| address | global | role |
|---------|--------|------|
| 0x52F7E0 | `g_sceneState_52F7E0` | live `CSceneState` (400 bytes) |
| 0x530170 | `g_sysDataSave_530170` | `data0.dat` image (256 bytes) |
| 0x530260 | `g_dwUnlockedSaveSlots_530260` | diary slot unlock bits |
| 0x530264..0x530269 | `g_nHSceneBackground`, `g_nHSceneMale`, `g_nVolumeLevel`, `g_nTextSpeed`, `g_nFirstPersonCam`, `g_sceneProgressFlag` | settings tail |
| 0x52F1A0 / 0x52F1A4 | `g_timerStart` / `g_timerLast` | frame clock pair |
| 0x4C6F74 | `TIMER_SCALE_FACTOR` | global time scale (default 20.0) |
| 0x52F0D0 | `g_hWaitScreenThread` | WAIT-screen thread |

### Paths (Part III/VI)

| address | global | directory |
|---------|--------|-----------|
| 0x530270 | `g_szHnhPath_530270` | install root |
| 0x52FC70 | `g_szDriveLetr_52FC70` | CD root |
| 0x52F6E0 | `g_szODSPath` | ODS (1002) |
| 0x52FB70 | `g_szSPRPath` | SPR (1003) |
| 0x52F5E0 | `g_szStrPath` | Str (1004) |
| 0x52FF70 | `g_szODFPath` | ODF (1005) |
| 0x530070 | `g_szWAVPath` | WAV (1006) |
| 0x52F970 | `g_szGRPPath` | GRP (1007) |
| 0x52FA70 | `g_szGRP800Path` | GRP800 (1008, unused) |
| 0x52FD70 | `g_szSEPath` | SE (1009) |
| 0x52FE70 | `g_szDATAPath` | DATA (1010) |

### Text & font (Part VIII)

| address | global | role |
|---------|--------|------|
| 0x518378 | `g_hFont` | GDI font (MS Mincho) |
| 0x4B01B4 | `g_dwCharWidth` | stored char cell (26) |
| 0x4B01B8 | `g_dwFontHeight` | stored, never read |
| 0x518CCC | `g_hFontTitleMenu` | launcher font (14x7) |
| 0x52F1B0 | `g_dwFontRenderFlag` | glyph reveal frontier |
| 0x52F1B4 | `g_fFrameTime_52F1B4` | tape scroll accumulator |
| 0x519CF8 | `g_lpDDSurface_519CF8` | rasterizer scratch |
| 0x519D00 | `g_tempText_519D00` | debug text copy (never read) |
| 0x52F1A8 | `g_pSurfaceDarkenBmp_52F1A8` | fade darken tile |
| 0x52F1AC | `g_pszSaveNameFmt_52F1AC` | vestigial save format string |

### Compiled-in .rdata tables (original addresses, rebuilt in struct.cpp)

| address | table | content |
|---------|-------|---------|
| 0x4C6F4C | `g_fVolumeDbTable[10]` | DS dB ladder |
| 0x4C6F78 | `g_arrSceneNo_4C6F78[100]` | chapter walk order |
| 0x4B5970 | `g_arrSceneStepThresholds_4B5970[40]` | diary block picker |
| 0x4B5A10 | `g_szSceneInfoText_4B5A10[40][20][80]` | save-slot diary text (SJIS) |
| 0x4C9FB0 | `g_aG00Gp_4C9FB0[100]` | event-picture names |
| 0x4C78A0 | `g_ascSndFileNames_4C78A0[200]` | sound name table |
| 0x4A8368 | `g_texCoordSizeLUT_4A8368[4]` | FVF texcoord size LUT |
