// GAME: [011019][TEATIME] 華の悲鳴　～壊れた硝子の心～
#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"
#include "hnh_hook.hpp"

// === PINNED GAME FUNCTIONS ===

// (the original-function pin PlayAssault_4703AE is defined in
//  MenuCharSelect.cpp - its first caller - and extern'd in shared.hpp)

// --- not yet extracted; pinned here call-only ---
// (ToggleWaitScreen's only caller is this scene)

void (__cdecl*ToggleWaitScreen)(int bShow) =
    reinterpret_cast<void(__cdecl*)(int)>(0x47E3E1);

CHanaFrameT3EC_S354 *(__cdecl*Scene_FindMapFrame)(
    CHanaSceneT458_S504 *pScene) =
    reinterpret_cast<CHanaFrameT3EC_S354 *(*)(CHanaSceneT458_S504 *)>(
      0x46777D);

void (__cdecl*InitializePracticeAnimations_4667A9)(
    CHanaFrameT3EC_S354 *pModeFrame) =
    reinterpret_cast<void(__cdecl*)(CHanaFrameT3EC_S354 *)>(0x4667A9);

void (WINAPI *WaitScreenThreadProc)(LPVOID lpThreadParameter)
= reinterpret_cast<void (WINAPI*)(LPVOID lpThreadParameter)>(0x47e1a2);
void (__cdecl *Frame_SetPracticeAnimRootRecursive)(CHanaFrameT3EC_S354 *pFrame, CHanaFrameT3EC_S354 *pRootAnimFrame)
= reinterpret_cast<void (__cdecl*)(CHanaFrameT3EC_S354 *pFrame, CHanaFrameT3EC_S354 *pRootAnimFrame)>(0x466738);
int  (__cdecl*PlayAssault_4703AE)(int iModeId, int iMenuTab) =
reinterpret_cast<int(__cdecl*)(int, int)>(0x4703AE);

// === PINNED GLOBALS ===
// (none new - g_pDSBufAssault_52EFD0 is pinned in RunModeSelectScreen.cpp; every
//  other global used here is already extern in shared.hpp)

static LPDIRECTSOUNDBUFFER s_pDSBufAssault_52EFD0;

#ifdef _USRDLL
LPDIRECTSOUNDBUFFER &g_pDSBufAssault_52EFD0 = *(LPDIRECTSOUNDBUFFER *)0x52EFD0;  // WRITABLE

HANDLE &g_hWaitScreenThread = *(HANDLE *)0x52F0D0; // WRITABLE
BOOL &g_bWaitScreenOn = *(BOOL *)0x52F0CC; // WRITABLE
HWND &g_hwndWaitScreen = *(HWND *)0x52F0C8; // WRITABLE
#else
LPDIRECTSOUNDBUFFER& g_pDSBufAssault_52EFD0 = s_pDSBufAssault_52EFD0;

HANDLE g_hWaitScreenThread;
BOOL   g_bWaitScreenOn;
HWND   g_hwndWaitScreen;

#endif


// === FUNCTIONS ===

// PlayAssault_4703AE(iModeId, iMenuTab) -- GameMainLoop case-4 scene, the
// kendo-"practice" session (also entered from MenuCharSelect with iModeId =
// ch*2 / ch*2+1, iMenuTab = 1). One session = selection screen -> chosen
// .Ods sub-scene playback, repeated until the player backs out of the
// selection screen or the system menu requests exit.
//
// iModeId: parity picks the variant. iModeId/2 = group 0..4 (h02/h05/h04/
// h03/h01 tables). even: 20-scene grid select (RunSceneSelectScreen, remap
// arrSceneRemap[20] = {0,5,6,4,2, 5,6,4,1, 5,6,4,3, 5,6,4,0, 12,13,14});
// odd: 5-button mode select (RunModeSelectScreen, arrModeRemap[5]={7..11}).
// A finished selection persists bit 1<<(remap[sel]+10) into the 64-bit
// g_sceneState_52F7E0.girlsStateBits_98[group] unlock mask (qwSelMask_C8 gates
// the buttons; defaulted to 2 on entry, cleared to 0 on exit).
// iMenuTab: system-menu tab while in-scene (0 -> tab 0, nonzero -> tab 2).
//
// Setup (behind the blinking WAIT thread, ToggleWaitScreen): <=30 h\ah??_NN
// voice wavs of the group's 30-slot block (arrSoundAHs; slot 21 is ch??_00,
// slots 22..29 empty), 9 a\hSE0N[abc].wav (arrSoundSEs), 10 cursor/face
// BMPs mouse.bmp + h\Eye1-3.bmp + h\En.bmp (arrBMPMouses - all 10 load
// attempts run, empty names just fail), 6 h\GoNa.bmp/GoNb.bmp stance pairs
// (arrBMPGo), the modo sprite pages (even: arrModoNames A/B columns +
// group's vai/Bel/kyu/kan/nas/eda overlay pages appended after them; odd:
// arrModoNamesOdd rows with the B name at .str[4] of row+20; even groups
// 1..4 swap slot 2 to suwaA/suwaB and slot 10 to suwa0a), 6 body-part mask
// pairs rnune/lnune/rsiri/lsiri/momo/asoko A/B (arrBMPNunes); the lock-on
// body lockon\lock.sek+Target.odf (pLockOnBody, UpdateTo2(0), frame hidden),
// the mode body h\hXX\hhXX_0N.sek/.odf (pModeBody, attached to g_pSceneFrame,
// InitializePracticeAnimations + UpdateTo2(3), hidden), <=3 Tears bodies
// h\Tears\h[.sek] + h\Tears\hXX[_sl/_sr].odf (hidden; group h05's hands are
// h04_sl/h04_sr, sic); BGM s012.wav (even) / s013.wav (odd) into
// g_pDSBufAssault_52EFD0 at AVI-volume - 1000 dB, looped. Returns -1 if the
// lock-on/mode/Tears bodies or the selected .Ods fail to load.
//
// Session loop: pick via the group's select screen (skipped on the first
// pass, so sub-scene 0 auto-plays once as the intro/backdrop); selection
// result >= 1 or piSelected < 0 exits. Load "<h\hXX\hXX?>.Ods" - LoadOdsEx
// reusing another live scene's frame/anim-set tables when one exists, else
// a fresh LoadOds - then free every other scene, keep only the new one's
// frames visible (all frames of all scenes are hidden first), reset and
// pre-roll the script (exec time 1.0 then 5.0), and run the frame loop:
// DoPeekMessages + GetFrameTime (capped 5.0); menu key pauses the scene
// audio and opens SystemMenu on the BGM (tab 0/2 by iMenuTab) over a copy
// of the scene state, then re-applies the effect option and re-finds every
// map frame (>= 1 exits); mouse btn 2 (right) toggles bSkipMode (hides the
// dialogue window, cursor and UI sprites and skips the script parse);
// left-click with x+16 > 576 and y < 32 while piSelected < 14 and no still
// image opens PlayCutInScene_46A76F (returns 1 to leave the scene);
// btn 3 held without skip runs the frame at x5 speed. ParseExecStr drives
// the scene; when it runs dry the scene pointers are rewound and the loop
// breaks back to selection; UpdateSceneAtTime returning 0 ends the scene.
// Draw: still-image scenes Get/Set ANTIALIAS around a straight back-buffer
// Blt of pSurface_468, real scenes Clear(3) + 3D pass (alpha off for
// HanaFrame_Draw, on for HanaVert_DrawBatch); then DrawDialogueWindow (text
// speed x1000 on a click), and a second BeginScene pass with SRCBLEND 5 /
// DESTBLEND 6 drawing the Go2b prompt (fades in/out around x = 576, at
// 608,16) and the cursor sprite (face sprites only track the mouse).
// Present, then fade each arrSoundAHs voice 300 dB/s toward -10000 (a
// voice already <= -1000 is stopped, sic unguarded). (Two dead stores -
// xMouse into the modo table's dead row 20 and yMouseStash - are kept.)
//
// Exit: RestartInputThread(0); on a non-menu end PlayScreenFade(0,0,0,
// 10,0); release the Tears bodies (re-attached to g_pSceneFrame first),
// lock-on and mode bodies; stop/free the BGM; free every Go/modo/nune/
// mouse sprite, scene and sound struct; qwSelMask_C8 = 0. Returns -1 on a
// load failure, 0 on normal completion, or the SystemMenu/select value.
//
// Notes vs the raw decompilation:
// - The 52 single-byte table sources byte_52EFF8..byte_52F0C4 (.data, all
//   0xFF in the static image, no writer in the binary) are the same
//   "runtime-empty marker" pool PlayHunting's byte_52EExx rows use; they
//   are normalized to "" here (the Tears loop returns -1 on a failed load,
//   so at runtime they must terminate the loops exactly like empty rows).
// - The persist writes flag bits 10..24, so the dword-pair writes collapse
//   into one QWORD |= on girlsStateBits_98[group] (high dword always ORs 0).
// - The select screens sat in do/while wrappers whose back-edges are
//   unreachable (piSelected < 0 only as -1, which breaks); single calls.
// - Two dead .data-copied scratch buffers (40 and 100 bytes, only memset)
//   were dropped.

int __cdecl hook_PlayAssault_4703AE(int iModeId, int iMenuTab)
{
    CSceneState sceneMenuCopy;          // SystemMenu by-value arg copy
    CHanaSceneT458_S504 *arrScenes[30]; // loaded sub-scenes (slot = selection)
    CHanaSceneT458_S504 *pSceneAny;     // any live scene (LoadOdsEx source)
    CHanaSceneT458_S504 *pSceneNew;     // freshly loaded sub-scene
    CSekBody *arrTearsBodies[5];        // h\Tears body + _sl/_sr
    CSekBody *pLockOnBody;              // lockon\lock.sek + Target.odf
    CSekBody *pModeBody;                // h\hXX\hhXX_0N.sek/.odf
    CSoundStruct_8 *arrSoundAHs[30];    // h\ah??_NN.wav voice wavs
    CSoundStruct_8 *arrSoundSEs[30];    // a\hSE0N[abc].wav
    CHanaSpriteT3F6_S1FC *arrBMPGo[30]; // h\GoNa.bmp/GoNb.bmp stance pairs
    CHanaSpriteT3F6_S1FC *arrBMPMouses[10]; // cursor + face overlays
    CHanaSpriteT3F6_S1FC *arrBMPModos[30];  // modo / vai sprite pages
    CHanaSpriteT3F6_S1FC *arrBMPNunes[30];  // body-part mask pairs
    CHAR pszFileName[256];
    CHAR String[256];
    DWORD dwAntialiasSave;
    int nVoiceFadeVol;
    float pfExecTime;
    float fDeltaTime;
    float fTextSpeed;
    int xMouse, yMouse;
    int yMouseStash;
    WORD btnMouse = 0, btnMousePrev;
    int nResult;                        // select-screen / menu / cut-in result
    int piSelected;
    int i, j;
    BOOL bSkipMode;                     // overlay hidden + script skipped
    int bFirstPassDone;

    ONCELOG("[HNH:SUB] PlayAssault_4703AE\n");

    // --- state init
    nResult = 0;
    bFirstPassDone = 0;
    bSkipMode = 0;
    pfExecTime = 5.0f;
    piSelected = 0;
    pLockOnBody = 0;
    pModeBody = 0;

    // --- BGM names (even -> s012, odd -> s013)
#if 1
    static const char arrBgmWavs[2][20] = {
      "s012.wav", "s013.wav"
    };
#else
    String20 arrBgmWavs[2];             // s012 / s013.wav

    strcpy(arrBgmWavs[0].str, "s012.wav");
    strcpy(arrBgmWavs[1].str, "s013.wav");
#endif

#if 1
    static const char arrSceneNames[20][30][20] = {
      {
        "h\\h02\\h02a", "h\\h02\\h02e",
        "h\\h02\\h02f", "h\\h02\\h02g",
        "h\\h02\\h02b", "h\\h02\\h02h",
        "h\\h02\\h02i", "h\\h02\\h02j",
        "h\\h02\\h02c", "h\\h02\\h02k",
        "h\\h02\\h02l", "h\\h02\\h02m",
        "h\\h02\\h02d", "h\\h02\\h02n",
      },
      {
        "h\\h02\\h02o", "h\\h02\\h02p",
        "h\\h02\\h02q", "h\\h02\\h02r",
        "h\\h02\\h02s",
      },
      {
        "h\\h05\\h05a", "h\\h05\\h05e",
        "h\\h05\\h05f", "h\\h05\\h05g",
        "h\\h05\\h05b", "h\\h05\\h05h",
        "h\\h05\\h05i", "h\\h05\\h05j",
        "h\\h05\\h05c", "h\\h05\\h05k",
        "h\\h05\\h05l", "h\\h05\\h05m",
        "h\\h05\\h05d", "h\\h05\\h05n",
        "", "",
        "", "h\\h05\\h05w",
        "h\\h05\\h05x"
      },
      {
        "h\\h05\\h05o", "h\\h05\\h05p",
        "h\\h05\\h05q", "h\\h05\\h05r",
        "h\\h05\\h05s",
      },
      {
        "h\\h04\\h04a", "h\\h04\\h04e",
        "h\\h04\\h04f", "h\\h04\\h04g",
        "h\\h04\\h04b", "h\\h04\\h04h",
        "h\\h04\\h04i", "h\\h04\\h04j",
        "h\\h04\\h04c", "h\\h04\\h04k",
        "h\\h04\\h04l", "h\\h04\\h04m",
        "h\\h04\\h04d", "h\\h04\\h04n",
        "", "",
        "", "h\\h04\\h04v",
        "h\\h04\\h04w",
      },
      {
        "h\\h04\\h04o", "h\\h04\\h04p",
        "h\\h04\\h04q", "h\\h04\\h04r",
        "h\\h04\\h04s",
      },
      {
        "h\\h03\\h03a", "h\\h03\\h03e",
        "h\\h03\\h03f", "h\\h03\\h03g",
        "h\\h03\\h03b", "h\\h03\\h03h",
        "h\\h03\\h03i", "h\\h03\\h03j",
        "h\\h03\\h03c", "h\\h03\\h03k",
        "h\\h03\\h03l", "h\\h03\\h03m",
        "h\\h03\\h03d", "h\\h03\\h03n",
        "", "",
        "", "h\\h03\\h03t",
        "h\\h03\\h03v", "h\\h03\\h03u",
      },
      {
        "h\\h03\\h03o", "h\\h03\\h03p",
        "h\\h03\\h03q", "h\\h03\\h03r",
        "h\\h03\\h03s",
      },
      {
        "h\\h01\\h01a", "h\\h01\\h01e",
        "h\\h01\\h01f", "h\\h01\\h01g",
        "h\\h01\\h01b", "h\\h01\\h01h",
        "h\\h01\\h01i", "h\\h01\\h01j",
        "h\\h01\\h01c", "h\\h01\\h01k",
        "h\\h01\\h01l", "h\\h01\\h01m",
        "h\\h01\\h01d", "h\\h01\\h01n",
        "", "",
        "", "h\\h01\\h01t",
        "h\\h01\\h01v", "h\\h01\\h01u",
      },
      {
        "h\\h01\\h01o", "h\\h01\\h01p",
        "h\\h01\\h01q", "h\\h01\\h01r",
        "h\\h01\\h01s",
      },
    };
#else
    String20 arrSceneNames[20][30];     // h\hXX\hXX? .Ods sub-scene names

    // --- .Ods sub-scene names per (mode row, selection). Rows 0..9 are the
    // even/odd variant pairs of the 5 groups (h02, h05, h04, h03, h01); the
    // even rows list 14 base scenes a..n plus tail poses, the odd rows the 5
    // B scenes o..s. Rows 10..19 and columns past the named ones are dead.
    memset(arrSceneNames, 0, sizeof(arrSceneNames));
    strcpy(arrSceneNames[0][0].str,  "h\\h02\\h02a");   strcpy(arrSceneNames[0][1].str,  "h\\h02\\h02e");
    strcpy(arrSceneNames[0][2].str,  "h\\h02\\h02f");   strcpy(arrSceneNames[0][3].str,  "h\\h02\\h02g");
    strcpy(arrSceneNames[0][4].str,  "h\\h02\\h02b");   strcpy(arrSceneNames[0][5].str,  "h\\h02\\h02h");
    strcpy(arrSceneNames[0][6].str,  "h\\h02\\h02i");   strcpy(arrSceneNames[0][7].str,  "h\\h02\\h02j");
    strcpy(arrSceneNames[0][8].str,  "h\\h02\\h02c");   strcpy(arrSceneNames[0][9].str,  "h\\h02\\h02k");
    strcpy(arrSceneNames[0][10].str, "h\\h02\\h02l");   strcpy(arrSceneNames[0][11].str, "h\\h02\\h02m");
    strcpy(arrSceneNames[0][12].str, "h\\h02\\h02d");   strcpy(arrSceneNames[0][13].str, "h\\h02\\h02n");
    // [0][14] runtime-empty
    strcpy(arrSceneNames[1][0].str,  "h\\h02\\h02o");   strcpy(arrSceneNames[1][1].str,  "h\\h02\\h02p");
    strcpy(arrSceneNames[1][2].str,  "h\\h02\\h02q");   strcpy(arrSceneNames[1][3].str,  "h\\h02\\h02r");
    strcpy(arrSceneNames[1][4].str,  "h\\h02\\h02s");
    // [1][5] runtime-empty
    strcpy(arrSceneNames[2][0].str,  "h\\h05\\h05a");   strcpy(arrSceneNames[2][1].str,  "h\\h05\\h05e");
    strcpy(arrSceneNames[2][2].str,  "h\\h05\\h05f");   strcpy(arrSceneNames[2][3].str,  "h\\h05\\h05g");
    strcpy(arrSceneNames[2][4].str,  "h\\h05\\h05b");   strcpy(arrSceneNames[2][5].str,  "h\\h05\\h05h");
    strcpy(arrSceneNames[2][6].str,  "h\\h05\\h05i");   strcpy(arrSceneNames[2][7].str,  "h\\h05\\h05j");
    strcpy(arrSceneNames[2][8].str,  "h\\h05\\h05c");   strcpy(arrSceneNames[2][9].str,  "h\\h05\\h05k");
    strcpy(arrSceneNames[2][10].str, "h\\h05\\h05l");   strcpy(arrSceneNames[2][11].str, "h\\h05\\h05m");
    strcpy(arrSceneNames[2][12].str, "h\\h05\\h05d");   strcpy(arrSceneNames[2][13].str, "h\\h05\\h05n");
    // [2][14..16] runtime-empty
    strcpy(arrSceneNames[2][17].str, "h\\h05\\h05w");   strcpy(arrSceneNames[2][18].str, "h\\h05\\h05x");
    // [2][19] runtime-empty
    strcpy(arrSceneNames[3][0].str,  "h\\h05\\h05o");   strcpy(arrSceneNames[3][1].str,  "h\\h05\\h05p");
    strcpy(arrSceneNames[3][2].str,  "h\\h05\\h05q");   strcpy(arrSceneNames[3][3].str,  "h\\h05\\h05r");
    strcpy(arrSceneNames[3][4].str,  "h\\h05\\h05s");
    // [3][5] runtime-empty
    strcpy(arrSceneNames[4][0].str,  "h\\h04\\h04a");   strcpy(arrSceneNames[4][1].str,  "h\\h04\\h04e");
    strcpy(arrSceneNames[4][2].str,  "h\\h04\\h04f");   strcpy(arrSceneNames[4][3].str,  "h\\h04\\h04g");
    strcpy(arrSceneNames[4][4].str,  "h\\h04\\h04b");   strcpy(arrSceneNames[4][5].str,  "h\\h04\\h04h");
    strcpy(arrSceneNames[4][6].str,  "h\\h04\\h04i");   strcpy(arrSceneNames[4][7].str,  "h\\h04\\h04j");
    strcpy(arrSceneNames[4][8].str,  "h\\h04\\h04c");   strcpy(arrSceneNames[4][9].str,  "h\\h04\\h04k");
    strcpy(arrSceneNames[4][10].str, "h\\h04\\h04l");   strcpy(arrSceneNames[4][11].str, "h\\h04\\h04m");
    strcpy(arrSceneNames[4][12].str, "h\\h04\\h04d");   strcpy(arrSceneNames[4][13].str, "h\\h04\\h04n");
    // [4][14..16] runtime-empty
    strcpy(arrSceneNames[4][17].str, "h\\h04\\h04v");   strcpy(arrSceneNames[4][18].str, "h\\h04\\h04w");
    // [4][19] runtime-empty
    strcpy(arrSceneNames[5][0].str,  "h\\h04\\h04o");   strcpy(arrSceneNames[5][1].str,  "h\\h04\\h04p");
    strcpy(arrSceneNames[5][2].str,  "h\\h04\\h04q");   strcpy(arrSceneNames[5][3].str,  "h\\h04\\h04r");
    strcpy(arrSceneNames[5][4].str,  "h\\h04\\h04s");
    // [5][5] runtime-empty
    strcpy(arrSceneNames[6][0].str,  "h\\h03\\h03a");   strcpy(arrSceneNames[6][1].str,  "h\\h03\\h03e");
    strcpy(arrSceneNames[6][2].str,  "h\\h03\\h03f");   strcpy(arrSceneNames[6][3].str,  "h\\h03\\h03g");
    strcpy(arrSceneNames[6][4].str,  "h\\h03\\h03b");   strcpy(arrSceneNames[6][5].str,  "h\\h03\\h03h");
    strcpy(arrSceneNames[6][6].str,  "h\\h03\\h03i");   strcpy(arrSceneNames[6][7].str,  "h\\h03\\h03j");
    strcpy(arrSceneNames[6][8].str,  "h\\h03\\h03c");   strcpy(arrSceneNames[6][9].str,  "h\\h03\\h03k");
    strcpy(arrSceneNames[6][10].str, "h\\h03\\h03l");   strcpy(arrSceneNames[6][11].str, "h\\h03\\h03m");
    strcpy(arrSceneNames[6][12].str, "h\\h03\\h03d");   strcpy(arrSceneNames[6][13].str, "h\\h03\\h03n");
    // [6][14..16] runtime-empty
    strcpy(arrSceneNames[6][17].str, "h\\h03\\h03t");   strcpy(arrSceneNames[6][18].str, "h\\h03\\h03v");
    strcpy(arrSceneNames[6][19].str, "h\\h03\\h03u");
    strcpy(arrSceneNames[7][0].str,  "h\\h03\\h03o");   strcpy(arrSceneNames[7][1].str,  "h\\h03\\h03p");
    strcpy(arrSceneNames[7][2].str,  "h\\h03\\h03q");   strcpy(arrSceneNames[7][3].str,  "h\\h03\\h03r");
    strcpy(arrSceneNames[7][4].str,  "h\\h03\\h03s");
    // [7][5] runtime-empty
    strcpy(arrSceneNames[8][0].str,  "h\\h01\\h01a");   strcpy(arrSceneNames[8][1].str,  "h\\h01\\h01e");
    strcpy(arrSceneNames[8][2].str,  "h\\h01\\h01f");   strcpy(arrSceneNames[8][3].str,  "h\\h01\\h01g");
    strcpy(arrSceneNames[8][4].str,  "h\\h01\\h01b");   strcpy(arrSceneNames[8][5].str,  "h\\h01\\h01h");
    strcpy(arrSceneNames[8][6].str,  "h\\h01\\h01i");   strcpy(arrSceneNames[8][7].str,  "h\\h01\\h01j");
    strcpy(arrSceneNames[8][8].str,  "h\\h01\\h01c");   strcpy(arrSceneNames[8][9].str,  "h\\h01\\h01k");
    strcpy(arrSceneNames[8][10].str, "h\\h01\\h01l");   strcpy(arrSceneNames[8][11].str, "h\\h01\\h01m");
    strcpy(arrSceneNames[8][12].str, "h\\h01\\h01d");   strcpy(arrSceneNames[8][13].str, "h\\h01\\h01n");
    // [8][14..16] runtime-empty
    strcpy(arrSceneNames[8][17].str, "h\\h01\\h01t");   strcpy(arrSceneNames[8][18].str, "h\\h01\\h01v");
    strcpy(arrSceneNames[8][19].str, "h\\h01\\h01u");
    strcpy(arrSceneNames[9][0].str,  "h\\h01\\h01o");   strcpy(arrSceneNames[9][1].str,  "h\\h01\\h01p");
    strcpy(arrSceneNames[9][2].str,  "h\\h01\\h01q");   strcpy(arrSceneNames[9][3].str,  "h\\h01\\h01r");
    strcpy(arrSceneNames[9][4].str,  "h\\h01\\h01s");
    // [9][5] runtime-empty; [10][0] and rows 11..19 dead
#endif

    String20 arrModeBodyNames[10];      // h\hXX\hhXX_0N mode bodies
    // --- mode body per iModeId (group A/B variant)
    strcpy(arrModeBodyNames[0].str, "h\\h02\\hh02_01");
    strcpy(arrModeBodyNames[1].str, "h\\h02\\hh02_02");
    strcpy(arrModeBodyNames[2].str, "h\\h05\\hh05_01");
    strcpy(arrModeBodyNames[3].str, "h\\h05\\hh05_02");
    strcpy(arrModeBodyNames[4].str, "h\\h04\\hh04_01");
    strcpy(arrModeBodyNames[5].str, "h\\h04\\hh04_02");
    strcpy(arrModeBodyNames[6].str, "h\\h03\\hh03_01");
    strcpy(arrModeBodyNames[7].str, "h\\h03\\hh03_02");
    strcpy(arrModeBodyNames[8].str, "h\\h01\\hh01_01");
    strcpy(arrModeBodyNames[9].str, "h\\h01\\hh01_02");

    String20 arrTearsOdfs[5][10];       // h\Tears\hXX[_sl/_sr]


    // --- Tears odf names per group (h05's hands use h04's, sic)
    memset(arrTearsOdfs, 0, sizeof(arrTearsOdfs));
    strcpy(arrTearsOdfs[0][0].str, "h\\Tears\\h02");
    strcpy(arrTearsOdfs[0][1].str, "h\\Tears\\h02_sl");
    strcpy(arrTearsOdfs[0][2].str, "h\\Tears\\h02_sr");
    strcpy(arrTearsOdfs[1][0].str, "h\\Tears\\h05");
    strcpy(arrTearsOdfs[1][1].str, "h\\Tears\\h04_sl"); // (sic)
    strcpy(arrTearsOdfs[1][2].str, "h\\Tears\\h04_sr"); // (sic)
    strcpy(arrTearsOdfs[2][0].str, "h\\Tears\\h04");
    strcpy(arrTearsOdfs[2][1].str, "h\\Tears\\h04_sl");
    strcpy(arrTearsOdfs[2][2].str, "h\\Tears\\h04_sr");
    strcpy(arrTearsOdfs[3][0].str, "h\\Tears\\h03");
    strcpy(arrTearsOdfs[3][1].str, "h\\Tears\\h03_sl");
    strcpy(arrTearsOdfs[3][2].str, "h\\Tears\\h03_sr");
    strcpy(arrTearsOdfs[4][0].str, "h\\Tears\\h01");
    strcpy(arrTearsOdfs[4][1].str, "h\\Tears\\h01_sl");
    strcpy(arrTearsOdfs[4][2].str, "h\\Tears\\h01_sr");
    // [g][3] runtime-empty

    String20 arrTearsSeks[10];          // h\Tears\h[_sl/_sr]


    // --- Tears sek names (row index shared across groups)
    memset(arrTearsSeks, 0, sizeof(arrTearsSeks));
    strcpy(arrTearsSeks[0].str, "h\\Tears\\h");
    strcpy(arrTearsSeks[1].str, "h\\Tears\\h_sl");
    strcpy(arrTearsSeks[2].str, "h\\Tears\\h_sr");
    // [3] runtime-empty

    String20 arrGoBmpNames[5][2];


    // --- Go stance sprite pairs
    memset(arrGoBmpNames, 0, sizeof(arrGoBmpNames));
    strcpy(arrGoBmpNames[0][0].str, "h\\Go1a.bmp");
    strcpy(arrGoBmpNames[0][1].str, "h\\Go1b.bmp");
    strcpy(arrGoBmpNames[1][0].str, "h\\Go2a.bmp");
    strcpy(arrGoBmpNames[1][1].str, "h\\Go2b.bmp");
    strcpy(arrGoBmpNames[2][0].str, "h\\Go3a.bmp");
    strcpy(arrGoBmpNames[2][1].str, "h\\Go3b.bmp");
    // [3][0] runtime-empty

    String20 arrMouseBmpNames[10];


    // --- cursor / face overlay BMPs
    memset(arrMouseBmpNames, 0, sizeof(arrMouseBmpNames));
    strcpy(arrMouseBmpNames[0].str, "mouse.bmp");
    strcpy(arrMouseBmpNames[1].str, "h\\Eye1.bmp");
    strcpy(arrMouseBmpNames[2].str, "h\\Eye2.bmp");
    strcpy(arrMouseBmpNames[3].str, "h\\Eye3.bmp");
    strcpy(arrMouseBmpNames[4].str, "h\\En.bmp");
    // [5] runtime-empty

    String20 arrMuneBmpNames[10][2];

    // --- body-part mask pairs (rnune/lnune/rsiri/lsiri/momo/asoko)
    memset(arrMuneBmpNames, 0, sizeof(arrMuneBmpNames));
    strcpy(arrMuneBmpNames[0][0].str, "rnuneA");   strcpy(arrMuneBmpNames[0][1].str, "rnuneB");
    strcpy(arrMuneBmpNames[1][0].str, "lnuneA");   strcpy(arrMuneBmpNames[1][1].str, "lnuneB");
    strcpy(arrMuneBmpNames[2][0].str, "rsiriA");   strcpy(arrMuneBmpNames[2][1].str, "rsiriB");
    strcpy(arrMuneBmpNames[3][0].str, "lsiriA");   strcpy(arrMuneBmpNames[3][1].str, "lsiriB");
    strcpy(arrMuneBmpNames[4][0].str, "momoA");    strcpy(arrMuneBmpNames[4][1].str, "momoB");
    strcpy(arrMuneBmpNames[5][0].str, "asokoA");   strcpy(arrMuneBmpNames[5][1].str, "asokoB");
    // [6][0] runtime-empty

    String20 arrModoNames[2][20];       // even mode: A/B name columns



    // --- modo sprite names, even variant: [0] A column, [1] B column.
    // Slot 9 of the B column is "taii" x5 in the original (kept).
    memset(arrModoNames, 0, sizeof(arrModoNames));
    strcpy(arrModoNames[0][0].str,  "modoA");      strcpy(arrModoNames[1][0].str,  "modoB");
    strcpy(arrModoNames[0][1].str,  "aoA");        strcpy(arrModoNames[1][1].str,  "aoB");
    strcpy(arrModoNames[0][2].str,  "umaA");       strcpy(arrModoNames[1][2].str,  "umaB");
    strcpy(arrModoNames[0][3].str,  "yoA");        strcpy(arrModoNames[1][3].str,  "yoB");
    strcpy(arrModoNames[0][4].str,  "manA");       strcpy(arrModoNames[1][4].str,  "manB");
    strcpy(arrModoNames[0][5].str,  "aitA");       strcpy(arrModoNames[1][5].str,  "aitB");
    strcpy(arrModoNames[0][6].str,  "yubiA");      strcpy(arrModoNames[1][6].str,  "yubiB");
    strcpy(arrModoNames[0][7].str,  "muneA");      strcpy(arrModoNames[1][7].str,  "muneB");
    strcpy(arrModoNames[0][8].str,  "feraA");      strcpy(arrModoNames[1][8].str,  "feraB");
    strcpy(arrModoNames[0][9].str,  "ao0A");       strcpy(arrModoNames[1][9].str,  "taii");
    strcpy(arrModoNames[0][10].str, "uma0A");      strcpy(arrModoNames[1][10].str, "taii");
    strcpy(arrModoNames[0][11].str, "yotu0A");     strcpy(arrModoNames[1][11].str, "taii");
    strcpy(arrModoNames[0][12].str, "man0A");      strcpy(arrModoNames[1][12].str, "taii");
    strcpy(arrModoNames[0][13].str, "ait0A");      strcpy(arrModoNames[1][13].str, "taii");
    strcpy(arrModoNames[0][14].str, "kouA");       strcpy(arrModoNames[1][14].str, "KouB");
    strcpy(arrModoNames[0][15].str, "taiA");       strcpy(arrModoNames[1][15].str, "taiB");
    strcpy(arrModoNames[0][16].str, "aiA");        strcpy(arrModoNames[1][16].str, "aiB");
    // [.][17..19] runtime-empty / zero

    String20 arrModoNamesOdd[40];       // odd mode: A rows 0..6, B at row+20 str[4]

    // --- modo sprite names, odd variant: A names in rows 0..6, B names at
    // .str[4] of rows 20..26 (bytes 0..3 of those rows stay zero)
    memset(arrModoNamesOdd, 0, sizeof(arrModoNamesOdd));
    strcpy(arrModoNamesOdd[0].str, "modoA");       strcpy(&arrModoNamesOdd[20].str[4], "modoB");
    strcpy(arrModoNamesOdd[1].str, "seiA");        strcpy(&arrModoNamesOdd[21].str[4], "seiB");
    strcpy(arrModoNamesOdd[2].str, "kouhA");       strcpy(&arrModoNamesOdd[22].str[4], "kouhB");
    strcpy(arrModoNamesOdd[3].str, "kijyoA");      strcpy(&arrModoNamesOdd[23].str[4], "kijyoB");
    strcpy(arrModoNamesOdd[4].str, "matuA");       strcpy(&arrModoNamesOdd[24].str[4], "matuB");
    strcpy(arrModoNamesOdd[5].str, "zaiA");        strcpy(&arrModoNamesOdd[25].str[4], "zaiB");
    strcpy(arrModoNamesOdd[6].str, "taiA");        strcpy(&arrModoNamesOdd[26].str[4], "taiB");
    // row 7 / row 27 .str[4] runtime-empty; rows 8..19 / 28..39 dead

    String20 arrVaiNames[6][5];         // group -> overlay page names
    String20 arrVaibNames[6][5];


    // --- overlay page names per group ([k][0] A, [k][1] B); row 0 unused.
    // arrVaibNames[4][2] is "edaA" in the original (sic).
    memset(arrVaiNames, 0, sizeof(arrVaiNames));
    memset(arrVaibNames, 0, sizeof(arrVaibNames));
    strcpy(arrVaiNames[1][0].str, "vaiA");         strcpy(arrVaibNames[1][0].str, "vaiB");
    strcpy(arrVaiNames[1][1].str, "rouA");         strcpy(arrVaibNames[1][1].str, "rouB");
    strcpy(arrVaiNames[2][0].str, "vaiA");         strcpy(arrVaibNames[2][0].str, "vaiB");
    strcpy(arrVaiNames[2][1].str, "BelA");         strcpy(arrVaibNames[2][1].str, "BelB");
    strcpy(arrVaiNames[3][0].str, "vaiA");         strcpy(arrVaibNames[3][0].str, "vaiB");
    strcpy(arrVaiNames[3][1].str, "kyuA");         strcpy(arrVaibNames[3][1].str, "kyuB");
    strcpy(arrVaiNames[3][2].str, "kanA");         strcpy(arrVaibNames[3][2].str, "kanB");
    strcpy(arrVaiNames[4][0].str, "vaiA");         strcpy(arrVaibNames[4][0].str, "vaiB");
    strcpy(arrVaiNames[4][1].str, "nasA");         strcpy(arrVaibNames[4][1].str, "nasB");
    strcpy(arrVaiNames[4][2].str, "edaA");         strcpy(arrVaibNames[4][2].str, "edaA"); // (sic)
    // [5][*], [1][2], [2][2] runtime-empty

    String20 arrAhWavNames[180];        // 5 groups x 30 (ah??_00..20 + ch??_00)

    // --- voice wav names: 5 group blocks of 30 slots = ah??_00..20 plus
    // ch??_00 at slot 21 (group tags 00, 05, 04, 03, 01; blocks 5..6 dead)
    memset(arrAhWavNames, 0, sizeof(arrAhWavNames));
    for (j = 0; j <= 20; ++j)
        wsprintfA(arrAhWavNames[j].str, "ah00_%02d", j);
    wsprintfA(arrAhWavNames[21].str, "ch00_00");
    for (j = 0; j <= 20; ++j)
        wsprintfA(arrAhWavNames[30 + j].str, "ah05_%02d", j);
    wsprintfA(arrAhWavNames[51].str, "ch05_00");
    for (j = 0; j <= 20; ++j)
        wsprintfA(arrAhWavNames[60 + j].str, "ah04_%02d", j);
    wsprintfA(arrAhWavNames[81].str, "ch04_00");
    for (j = 0; j <= 20; ++j)
        wsprintfA(arrAhWavNames[90 + j].str, "ah03_%02d", j);
    wsprintfA(arrAhWavNames[111].str, "ch03_00");
    for (j = 0; j <= 20; ++j)
        wsprintfA(arrAhWavNames[120 + j].str, "ah01_%02d", j);
    wsprintfA(arrAhWavNames[141].str, "ch01_00");

    String20 arrSeWavNames[10];         // hSE01a..hSE03c

    // --- SE wav names
    memset(arrSeWavNames, 0, sizeof(arrSeWavNames));
    strcpy(arrSeWavNames[0].str, "hSE01a");       strcpy(arrSeWavNames[1].str, "hSE01b");
    strcpy(arrSeWavNames[2].str, "hSE01c");       strcpy(arrSeWavNames[3].str, "hSE02a");
    strcpy(arrSeWavNames[4].str, "hSE02b");       strcpy(arrSeWavNames[5].str, "hSE02c");
    strcpy(arrSeWavNames[6].str, "hSE03a");       strcpy(arrSeWavNames[7].str, "hSE03b");
    strcpy(arrSeWavNames[8].str, "hSE03c");
    // [9] runtime-empty

    DWORD arrSceneRemap[20];            // selection -> flags bit index (+10)

    // --- selection -> unlock-bit remaps (bit index is value + 10)
    arrSceneRemap[0] = 0;    arrSceneRemap[1] = 5;    arrSceneRemap[2] = 6;
    arrSceneRemap[3] = 4;    arrSceneRemap[4] = 2;    arrSceneRemap[5] = 5;
    arrSceneRemap[6] = 6;    arrSceneRemap[7] = 4;    arrSceneRemap[8] = 1;
    arrSceneRemap[9] = 5;    arrSceneRemap[10] = 6;   arrSceneRemap[11] = 4;
    arrSceneRemap[12] = 3;   arrSceneRemap[13] = 5;   arrSceneRemap[14] = 6;
    arrSceneRemap[15] = 4;   arrSceneRemap[16] = 0;   arrSceneRemap[17] = 12;
    arrSceneRemap[18] = 13;  arrSceneRemap[19] = 14;

    DWORD arrModeRemap[5];

    arrModeRemap[0] = 7;     arrModeRemap[1] = 8;     arrModeRemap[2] = 9;
    arrModeRemap[3] = 10;    arrModeRemap[4] = 11;

    // --- even groups 1..4 swap modo slots 2 and 10 to the suwa pages
    if (iModeId && !(iModeId % 2))
    {
        strcpy(arrModoNames[0][2].str, "suwaA");
        strcpy(arrModoNames[1][2].str, "suwaB");
        strcpy(arrModoNames[0][10].str, "suwa0a");
        strcpy(arrModoNames[1][10].str, "taii");   // (sic) no-op rewrite
    }

    if (!g_sceneState_52F7E0.qwSelMask_C8)
        g_sceneState_52F7E0.qwSelMask_C8 = 2;
    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
    SetProjectionFov_40DCA3(0.5f);
    SetTimerScaleFactor(14.0f);
    memset(g_sceneState_52F7E0.ary33, 0, sizeof(g_sceneState_52F7E0.ary33));
    memset(arrBMPModos, 0, sizeof(arrBMPModos));
    memset(arrBMPMouses, 0, sizeof(arrBMPMouses));
    memset(arrBMPGo, 0, sizeof(arrBMPGo));
    memset(arrBMPNunes, 0, sizeof(arrBMPNunes));
    memset(arrScenes, 0, sizeof(arrScenes));
    memset(arrSoundAHs, 0, sizeof(arrSoundAHs));
    memset(arrSoundSEs, 0, sizeof(arrSoundSEs));

    // --- one-time loads behind the WAIT screen
    ToggleWaitScreen(1);
    for (i = 0; i < 30 && arrAhWavNames[30 * (iModeId / 2) + i].str[0]; ++i)
    {
        wsprintfA(pszFileName, "h\\%s.wav",
                  arrAhWavNames[30 * (iModeId / 2) + i].str);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1009);      // SE dir
        arrSoundAHs[i] = LoadWavToDS3DBuffer_42277E(pszFileName);
    }
    for (i = 0; i < 30 && arrSeWavNames[i].str[0]; ++i)
    {
        wsprintfA(pszFileName, "a\\%s.wav", arrSeWavNames[i].str);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1009);      // SE dir
        arrSoundSEs[i] = LoadWavToDS3DBuffer_42277E(pszFileName);
    }
    // cursor/face BMPs - all 10 load attempts run, the empty names fail
    for (i = 0; i < 10; ++i)
    {
        wsprintfA(pszFileName, "%s", arrMouseBmpNames[i].str);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1003);      // SPR dir
        LoadBmpWithMask_413A5E(&arrBMPMouses[i], pszFileName, pszFileName);
    }
    for (i = 0; i < 30 && arrGoBmpNames[i][0].str[0]; ++i)
    {
        wsprintfA(pszFileName, "%s", arrGoBmpNames[i][0].str);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1003);      // SPR dir
        wsprintfA(String, "%s", arrGoBmpNames[i][1].str);
        CHanaObj__GetRealPath_486D9A(String, 1003);           // SPR dir
        LoadBmpWithMask_413A5E(&arrBMPGo[i], pszFileName, String);
    }
    if (iModeId % 2)
    {
        // odd: single-page modo table (B name at str[4] of row + 20)
        for (i = 0; i < 30 && arrModoNamesOdd[i].str[0]; ++i)
        {
            wsprintfA(pszFileName, "h\\%s.bmp", arrModoNamesOdd[i].str);
            CHanaObj__GetRealPath_486D9A(pszFileName, 1003);  // SPR dir
            wsprintfA(String, "h\\%s.bmp", &arrModoNamesOdd[i + 20].str[4]);
            CHanaObj__GetRealPath_486D9A(String, 1003);       // SPR dir
            LoadBmpWithMask_413A5E(&arrBMPModos[i], pszFileName, String);
        }
    }
    else
    {
        // even: A/B columns, then the group's overlay pages appended
        for (i = 0; i < 30 && arrModoNames[0][i].str[0]; ++i)
        {
            wsprintfA(pszFileName, "h\\%s.bmp", arrModoNames[0][i].str);
            CHanaObj__GetRealPath_486D9A(pszFileName, 1003);  // SPR dir
            wsprintfA(String, "h\\%s.bmp", arrModoNames[1][i].str);
            CHanaObj__GetRealPath_486D9A(String, 1003);       // SPR dir
            LoadBmpWithMask_413A5E(&arrBMPModos[i], pszFileName, String);
        }
        for (j = 0; i + j < 30 && arrVaiNames[iModeId / 2][j].str[0]; ++j)
        {
            wsprintfA(pszFileName, "h\\%s.bmp", arrVaiNames[iModeId / 2][j].str);
            CHanaObj__GetRealPath_486D9A(pszFileName, 1003);  // SPR dir
            wsprintfA(String, "h\\%s.bmp", arrVaibNames[iModeId / 2][j].str);
            CHanaObj__GetRealPath_486D9A(String, 1003);       // SPR dir
            LoadBmpWithMask_413A5E(&arrBMPModos[i + j], pszFileName, String);
        }
    }
    for (i = 0; i < 30 && arrMuneBmpNames[i][0].str[0]; ++i)
    {
        wsprintfA(pszFileName, "h\\%s.bmp", arrMuneBmpNames[i][0].str);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1003);      // SPR dir
        wsprintfA(String, "h\\%s.bmp", arrMuneBmpNames[i][1].str);
        CHanaObj__GetRealPath_486D9A(String, 1003);           // SPR dir
        LoadBmpWithMask_413A5E(&arrBMPNunes[i], pszFileName, String);
    }

    // lock-on body
    wsprintfA(pszFileName, "lockon\\lock.sek");
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
    wsprintfA(String, "lockon\\Target.odf");
    CHanaObj__GetRealPath_486D9A(String, 1005);               // ODF dir
    LoadSekAndOdf_4445DD(pszFileName, String, &pLockOnBody);
    if (!pLockOnBody)
        return -1;
    SekBody__UpdateTo2_401647(pLockOnBody, 0);
    pLockOnBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;

    // mode body
    wsprintfA(pszFileName, "%s.sek", arrModeBodyNames[iModeId].str);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1005);          // ODF dir
    wsprintfA(String, "%s.odf", arrModeBodyNames[iModeId].str);
    CHanaObj__GetRealPath_486D9A(String, 1005);               // ODF dir
    LoadSekAndOdf_4445DD(pszFileName, String, &pModeBody);
    if (!pModeBody)
        return -1;
    AttachChild_4055FD(g_pSceneFrame, pModeBody->pSekHeader_220->pFrameT3EC);
    SetFramePosRel_4069DD(pModeBody->pSekHeader_220->pFrameT3EC,
            (CHanaFrameT3EC_S354 *)g_pSceneFrame, 0.0f, 0.0f, 0.0f);
    UpdateBoneLookAtTransform(pModeBody->pSekHeader_220->pFrameT3EC,
            (CHanaFrameT3EC_S354 *)g_pSceneFrame,
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    pModeBody->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
    InitializePracticeAnimations_4667A9(pModeBody->pSekHeader_220->pFrameT3EC);
    SekBody__UpdateTo2_401647(pModeBody, 3);

    // Tears bodies
    memset(arrTearsBodies, 0, sizeof(arrTearsBodies));
    for (i = 0; i < 5 && arrTearsOdfs[iModeId / 2][i].str[0]; ++i)
    {
        wsprintfA(pszFileName, "%s.sek", arrTearsSeks[i].str);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1005);      // ODF dir
        wsprintfA(String, "%s.odf", arrTearsOdfs[iModeId / 2][i].str);
        CHanaObj__GetRealPath_486D9A(String, 1005);           // ODF dir
        LoadSekAndOdf_4445DD(pszFileName, String, &arrTearsBodies[i]);
        if (!arrTearsBodies[i])
            return -1;
        arrTearsBodies[i]->pSekHeader_220->pFrameT3EC->bAlphaDeferred_130 = 1;
    }

    // BGM (even -> s012, odd -> s013), looped at AVI volume - 1000 dB
    i = (iModeId % 2) != 0;
    wsprintfA(pszFileName, "%s", arrBgmWavs[i]);
    CHanaObj__GetRealPath_486D9A(pszFileName, 1006);          // WAV dir
    g_pDSBufAssault_52EFD0 = LoadWav_422E7B(pszFileName);
    if (g_pDSBufAssault_52EFD0)
    {
        // clamp table[aviVolume] - 1000 dB to [-10000, 0]
        if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_sysDataSave_530170.g_nVolumeLevel_530266] - 1000.0) >= -10000)
        {
            if ((int)(__int64)(g_fVolumeDbTable[(unsigned __int8)g_sysDataSave_530170.g_nVolumeLevel_530266] - 1000.0) <= 0)
                g_pDSBufAssault_52EFD0->SetVolume((__int64)(g_fVolumeDbTable[(unsigned __int8)g_sysDataSave_530170.g_nVolumeLevel_530266] - 1000.0));
            else
                g_pDSBufAssault_52EFD0->SetVolume(0);
        }
        else
        {
            g_pDSBufAssault_52EFD0->SetVolume(-10000);
        }
    }
    if (g_pDSBufAssault_52EFD0)
        DSBufferPlay(g_pDSBufAssault_52EFD0, 1);
    for (i = 0; i < 20; ++i)      // (no-op here: no scene is loaded yet)
    {
        if (arrScenes[i])
        {
            ApplyFrameEffectOption(arrScenes[i]);
            Scene_FindMapFrame(arrScenes[i]);
        }
    }

    // persist the auto-selected first scene's unlock bit
    if (iModeId % 2)
        g_sceneState_52F7E0.girlsStateBits_98[iModeId / 2] |=
            (unsigned __int64)1 << (arrModeRemap[piSelected] + 10);
    else
        g_sceneState_52F7E0.girlsStateBits_98[iModeId / 2] |=
            (unsigned __int64)1 << (arrSceneRemap[piSelected] + 10);
    ToggleWaitScreen(0);

    while (1)  // outer: one selection pass + one sub-scene playback
    {
        RestartInputThread(2);
        pfExecTime = 5.0f;
        FrameTimer_BeginFrame();
        FrameTimer_EndFrame();
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFFFFFF);
        g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
        if (bFirstPassDone)
        {
            // run the group's selection screen (even: 20-scene grid,
            // odd: 5 mode buttons) and persist the chosen unlock bit
            if (!(iModeId % 2))
            {
                nResult = RunSceneSelectScreen(arrScenes[piSelected], arrBMPModos,
                        arrBMPMouses, &piSelected, iModeId / 2,
                        &g_sceneState_52F7E0.qwSelMask_C8, iMenuTab);
                if (nResult < 1 && piSelected >= 0)
                    g_sceneState_52F7E0.girlsStateBits_98[iModeId / 2] |=
                        (unsigned __int64)1 << (arrSceneRemap[piSelected] + 10);
            }
            else
            {
                nResult = RunModeSelectScreen(arrScenes[piSelected], arrBMPModos,
                        arrBMPMouses, &piSelected, iModeId,
                        &g_sceneState_52F7E0.qwSelMask_C8, iMenuTab);
                if (nResult < 1 && piSelected >= 0)
                    g_sceneState_52F7E0.girlsStateBits_98[iModeId / 2] |=
                        (unsigned __int64)1 << (arrModeRemap[piSelected] + 10);
            }
            if (nResult >= 1)
                break;
        }
        if (piSelected < 0)  // selection done: all choices made
            break;
        bFirstPassDone = 1;
        pSceneAny = 0;
        pSceneNew = 0;
        for (i = 0; i < 30; ++i)
        {
            if (arrScenes[i])
            {
                pSceneAny = arrScenes[i];
                break;
            }
        }
        if (!arrSceneNames[iModeId][piSelected][0])
        {
            // dead slot: drop every loaded scene and pick again
            for (i = 0; i < 30; ++i)
            {
                if (arrScenes[i])
                {
                    CHanaObj__Free_40C1E4(arrScenes[i]);
                    arrScenes[i] = 0;
                }
            }
            continue;
        }
        ToggleWaitScreen(1);
        wsprintfA(pszFileName, "%s.Ods", arrSceneNames[iModeId][piSelected]);
        CHanaObj__GetRealPath_486D9A(pszFileName, 1002);      // ODS dir
        if (pSceneAny)
        {
            // reuse another live scene's frame/anim-set tables
            if (!LoadOdsEx_48564B(pszFileName, &pSceneNew, pSceneAny->refsFrames_134,
                    pSceneAny->refsAnimeSets_138, pSceneAny->dwCount_130))
            {
                ToggleWaitScreen(0);
                return -1;
            }
        }
        else if (!LoadOds_4843CF(pszFileName, &pSceneNew))
        {
            ToggleWaitScreen(0);
            return -1;
        }
        ToggleWaitScreen(0);
        for (i = 0; i < 30; ++i)
        {
            if (arrScenes[i])
            {
                CHanaObj__Free_40C1E4(arrScenes[i]);
                arrScenes[i] = 0;
            }
        }
        arrScenes[piSelected] = pSceneNew;
        // pre-roll: reset the bodies, run the script head at t=1 then t=5
        pfExecTime = 1.0f;
        UpdateSceneAtTime_4825D5(arrScenes[piSelected], &pfExecTime, 1.0f);
        ParseExecStr_487D89(arrScenes[piSelected], &pfExecTime);
        pfExecTime = 5.0f;
        ParseExecStr_487D89(arrScenes[piSelected], &pfExecTime);
        ResetSceneBodies_4857CA(arrScenes[piSelected]);
        ApplyFrameEffectOption(arrScenes[piSelected]);
        // hide every frame of every scene, then show only the new one's
        for (i = 0; i < 30; ++i)
        {
            if (arrScenes[i])
            {
                for (j = 0; j < (int)arrScenes[i]->dwCount_130; ++j)
                    arrScenes[i]->refsFrames_134[j]->bAlphaDeferred_130 = 1;
            }
        }
        if (arrScenes[piSelected])
        {
            for (j = 0; j < (int)arrScenes[piSelected]->dwCount_130; ++j)
                arrScenes[piSelected]->refsFrames_134[j]->bAlphaDeferred_130 = 0;
        }
        ApplyFrameEffectOption(arrScenes[piSelected]);
        Scene_FindMapFrame(arrScenes[piSelected]);
        D3DSetRenderStates_4276C0();

        while (1)  // inner: frame loop of the loaded sub-scene
        {
            DoPeekMessages();
            fDeltaTime = (float)GetFrameTime();
            if (fDeltaTime > 5.0f)
                fDeltaTime = 5.0f;
            FrameTimer_EndFrame();

            // menu key -> system menu (tab 0 / 2 by iMenuTab) on the BGM
            if (IsMenuKeyPressed_481170())
            {
                PauseSceneAudio_46A2A0(arrScenes[piSelected], 1);
                RestartInputThread(0);
                sceneMenuCopy = g_sceneState_52F7E0;
                nResult = SystemMenu_4655AB(g_pDSBufAssault_52EFD0, -1000,
                        iMenuTab ? 2 : 0, g_sceneState_52F7E0.protCurrentHP_C0,
                        g_sceneState_52F7E0.protMaximumHP_C4, sceneMenuCopy);
                RestartInputThread(2);
                ApplyFrameEffectOption(arrScenes[piSelected]);
                PauseSceneAudio_46A2A0(arrScenes[piSelected], 0);
                for (i = 0; i < 20; ++i)
                {
                    if (arrScenes[i])
                        Scene_FindMapFrame(arrScenes[i]);
                }
                if (nResult >= 1)
                    break;
            }
            btnMousePrev = btnMouse;
            GetMouseBtn_45A41B(&btnMouse);
            GetMouseXY_45A4D8((DWORD *)&xMouse, (DWORD *)&yMouse);
            // right button toggles the overlay/skip mode
            if (btnMouse == 2 && !btnMousePrev)
                bSkipMode = !bSkipMode;
            // left-click in the top-right corner -> cut-in viewer
            if (piSelected < 14 && btnMouse == 1 && !btnMousePrev
                && xMouse + 16 > ConvertFrom640AsInt(576)
                && yMouse < ConvertFrom640AsInt(32)
                && !arrScenes[piSelected]->pSurface_468)
            {
                nResult = PlayCutInScene_46A76F(arrScenes[piSelected],
                        arrBMPMouses, arrBMPGo, arrBMPNunes, pLockOnBody,
                        pModeBody, iModeId / 2, arrSoundAHs, arrSoundSEs,
                        arrTearsBodies, iMenuTab);
                if (nResult == 1)
                    break;
                btnMousePrev = btnMouse;
            }
            // both buttons held: 5x speed (not in skip mode)
            if (btnMouse == 3 && !bSkipMode)
                fDeltaTime = fDeltaTime * 5.0f;
            if (arrScenes[piSelected])
            {
                UpdateSceneTimer_4827FE(arrScenes[piSelected], &pfExecTime,
                                        fDeltaTime);
                if (!UpdateSceneAtTime_4825D5(arrScenes[piSelected], &pfExecTime,
                                              fDeltaTime))
                    break;  // scene clock ran out
            }
            fTextSpeed = 1.0f;
            if (!bSkipMode)
            {
                if (!ParseExecStr_487D89(arrScenes[piSelected], &pfExecTime))
                {
                    // script ran dry: rewind the scene, back to selection
                    pfExecTime = 5.0f;
                    arrScenes[piSelected]->dwPtrPos_444 = 0;
                    arrScenes[piSelected]->dwPtrCurrent_448 = 0;
                    arrScenes[piSelected]->flt_450 = 0.0f;
                    arrScenes[piSelected]->flt_454 = 1.0f;
                    arrScenes[piSelected]->flt_458 = 1.0f;
                    arrScenes[piSelected]->WCurNum = 0.0f;
                    arrScenes[piSelected]->by_460 = 0;
                    arrScenes[piSelected]->by_461 = 0;
                    arrScenes[piSelected]->by_462 = 0;
                    arrScenes[piSelected]->by_463 = 0;
                    arrScenes[piSelected]->flt_464 = 0.0f;
                    break;
                }
                // a click during a text wait fast-forwards it
                if (arrScenes[piSelected]
                    && (btnMouse == 3 || (btnMouse == 1 && btnMouse != btnMousePrev))
                    && arrScenes[piSelected]->by_461
                    && !arrScenes[piSelected]->by_460)
                {
                    arrScenes[piSelected]->by_461 = 0;
                }
                if (btnMouse == 1 && btnMouse != btnMousePrev)
                    fTextSpeed = 1000.0f;
            }
            if (arrScenes[piSelected] && arrScenes[piSelected]->pSurface_468)
            {
                // still-image scene: blit the surface instead of 3D
                g_pD3DDevice_4D6C78->GetRenderState(D3DRENDERSTATE_ANTIALIAS,
                                                    &dwAntialiasSave);
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS, 0);
                FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
                g_pBackBuffer_4D6C90->Blt(0, arrScenes[piSelected]->pSurface_468,
                                           0, 0x1000000, 0);   // DDBLT_WAIT
                FlushD3DAndTouchSurface(g_pBackBuffer_4D6C90);
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ANTIALIAS,
                                                    dwAntialiasSave);
            }
            else
            {
                MtxStackStartMultLocal_4074EF();
                g_pRenderCtx->Clear(3);
                if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
                {
                    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 0);
                    HanaFrame_Draw((CHanaFrameT3EC_S354 *)g_pSceneFrame);
                    g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
                    HanaVert_DrawBatch(1);
                    g_pD3DDevice_4D6C78->EndScene();
                }
            }
            if (!bSkipMode)
                DrawDialogueWindow_480B2D(arrScenes[piSelected],
                        fDeltaTime * fTextSpeed, xMouse, yMouse, btnMouse);
            if (g_pD3DDevice_4D6C78->BeginScene() >= 0)
            {
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, 1);
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_SRCBLEND, 5);   // SRCALPHA
                g_pD3DDevice_4D6C78->SetRenderState(D3DRENDERSTATE_DESTBLEND, 6);  // INVSRCALPHA
                if (!bSkipMode)
                {
                    // Go2b prompt fades out below x=576, in above it
                    if (piSelected < 14 && arrBMPGo[2])
                    {
                        if (xMouse + 16 <= ConvertFrom640AsInt(576))
                            CHanaObj__SetAlpha3F6(arrBMPGo[2],
                                    arrBMPGo[2]->fAlpha_144 - fDeltaTime / 10.0f);
                        else
                            CHanaObj__SetAlpha3F6(arrBMPGo[2],
                                    fDeltaTime / 10.0f + arrBMPGo[2]->fAlpha_144);
                        CHanaSpriteT3F6__SetPos_414A5D(arrBMPGo[2],
                                (float)ConvertFrom640AsFloat(608.0),
                                (float)ConvertFrom640AsFloat(16.0));
                        DrawSprite2D(arrBMPGo[2]);
                    }
                    if (arrBMPMouses[0])
                    {
                        CHanaSpriteT3F6__SetPos_414A5D(arrBMPMouses[0],
                                (float)(xMouse + 16), (float)(yMouse + 16));
                        DrawSprite2D(arrBMPMouses[0]);
                    }
                    // face overlays only track the cursor
                    for (i = 1; i < 10; ++i)
                    {
                        if (arrBMPMouses[i])
                            CHanaSpriteT3F6__SetPos_414A5D(arrBMPMouses[i],
                                    (float)xMouse, (float)yMouse);
                    }
                }
                g_pD3DDevice_4D6C78->EndScene();
            }
            PresentFrame();
            HanaVert_DrawBatch(0);

            // fade each voice wav 300 dB/s toward -10000; a voice already
            // <= -1000 is stopped (sic: Stop without a null check)
            for (i = 0; i < 30; ++i)
            {
                if (arrSoundAHs[i])
                {
                    nVoiceFadeVol = DSBufferGetVolume_423C65(
                            arrSoundAHs[i]->pDSBuffer_0);
                    if (nVoiceFadeVol >= -1000)
                    {
                        if (arrSoundAHs[i]->pDSBuffer_0)
                        {
                            if (nVoiceFadeVol - (int)(__int64)(fDeltaTime * 300.0) >= -10000)
                            {
                                if (nVoiceFadeVol - (int)(__int64)(fDeltaTime * 300.0) <= 0)
                                    arrSoundAHs[i]->pDSBuffer_0->SetVolume(
                                        nVoiceFadeVol - (int)(__int64)(fDeltaTime * 300.0));
                                else
                                    arrSoundAHs[i]->pDSBuffer_0->SetVolume(0);
                            }
                            else
                            {
                                arrSoundAHs[i]->pDSBuffer_0->SetVolume(-10000);
                            }
                        }
                    }
                    else
                    {
                        arrSoundAHs[i]->pDSBuffer_0->Stop();
                    }
                }
            }
            *(DWORD *)arrModoNamesOdd[20].str = xMouse;  // (sic) dead stores
            yMouseStash = yMouse;
        }
        RestartInputThread(0);
        if (nResult >= 1)
            break;
    }

    // --- cleanup
    RestartInputThread(0);
    if (nResult <= 0)
        PlayScreenFade(0, 0, 0, 10.0f, 0);
    for (i = 0; i < 5; ++i)
    {
        if (arrTearsBodies[i])
        {
            AttachChild_4055FD(g_pSceneFrame,       // (sic) re-attach before
                    arrTearsBodies[i]->pSekHeader_220->pFrameT3EC); // release
            CSekBody__Release_4012FE(arrTearsBodies[i]);
        }
    }
    CSekBody__Release_4012FE(pLockOnBody);
    CSekBody__Release_4012FE(pModeBody);
    if (g_pDSBufAssault_52EFD0)
    {
        g_pDSBufAssault_52EFD0->Stop();
        g_pDSBufAssault_52EFD0->Release();
        g_pDSBufAssault_52EFD0 = 0;
    }
    for (i = 0; i < 30; ++i)
    {
        if (arrBMPGo[i])
        {
            CHanaObj__Free_40C1E4(arrBMPGo[i]);
            arrBMPGo[i] = 0;
        }
        if (arrBMPModos[i])
        {
            CHanaObj__Free_40C1E4(arrBMPModos[i]);
            arrBMPModos[i] = 0;
        }
        if (arrBMPNunes[i])
        {
            CHanaObj__Free_40C1E4(arrBMPNunes[i]);
            arrBMPNunes[i] = 0;
        }
    }
    for (i = 0; i < 10; ++i)
    {
        if (arrBMPMouses[i])
        {
            CHanaObj__Free_40C1E4(arrBMPMouses[i]);
            arrBMPMouses[i] = 0;
        }
    }
    for (i = 0; i < 30; ++i)
    {
        if (arrScenes[i])
        {
            CHanaObj__Free_40C1E4(arrScenes[i]);
            arrScenes[i] = 0;
        }
    }
    for (i = 0; i < 30; ++i)
    {
        FreeSoundStruct_422C72(arrSoundAHs[i]);
        FreeSoundStruct_422C72(arrSoundSEs[i]);
    }
    g_sceneState_52F7E0.qwSelMask_C8 = 0;
    return nResult;
}

// ToggleWaitScreen - Start or stop the blinking WAIT-screen thread
// // Called only from Scene4_4703AE (push 1 = show, push 0 = hide)
// // 
// // Arguments:
// //   bShow: 1 = start the wait screen (only when no thread is running),
// //          0 = stop it (also taken when a thread is already running)
// // 
// // Show path:
// //   1. g_bWaitScreenOn = 1 (loop condition of the thread)
// //   2. g_hwndWaitScreen = g_hwndParent; invalidate both windows
// //   3. Blit 640x480 black to primary + back buffer, UpdateFrame + FlipToGDISurface x2
// //   4. hWaitScreenThread = CreateThread(WaitScreenThreadProc), priority ABOVE_NORMAL
// // 
// // Hide path:
// //   1. g_bWaitScreenOn = 0; thread notices on its next 10ms tick
// //   2. Poll GetExitCodeThread up to 1000 x 5ms (5s); if still STILL_ACTIVE (259)
// //      -> TerminateThread(hWaitScreenThread, 1)
// //   3. CloseHandle + primary buffer Lock/Unlock
void __cdecl hook_ToggleWaitScreen(BOOL bShow)
{
  int v1; // eax
  int v2; // eax
  int v3; // [esp-8h] [ebp-198h]
  int v4; // [esp-8h] [ebp-198h]
  int i; // [esp+4h] [ebp-18Ch]
  int bNorm; // [esp+8h] [ebp-188h]
  DWORD ThreadId; // [esp+Ch] [ebp-184h] BYREF
  DDSURFACEDESC2 ddSurfaceDesc; // [esp+10h] [ebp-180h] BYREF
  char szWeightTxt[256]; // [esp+8Ch] [ebp-104h] BYREF
  //char v10; // [esp+18Bh] [ebp-5h]
  DWORD ExitCode; // [esp+18Ch] [ebp-4h] BYREF

  ONCELOG("[HNH:SUB] ToggleWaitScreen_47E3E1\n");
  

  strcpy(szWeightTxt, "ウエイト");
  
  ddSurfaceDesc.dwSize = 124;
  if ( !bShow || g_hWaitScreenThread )
  {
    g_bWaitScreenOn = 0;
    Sleep(5u);
    if ( g_hWaitScreenThread )
    {
      bNorm = 1;
      for ( i = 0; i < 1000; ++i )
      {
        Sleep(5u);
        if ( !GetExitCodeThread(g_hWaitScreenThread, &ExitCode) )
          break;
        if ( ExitCode != 259 )
        {
          bNorm = 0;
          break;
        }
        SetThreadPriority(g_hWaitScreenThread, 0);
      }
      if ( bNorm == 1 )
        TerminateThread(g_hWaitScreenThread, 1u);
      CloseHandle(g_hWaitScreenThread);
      g_hWaitScreenThread = 0;
      g_pPrimaryBuffer_4D6C88->Lock(0, &ddSurfaceDesc, 1, 0);
      g_pPrimaryBuffer_4D6C88->Unlock(0);
    }
  }
  else
  {
    g_bWaitScreenOn = 1;
    InvalidateRect(g_hwndParent, 0, 0);
    InvalidateRect(g_hwndWaitScreen, 0, 0);
    g_hwndWaitScreen = g_hwndParent;
    v3 = ConvertFrom640AsInt(480);
    v1 = ConvertFrom640AsInt(640);
    BltSurface(g_pPrimaryBuffer_4D6C88, 0, 0, v1, v3, 0);
    v4 = ConvertFrom640AsInt(480);
    v2 = ConvertFrom640AsInt(640);
    BltSurface(g_pBackBuffer_4D6C90, 0, 0, v2, v4, 0);
    g_pRenderCtx->UpdateFrame(1);
    g_pDD_4D6C7C->FlipToGDISurface();
    g_pPrimaryBuffer_4D6C88->Lock(0, &ddSurfaceDesc, 1, 0);
    g_pPrimaryBuffer_4D6C88->Unlock(0);
    g_pDD_4D6C7C->FlipToGDISurface();
    g_hWaitScreenThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)WaitScreenThreadProc, 0, 0, &ThreadId);
    SetThreadPriority(g_hWaitScreenThread, 2);
    Sleep(50u);
  }
}

// WaitScreenThreadProc - Thread proc rendering a blinking WAIT screen
// // Started by ToggleWaitScreen (0x47E3E1) via CreateThread, priority ABOVE_NORMAL
// // 
// // Behavior:
// //   1. Loads wait.bmp, GetDC(g_hwndWaitScreen)
// //   2. While g_bWaitScreenOn == 1: every 100th 10ms tick, alternate between
// //      StretchDIBits(wait.bmp, centered) and black FillRect -> blinking effect
// //   3. On exit: ReleaseDC, FreeBmp, g_hwndWaitScreen = 0, ExitThread(1)
void __stdcall hook_WaitScreenThreadProc(LPVOID lpThreadParameter)
{
  int xdest; // eax
  int ydest; // [esp-2Ch] [ebp-94h]
  int biWidth; // [esp-18h] [ebp-80h]
  unsigned int biHeight; // [esp-14h] [ebp-7Ch]
  LPBYTE pData; // [esp-10h] [ebp-78h]
  const BITMAPINFO *pBitmapInfoHeader; // [esp-Ch] [ebp-74h]
  CBMPFile *pBmp; // [esp+0h] [ebp-68h]
  int nPrevPercent; // [esp+4h] [ebp-64h]
  CHAR FileName[64]; // [esp+8h] [ebp-60h] BYREF
  HDC hdc; // [esp+48h] [ebp-20h]
  HBRUSH hbr; // [esp+4Ch] [ebp-1Ch]
  int nCount; // [esp+50h] [ebp-18h]
  int nPercent; // [esp+54h] [ebp-14h]
  RECT rc; // [esp+58h] [ebp-10h] BYREF

  ONCELOG("[HNH:SUB] WaitScreenThreadProc_47E1A2\n");
 
  hdc = 0;
  nCount = 0;
  nPrevPercent = 1;
  rc.left = 0;
  rc.top = 0;
  rc.right = ConvertFrom640AsInt(640);
  rc.bottom = ConvertFrom640AsInt(480);
  wsprintfA(FileName, "Wait.bmp");
  CHanaObj__GetRealPath_486D9A(FileName, 1007);
  pBmp = LoadBmp_420DF0(FileName);
  if ( pBmp )
  {
    hdc = GetDC(g_hwndWaitScreen);
    if ( hdc && hdc != (HDC)-1 )
    {
      while ( g_bWaitScreenOn == 1 )
      {
        nPercent = ++nCount / 100 % 2;
        if ( nPrevPercent != nPercent )
        {
          g_pDD_4D6C7C->FlipToGDISurface();
          nPrevPercent = nPercent;
          if ( nPercent )
          {
            pBitmapInfoHeader = (const BITMAPINFO *)pBmp->pBitmapInfoHeader;
            pData = pBmp->pData;
            biHeight = pBitmapInfoHeader->bmiHeader.biHeight;
            biWidth = pBitmapInfoHeader->bmiHeader.biWidth;
            ydest = ConvertFrom640AsInt(240) - (biHeight >> 1);
            xdest = ConvertFrom640AsInt(320);
            StretchDIBits(
              hdc,
              xdest - ((unsigned int)pBmp->pBitmapInfoHeader->biWidth >> 1),
              ydest,
              biWidth,
              biHeight,
              0,
              0,
              biWidth,
              biHeight,
              pData,
              pBitmapInfoHeader,
              0,
              0xCC0020u);
          }
          else
          {
            hbr = CreateSolidBrush(0);
            FillRect(hdc, &rc, hbr);
            DeleteObject(hbr);
          }
        }
        Sleep(0xAu);
      }
      ReleaseDC(g_hwndWaitScreen, hdc);
    }
    FreeBmp_4211ED(pBmp);
  }
  g_hwndWaitScreen = 0;
  ExitThread(1u);
}

CHanaFrameT3EC_S354 *__cdecl hook_Scene_FindMapFrame(CHanaSceneT458_S504 *pScene)
{
  unsigned int i; // [esp+0h] [ebp-8h]
  CHanaFrameT3EC_S354 *pNotFound; // [esp+4h] [ebp-4h]

  ONCELOG("[HNH:SUB] Scene_FindMapFrame_46777D\n");
 
  pNotFound = 0;
  if ( !pScene )
    return 0;
  for ( i = 0; i < pScene->dwCount_130; ++i )
  {
    if ( !_memicmp(pScene->refsFrames_134[i]->szName_8, "Map\\", 4u) )
    {
      pScene->refsFrames_134[i]->bAlphaDeferred_130 = g_sysDataSave_530170.g_nHSceneBackground_530264 & 1;
      return pScene->refsFrames_134[i];
    }
  }
  return pNotFound;
}

void __cdecl hook_InitializePracticeAnimations_4667A9(CHanaFrameT3EC_S354 *pRootFrame)
{
  int i; // [esp+4h] [ebp-D0h]
  //String 50 szAnimNames[4]; // [esp+8h] [ebp-CCh] BYREF
  CHanaFrameT3EC_S354 *pFrame; // [esp+D0h] [ebp-4h]

  ONCELOG("[HNH:SUB] InitializePracticeAnimations_4667A9\n");
  
#if 1
  char szAnimNames[4][50] = {
    "_h00_all",
    "_p00_",
    "_d00_all",
    ""
  };
#else
  strcpy((char *)szAnimNames[0].str, "_h00_all");
  memset((void *)&szAnimNames[0].str[9], 0, 0x28u);
  szAnimNames[0].str[49] = 0;
  strcpy((char *)szAnimNames[1].str, "_p00_");
  memset((void *)&szAnimNames[1].str[6], 0, 0x2Cu);
  strcpy((char *)szAnimNames[2].str, "_d00_all");
  memset((void *)&szAnimNames[2].str[9], 0, 0x28u);
  szAnimNames[2].str[49] = 0;
  szAnimNames[3].str[0] = 0;
  memset((void *)&szAnimNames[3].str[1], 0, 0x30u);
  szAnimNames[3].str[49] = 0;
#endif

  for ( i = 0; i < 3; ++i )
  {
    pFrame = FindFrameByName_47E999(pRootFrame, szAnimNames[i]);
    if ( pFrame )
      Frame_SetPracticeAnimRootRecursive(pFrame, pFrame);
  }
}

void __cdecl hook_Frame_SetPracticeAnimRootRecursive(CHanaFrameT3EC_S354 *pFrame, CHanaFrameT3EC_S354 *pRootAnimFrame)
{
  CHanaNode *pChildFrames; // [esp+4h] [ebp-8h]
  unsigned int i; // [esp+8h] [ebp-4h]

  ONCELOG("[HNH:SUB] Frame_SetPracticeAnimRootRecursive_466738\n");
  
  pFrame->field_11C = (int)pRootAnimFrame;
  if ( pFrame->nCountChild )
  {
    pChildFrames = pFrame->pFirstChildNode;
    for ( i = 0; i < pFrame->nCountChild; ++i )
    {
      Frame_SetPracticeAnimRootRecursive(pChildFrames->pData, pRootAnimFrame);
      pChildFrames = pChildFrames->pNext;
    }
  }
}


void register_PlayAssault_4703AE(int cfg) {
  if (cfg == 1) {
    ToggleWaitScreen = hook_ToggleWaitScreen;
    Scene_FindMapFrame = hook_Scene_FindMapFrame;
    InitializePracticeAnimations_4667A9 = hook_InitializePracticeAnimations_4667A9;
    WaitScreenThreadProc = hook_WaitScreenThreadProc;
    Frame_SetPracticeAnimRootRecursive = hook_Frame_SetPracticeAnimRootRecursive;
    PlayAssault_4703AE = hook_PlayAssault_4703AE;
  }

  hnh_HookOff();
  hnh_Hook(0x4703AE, hook_PlayAssault_4703AE, "PlayAssault_4703AE"); // [OK]

  hnh_Hook(0x466738, hook_Frame_SetPracticeAnimRootRecursive, "Frame_SetPracticeAnimRootRecursive_466738"); // [OK]
  hnh_Hook(0x4667a9, hook_InitializePracticeAnimations_4667A9, "InitializePracticeAnimations_4667A9"); // [OK]
  hnh_Hook(0x47e3e1, hook_ToggleWaitScreen, "ToggleWaitScreen_47E3E1"); // [OK]
  hnh_Hook(0x47e1a2, hook_WaitScreenThreadProc, "WaitScreenThreadProc_47E1A2"); // [OK]
  hnh_Hook(0x46777d, hook_Scene_FindMapFrame, "Scene_FindMapFrame_46777D"); // [OK]

  hnh_HookOn();
}
