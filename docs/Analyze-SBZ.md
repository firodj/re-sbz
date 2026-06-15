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