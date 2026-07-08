## ObjAt_1A8 — Scene Stack Manager (CSubGlobalAt_1A8)

    ObjAt_1A8 is the game's scene/layer dispatch system. It holds a stack of up to 10 active scenes (game screens like
     Logo, Title, Map, Room, Play, etc.), each with function pointers for the update and render phases. Render_642BC0
    is the main render loop that dispatches these callbacks.

    Structure layout (548 bytes / 0x224)


    ┌────────┬────────────────────┬───────────────────────────────────────────────────────────────┐
    │ Offset │ Name               │ Purpose                                                       │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x00  │ nState             │ Current scene stack depth (0 = no scene)                      │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x01  │ nIndex             │ Index into ArrStruct_34 of the top/active scene               │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x02  │ field_1AA          │ Transition state, copied from previous scene                  │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x04  │ ArrStruct_34[10]   │ Array of 10 scene entries (CStruct_34)                        │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x20C │ SetCursorPosFn_20C │ Pre-render cursor setup callback                              │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x210 │ field_210          │ Pre-update callback                                           │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x214 │ field_214          │ Pre-update callback 2                                         │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x218 │ field_218          │ Mid-render callback (after 3D, before UI)                     │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x21C │ field_21C          │ Post-render callback                                          │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x220 │ field_220          │ "Dirty" flag — when set, Render_642BC0 skips the entire frame │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x221 │ field_221          │ Enable anaglyph/stereo 3D pass                                │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x222 │ field_222          │ Enable 3D scene render pass (pfnDo_14)                        │
    ├────────┼────────────────────┼───────────────────────────────────────────────────────────────┤
    │ +0x223 │ field_223          │ Enable UI/2D render pass (pfnDo_18/pfnDo_1C)                  │
    └────────┴────────────────────┴───────────────────────────────────────────────────────────────┘

    Each CStruct_34 entry contains a CStructScene_24 with 7 function pointers:
     - `pfnNew` — scene constructor (called on push)
     - `pfnDo_0C` — main scene update
     - `pfnDo_10` — child scene update
     - `pfnDo_14` — 3D render
     - `pfnDo_18` — UI render (main scene)
     - `pfnDo_1C` — UI render (child scenes)
     - `pfnDo_20` — destructor/cleanup

    Where it's filled

     1. Zeroed at startup: WinMain does memset(&g_SBZGlobal, 0, ...) (0x403fbf)

     2. Array init — sub_643A20 (0x643A20):
        - Zeroes ArrStruct_34, sets all nListIndex entries to 0xFF (no-link sentinel)

     3. Scene push — sub_6431F0 (0x6431F0), the core registration function:
        - Finds next free slot (nIndex + 1)
        - Copies the CStructScene_24 descriptor into ArrStruct_34[nextSlot].Scene
        - Increments nState, updates nIndex
        - Calls pfnNew to initialize the scene
        - Sets field_220 = 1 (dirty flag → skip one render frame)

     4. Scene factory — InitScene_4951A0 (0x4951A0):
        - Large switch dispatching ~30 scene types (Logo=0, Title=1, Map=2, Room=3, Shop=4, ..., Escape=0x19,
          Special=0x1A, etc.)
        - Each scene's Init_XXX fills a CStructScene_24 with its callbacks (see SBLogoScene::Init_5442A0 above)
        - Then calls sub_643800 (immediate) or sub_6431F0 (fade transition) to push it

     5. First scene: InitFirst_401000 calls InitScene_4951A0(0, 0, 3, 3, 500, 500) → boots into Logo scene (type 0)

    What it's for (in Render_642BC0)

    The Render function reads ObjAt_1A8 to drive the entire frame:

      1 1. If nState > 0 → get active scene from ArrStruct_34[nIndex]
      2 2. Call SetCursorPosFn_20C (cursor setup)
      3 3. UPDATE PHASE: for each child scene in nListIndex → call pfnDo_10
      4                  then call pfnDo_0C on the main scene
      5 4. If field_220 is set → return immediately (skip frame, scene just changed)
      6 5. Clear + BeginScene
      7 6. If field_222 → 3D RENDER: call pfnDo_14 on all scenes
      8 7. If field_221 → anaglyph pass
      9 8. If field_223 → UI RENDER: call pfnDo_18/pfnDo_1C
     10 9. EndScene + Present

    In short: `ObjAt_1A8` is a stack-based scene manager. Scenes are pushed via InitScene_4951A0 → sub_6431F0, each
    registering callbacks for update/3D-render/UI-render phases. Render_642BC0 dispatches those callbacks every frame
    in the correct order with proper D3D9 device state management.


## Relationship to BuildGaussianKernel (sub_69D640)


    ┌───────────┬────────────────────────────────┬─────────────────────────────────────────────────────┐
    │           │ BuildGaussianKernel (0x69D640) │ BuildGaussianKernel1D (0x69D890)                    │
    ├───────────┼────────────────────────────────┼─────────────────────────────────────────────────────┤
    │ Dimension │ 2D (diamond-pattern 5×5)       │ 1D (linear, 15 taps)                                │
    ├───────────┼────────────────────────────────┼─────────────────────────────────────────────────────┤
    │ Taps      │ 13 (diamond mask |x|+|y|<=2)   │ 15 (center + 7+ / 7−)                               │
    ├───────────┼────────────────────────────────┼─────────────────────────────────────────────────────┤
    │ Separable │ No (single pass)               │ Yes (called twice: once for X, once for Y)          │
    ├───────────┼────────────────────────────────┼─────────────────────────────────────────────────────┤
    │ Use case  │ Single-pass 2D blur            │ Two-pass separable blur (H then V) — more efficient │
    └───────────┴────────────────────────────────┴─────────────────────────────────────────────────────┘


    The 1D variant is more GPU-friendly: two 15-tap passes (H+V) cost 30 samples vs. the 2D version's 13 in a single
    pass, but the separable approach produces a much wider blur radius for the same cost — which is why CGlareEffect
    calls this function twice (visible in Init2 and sub_6A16B0, two calls each).

## Scene IDs

0: Logo Screen
1: Title Screen
2: Map
3: Room
4: Shop
6: Present
7: Clothes
9: Load Game / Save Game
18: Configure
20: Dialog
21: Free Mode
23: Modify Body
24: Modal - Return to Title Screen

##   ✦ Now I have a comprehensive picture. Let me also check sub_640DC0 more carefully. Looking at the code:

     1 v11[12768] = v21;  // v11 = &pSBZGlobal->ObjAt_3CC, offset 12768 = 0xC780 = field_CB4C
    So field_CB4C (offset 0xC780 within CGlobal_C78C) is being assigned `v21`, which is the return value of `LoadXX_70DDC0(...). This loads a "fade.xx" file. The file is at 
    data/base/fade.xx and data/base/fade.xa. The comment in the code says .\\!_CommonFiles\\GameFadeCtrl.cpp` - so this is a GameFadeCtrl object.
    For field_CB50:
     - In sub_641320 (cleanup): it calls sub_707480((void *)*p_field_CB50) which looks like a destructor/free call
     - In sub_6418D0: it calls *(float *)(ref_field_3CC->field_CB50 + 48) = pSBZGlobal_a1->ObjAt_3CC.field_4D4 and UpdateSpriteQuadVB_707570((CStructImage2D_80 *)ref_field_3CC->field_CB50) -        this is clearly a 2D sprite/image object used for rendering the fade overlay
     - In sub_641CA0: it writes to *(float *)(pSBZGlobal_a1->? + 48) and calls UpdateSpriteQuadVB_707570 - same pattern
    So:
     - field_CB4C = pointer to a loaded fade control object (loaded from data/base/fade.xx), specifically a GameFadeCtrl or similar fade effect resource. The file format is ".xx" (likely a  
       scene/effect file).
     - field_CB50 = pointer to a 2D sprite/quad object (CStructImage2D_80) used to render the fade overlay on screen
    Now let me also look at sub_6418D0 and sub_641400 which are the "StartFade" functions. They check if the respective field is non-zero before proceeding, meaning:
     - sub_641400 uses field_CB4C → it's the "start fade-in/fade-out" function using the .xx fade resource
     - sub_6418D0 uses field_CB50 → it's a different fade function using a sprite/image for the fade effect
    Wait, let me re-read the code. In sub_640DC0:
     1 v21 = LoadXX_70DDC0(v13, &Str_v6, 0, 0, &Str_v7, 0, 1);
     2 ...
     3 v11[12768] = v21;  // field_CB4C = loaded fade .xx object
    In sub_641320 (cleanup/destructor):
     1 if ( field_CB4C ) {
     2     sub_6E5010(&field_4E4, *p_field_CB4C);  // some operation on fade ctrl
     3     *p_field_CB4C = 0;
     4 }
     5 if ( field_CB50 ) {
     6     sub_707480((void *)*p_field_CB50);  // free/destruct the sprite       
     7     *p_field_CB50 = 0;
     8 }

    In sub_641400 (fade start function using field_CB4C):
     1 if ( !field_CB4C ) return 0;
     2 ...
     3 sub_6E1ED0((void *)field_CB4C, ...);  // set fade params   
     4 sub_6E42C0(field_CB4C, ...);          // set fade params   

    In sub_6418D0 (fade start function using field_CB50):
     1 if ( !field_CB50 ) return 0;
     2 ...
     3 *(float *)(field_CB50 + 48) = ...;     // set alpha/opacity
     4 UpdateSpriteQuadVB_707570((CStructImage2D_80 *)field_CB50);  // update vertex buffer

    And back in PushScene_643800, PushSceneWState_6431F0, and PopSceneMaybe_643520:
     1 if ( a2 && (field_CB4C || field_CB50) )
    The parameter a2 seems to be a "bUseFadeOverlay" flag. When the fade system is active (either field is non-zero), the scene push/pop uses the secondary "ArrObj2C_3D8" array (which       
    stores deferred scene transitions), and the fade effect runs in parallel. When no fade system is initialized, the scene push/pop happens directly on the main scene stack.
    So the naming should be:
     - field_CB4C → pFadeCtrlXX or pFadeEffectXX — pointer to the XX-based fade controller object
     - field_CB50 → pFadeSprite or pFadeOverlaySprite — pointer to the CStructImage2D_80 sprite used as the fade overlay
    Wait, let me reconsider. Looking at the source file path: .\\!_CommonFiles\\GameFadeCtrl.cpp. This confirms the whole thing is a GameFadeCtrl system.
    Let me think about what each field really is:
     - field_CB4C is loaded from `data/base/fade.xx` via `LoadXX_70DDC0. It's then operated on by sub_6E1ED0` and `sub_6E42C0, and destroyed by sub_6E5010`. This is the main fade controller 
       object (loaded .xx scene/resource).
     - field_CB50 is a `CStructImage2D_80*` that has its vertex buffer updated via `UpdateSpriteQuadVB_707570` and its alpha (offset +48) set. It's freed by `sub_707480`. This is a *2D      
       sprite/quad* used for rendering the fade overlay.
    Actually, looking more closely at sub_6418D0, it seems like a different type of fade. sub_641400 uses field_CB4C and calls `sub_6E1ED0/sub_6E42C0` which seem to operate on the XX        
    animation. sub_6418D0 uses field_CB50 with a sprite and alpha interpolation (fields 4C4-4DC).
    Given the naming conventions in the project (this is a Japanese Illusion game), these are likely:
     - field_CB4C: the fade animation/effect controller (from .xx file = an animation or scene object)
     - field_CB50: a simple sprite-based fade overlay (a fullscreen quad with alpha)
    Let me think about better names. Looking at the overall architecture:
     - sub_640DC0 = Init/Load fade system (loads fade.xx and fade.xa)
     - sub_641320 = Cleanup/Destroy fade system
     - sub_641400 = Start fade animation (using .xx fade controller)
     - sub_6418D0 = Start color/sprite fade (using sprite overlay)
     - sub_641CA0 = Update fade (per-frame interpolation)
    The whole thing is a "Fade" system. The two fields are:
     - field_CB4C: A pointer to the loaded "fade.xx" animation/effect resource. This is likely a pointer to a scene object or animation controller loaded from the .xx file. Better name:     
       pFadeXX or pFadeCtrl
     - field_CB50: A pointer to a CStructImage2D_80 sprite used as the fade overlay quad. Better name: pFadeSprite or pFadeOverlay
    Actually, wait. Let me look at how field_CB50 is created. In sub_640DC0, only field_CB4C gets set (to v21). Where does field_CB50 get created? Let me check the other init functions or   
    look for the creation of field_CB50.
    The find_bytes for 0xC784 (field_CB50) showed these addresses: 0x44ed1c, 0x5c70f1, 0x6412d1, 0x641389, 0x641905, 0x641a85, 0x641a97, 0x641ab3, 0x641d72, 0x641d84, 0x641da0, 0x641e12,    
    0x641e22, 0x7d120b.

    0x6412d1 is in CSBZGlobal::sub_640DC0 (0x640DC0, size 0x55b). Let me check if that's where field_CB50 is initialized. Let me re-read the decompilation more carefully.
    In sub_640DC0, after the LoadXX, we have:
     1 v11[12768] = v21;  // 0x64119e - this is field_CB4C = v21 (loaded .xx)
    At 0x6412d1, we have another reference to 0xC784. But the decompilation doesn't show it clearly. It might be in a different code path or the decompiler simplified it. Let me look at     
    the address 0x6412d1 more carefully - it's within sub_640DC0.
    Actually, looking at the code more carefully, I see sub_6FA2A0((_DWORD *)v13, 0, 1, 0) is called right after setting v11[127
  ✦ Now I have a very clear picture. Let me analyze what these fields are:
    field_CB4C (offset 0xC780 in CGlobal_C78C):
     1. Written in CSBZGlobal::sub_640DC0 (0x64119e): v11[12768] = v21; where v11 is &pSBZGlobal->ObjAt_3CC and v21 is the result of LoadXX_70DDC0(...) which loads data/base.pp with         
        data/base/fade.xx. The address v11 + 12768*4 = v11 + 0xC780 = field_CB4C. So field_CB4C stores a pointer to a loaded fade control object (loaded from "fade.xx" data file).
     2. Used in sub_641400 (0x64143a-0x6414e6): Checks if field_CB4C exists, then calls `sub_6E1ED0(field_CB4C, ...)` and `sub_6E42C0(field_CB4C, ...)` - these seem to be operations on the  
        fade control object (setting up parameters).
     3. Cleaned up in sub_641320 (0x641362-0x64137f): If field_CB4C is set, calls `sub_6E5010(&field_4E4, *field_CB4C)` then sets it to 0.

     4. Checked in PushScene_643800, PushSceneWState_6431F0, PopSceneMaybe_643520: Used to determine if push/pop should go through the secondary scene buffer (ArrObj2C_3D8) instead of the   
        primary scene stack.
    The source file path in sub_640DC0 is `.\\!_CommonFiles\\GameFadeCtrl.cpp` — this is a Game Fade Controller object!
    field_CB50 (offset 0xC784 in CGlobal_C78C):
     1. Used in sub_6418D0 (0x641903, 0x641a83, 0x641ab8): If field_CB50 is set, it accesses `*(float*)(field_CB50 + 48) to set a float value, and calls 
        UpdateSpriteQuadVB_707570((CStructImage2D_80*)field_CB50)`. This suggests field_CB50 is a *2D sprite/image quad (type `CStructImage2D_80`) used for fade rendering — essentially a fade
         overlay sprite*.

     2. Cleaned up in sub_641320 (0x64138d-0x6413a9): If field_CB50 is set, calls `sub_707480((void*)field_CB50)` (likely a destructor/release) then sets to 0.
     3. Checked in sub_641CA0 (0x641d72, 0x641d84, 0x641da0): Similar pattern - *(float*)(field_CB50 + 48) for setting alpha/transparency, and UpdateSpriteQuadVB_707570 for updating the     
        sprite quad vertex buffer.
     4. Also checked in the same push/pop functions as field_CB4C.
    So:
     - field_CB4C = pointer to the GameFadeCtrl object (the fade controller, loaded from data/base.pp / fade.xx). It's a fade animation controller.
     - field_CB50 = pointer to a fade overlay sprite/quad (type CStructImage2D_80), a 2D image used to render the fade effect on screen.

    Both are related to scene fade transitions. When either is active, push/pop scene operations are routed through the secondary scene buffer (ArrObj2C_3D8) to handle scene transitions     
    with fade effects.
    PushScene_643800 function analysis:
    The function pushes a scene onto the scene stack. It has two modes:
     1. Fade mode (a2 && (field_CB4C || field_CB50)): Uses the secondary scene transition buffer (ArrObj2C_3D8, max 5 entries). Records the scene with state 3 (push) and increments 
        nIndexSceneOf4_3D5.
     2. Normal mode: Cleans up the current scene stack (sub_643AA0), resets it (CSBZGlobal::sub_643A20), then pushes via PushSceneWState_6431F0 with state 0, and marks the frame as dirty.   
    Better names:
     - field_CB4C → pFadeCtrl or pGameFadeCtrl — pointer to the GameFadeCtrl object (fade animation controller loaded from fade.xx)
     - field_CB50 → pFadeSprite or pFadeQuad — pointer to the fade overlay sprite quad (CStructImage2D_80 used to render the fade effect)

    Let me also verify by looking at sub_6E5010 (the cleanup for field_CB4C) and sub_707480 (cleanup for field_CB50) to confirm.
    Actually, the source path .\\!_CommonFiles\\GameFadeCtrl.cpp is the dead giveaway. And the UpdateSpriteQuadVB_707570 call on field_CB50 confirms it's a 2D sprite/quad for rendering.     
    Let me propose names now. I have enough evidence.

  ✦ Now I have a complete picture. Let me verify the source file reference for the fade controller.
  ╭────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
  │ ✓  find_regex (ida-pro-mcp MCP Server) {"pattern":"GameFadeCtrl","limit":10}                                                                                                           │
  │    {                                                                                                                                                                                   │
  │      "n": 3,                                                                                                                                                                           │
  │      "matches": [                                                                                                                                                                      │
  │        {                                                                                                                                                                               │
  │          "addr": "0x80bb88",                                                                                                                                                           │
  │          "string": ".\\!_CommonFiles\\GameFadeCtrl.cpp"                                                                                                                                │
  │        },                                                                                                                                                                              │
  │        {                                                                                                                                                                               │
  │          "addr": "0x80bbc8",                                                                                                                                                           │
  │          "string": ".\\!_CommonFiles\\GameFadeCtrl.cpp"                                                                                                                                │
  │        },                                                                                                                                                                              │
  │        {                                                                                                                                                                               │
  │          "addr": "0x80bc0c",                                                                                                                                                           │
  │          "string": ".\\!_CommonFiles\\GameFadeCtrl.cpp"                                                                                                                                │
  │        }                                                                                                                                                                               │
  │      ],                                                                                                                                                                                │  
  │      "cursor": {                                                                                                                                                                       │  
  │        "done": true                                                                                                                                                                    │  
  │      }                                                                                                                                                                                 │  
  │    }                                                                                                                                                                                   │  
  ╰────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────╯  
  ✦ All confirmed. The source file is GameFadeCtrl.cpp. Now I can provide a comprehensive answer.  

    Let me summarize:
    PushScene_643800 — pushes a scene onto the game's scene stack, with two code paths:
     - Fade transition mode: when a2 is true and either field_CB4C (fade controller) or field_CB50 (fade sprite) is set, it uses the secondary scene transition buffer (ArrObj2C_3D8, max 5   
       entries). It records the scene callbacks with field_0 = 3 (a "push" marker) and advances the index.
     - Normal mode: pops all current scenes (sub_643AA0), resets the scene stack (sub_643A20), then pushes the new scene via PushSceneWState_6431F0 with state 0, and marks the frame as      
       dirty.
    field_CB4C → pGameFadeCtrl: A pointer to the game fade controller object. Created in sub_640DC0 by loading data/base.pp + data/base/fade.xx (source: GameFadeCtrl.cpp). Used to control   
    fade-in/fade-out animations during scene transitions.
    field_CB50 → pFadeSpriteQuad: A pointer to a CStructImage2D_80 sprite quad used to render the fade overlay. Updated via UpdateSpriteQuadVB_707570, with an alpha/opacity value stored at  
     offset +48 within the object. When present, it causes scene push/pop operations to route through the secondary transition buffer.
    These two fields together form the fade system: pGameFadeCtrl manages the fade animation state machine, while pFadeSpriteQuad is the actual on-screen overlay sprite that gets rendered   
    during transitions. When either is active, scene transitions are buffered to allow the fade animation to play out.

  ✦ I now have a complete picture. Here's the analysis:

    ---

    PushScene_643800 — Scene Push with Fade-Aware Routing

    This function pushes a new scene onto the game's scene management system. It has two code paths depending on whether the fade system is active:


    ┌──────────────┬─────────────────────────┬───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐        
    │ Path         │ Condition               │ Behavior                                                                                                                              │        
    ├──────────────┼─────────────────────────┼───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤        
    │ Fade         │ a2 && (field_CB4C ||    │ Records the scene into the secondary transition buffer (ArrObj2C_3D8, max 5 slots) with field_0 = 3 (push marker), advancing          │        
    │ transition   │ field_CB50)             │ nIndexSceneOf4_3D5. This defers the actual push so the fade animation can play first.                                                 │        
    ├──────────────┼─────────────────────────┼───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤        
    │ Normal push  │ otherwise               │ Tears down the entire current stack (sub_643AA0 → pop all, sub_643A20 → zero scenes), then calls PushSceneWState_6431F0 with state 0, │        
    │              │                         │  and marks the frame dirty.                                                                                                           │        
    └──────────────┴─────────────────────────┴───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘        


    The same fade-check pattern (a2 && (field_CB4C || field_CB50)) appears identically in PushSceneWState_6431F0 and PopSceneMaybe_643520.

    ---

    Field Analysis

    field_CB4C → `pGameFadeCtrl`

    Evidence:
     - Created in sub_640DC0 (v11[12768] = v21): loads data/base.pp + data/base/fade.xx via LoadXX_70DDC0. Source file string: .\\!_CommonFiles\\GameFadeCtrl.cpp.
     - Used in sub_641400: calls sub_6E1ED0(pFadeCtrl, ...) and sub_6E42C0(pFadeCtrl, ...) — setting fade parameters (direction, range).
     - Destroyed in sub_641320: sub_6E5010(&field_4E4, *pFadeCtrl) then zeroed.

    This is a pointer to the GameFadeCtrl object — the fade animation state machine loaded from the fade.xx asset.

    field_CB50 → `pFadeSpriteQuad`

    Evidence:
     - Used in sub_6418D0: *(float*)(pFadeSpriteQuad + 48) = field_4D4 (sets opacity/alpha at offset +0x30), then UpdateSpriteQuadVB_707570((CStructImage2D_80*)pFadeSpriteQuad).
     - Updated in sub_641CA0 (frame update): same +48 float write + UpdateSpriteQuadVB_707570 — interpolating the alpha each frame.
     - Destroyed in sub_641320: sub_707480(pFadeSpriteQuad) (release/free) then zeroed.

    This is a pointer to a `CStructImage2D_80` sprite quad — the full-screen overlay sprite rendered during fade transitions. Offset +48 (0x30) holds the alpha/opacity value.

    ---

    Summary


    ┌──────────────┬───────────────────┬────────────────────┬─────────────────────────────────────────────────┐
    │ Current Name │ Proposed Name     │ Type               │ Purpose                                         │
    ├──────────────┼───────────────────┼────────────────────┼─────────────────────────────────────────────────┤
    │ field_CB4C   │ `pGameFadeCtrl`   │ void*              │ Fade animation controller (loaded from fade.xx) │
    ├──────────────┼───────────────────┼────────────────────┼─────────────────────────────────────────────────┤
    │ field_CB50   │ `pFadeSpriteQuad` │ CStructImage2D_80* │ Full-screen fade overlay sprite quad            │
    └──────────────┴───────────────────┴────────────────────┴─────────────────────────────────────────────────┘


    Together they implement the fade transition system: pGameFadeCtrl drives the fade state machine, pFadeSpriteQuad is the rendered overlay. When either is active, PushScene/PopScene       
    defer to the secondary transition buffer (ArrObj2C_3D8) so the fade can animate before the scene change takes effect.

    Want me to apply these renames in IDA?

.....

  Analysis: sub_6FAFA0

  Signature (from stack frame)

  void __cdecl UpdateWorldMatricesRecursive(
      CSceneObjectTemp*    pScnObj,        // [esp+0x14] - scene object
      CScnSubObject_21C*   pSubObjRoot,    // [esp+0x18] - root sub-object
      D3DMATRIX            parentMatrix    // [esp+0x1C] - parent world matrix (by value!)
  );

  Stack Frame

  ┌────────┬─────────────┬────────────────────┬────────────────────────────────────────────┐
  │ Offset │    Name     │        Type        │                Description                 │
  ├────────┼─────────────┼────────────────────┼────────────────────────────────────────────┤
  │ -0x0C  │ var_4       │ int                │ pSubObjRoot + 0x54 (matrix pointer)        │
  ├────────┼─────────────┼────────────────────┼────────────────────────────────────────────┤
  │ -0x08  │ var_8       │ int                │ Loop counter (child index)                 │
  ├────────┼─────────────┼────────────────────┼────────────────────────────────────────────┤
  │ -0x04  │ pSubObj21C  │ CScnFrame_21C*     │ Current child being processed              │
  ├────────┼─────────────┼────────────────────┼────────────────────────────────────────────┤
  │ +0x14  │ pScnObj     │ CSceneObjectTemp*  │ Scene object context                       │
  ├────────┼─────────────┼────────────────────┼────────────────────────────────────────────┤
  │ +0x18  │ sub_21C_218 │ CScnFrame_21C*     │ Root sub-object (has child count at +8)    │
  ├────────┼─────────────┼────────────────────┼────────────────────────────────────────────┤
  │ +0x1C  │ mat4x4      │ D3DMATRIX          │ Parent matrix (64 bytes, passed by value!) │
  └────────┴─────────────┴────────────────────┴────────────────────────────────────────────┘

  Logic Flow
```cpp
  void UpdateWorldMatricesRecursive(CSceneObjectTemp* pScnObj,
                                    CScnSubObject_21C* pRoot,
                                    D3DMATRIX parentMatrix) {
      // 1. Get this node's local matrix pointer (at offset 0x54 in CScnSubObject_21C)
      D3DMATRIX* pLocalMatrix = &pRoot->localMatrix;  // pRoot + 0x54

      // 2. Compute world matrix: world = local * parent
      D3DMATRIX worldMatrix;
      D3DXMatrixMultiply(&worldMatrix, pLocalMatrix, &parentMatrix);

      // 3. Get first child (at offset 0x0C in CScnSubObject_21C)
      CScnFrame_21C* pChild = pRoot->pFirstChild;  // pRoot + 0x0C

      // 4. Iterate children (count at offset 0x08)
      for (int i = 0; i < pRoot->childCount; ++i) {    // pRoot + 0x08
          // 5. RECURSIVE CALL with computed world matrix as new parent
          UpdateWorldMatricesRecursive(pScnObj, pChild, worldMatrix);

          // 6. Advance to next sibling (stride = 0x21C = sizeof(CScnSubObject_21C))
          pChild = (CScnFrame_21C*)((BYTE*)pChild + 0x21C);
      }
  }
```