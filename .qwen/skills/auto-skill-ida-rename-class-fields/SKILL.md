---
name: ida-rename-class-fields
description: Systematic approach to analyze unnamed functions, variables, fields, GUIDs/IIDs, and COM interface pointers in IDA Pro structs or classes, determine their purpose, and propose meaningful names by tracing usage across callees, callers, constructors, and vtable layouts. Covers DirectSound, DirectInput, D3D9 render states, WAV/RIFF MMIO file parsing, Gaussian blur shader kernels, and frame timing patterns.
source: auto-skill
extracted_at: '2026-06-14T20:34:07.000Z'
---

# IDA Pro: Analyze & Name Unnamed Functions, Variables, and Fields

## When to use
When the user asks to analyze a function and propose better names for the function, its local variables, and unnamed struct/class fields (`field_XX`, `dword_XX`, `float_XX`, etc.) in a binary reverse-engineered in IDA Pro. Works for both C++ class methods (`this->field_XX`) and standalone functions taking struct pointers (`CStruct_80 *a1`). Also covers identifying unknown GUIDs/IIDs (`stru_XXXXXX` data symbols) and identifying COM interface pointer fields by tracing their vtable method calls.

## Procedure

### 1. Decompile the target function
Use `mcp__ida-pro-mcp__decompile` on the target address. Read the pseudocode carefully to understand the high-level purpose.

### 2. Check xrefs and callees (in parallel with step 1)
Call these in the same tool block as the decompile to save round-trips:
- `mcp__ida-pro-mcp__xrefs_to` → find callers and understand the public API context (also reveals how many call sites — a hint at generality)
- `mcp__ida-pro-mcp__callees` → see what it calls (D3D APIs, memory ops, helper functions)

### 3. Dump struct layouts with `ida_typeinf` (NOT `ida_struct`)
**Critical:** the `ida_struct` module is NOT available in this IDA MCP environment. Use `ida_typeinf` via `mcp__ida-pro-mcp__py_eval` instead:

```python
import ida_typeinf
results = {}
for name in ["CStruct_80", "CCustomVertex"]:
    tif = ida_typeinf.tinfo_t()
    if not tif.get_named_type(None, name):
        results[name] = "not found"
        continue
    members = []
    udt = ida_typeinf.udt_type_data_t()
    if tif.get_udt_details(udt):
        for i in range(udt.size()):
            m = udt[i]
            members.append({
                "name": m.name, "offset": m.offset // 8,
                "size": m.size // 8, "type": str(m.type)
            })
    results[name] = {"size": tif.get_size(), "members": members}
results
```

First find struct names with `mcp__ida-pro-mcp__search_structs` (substring filter). The dump reveals offset/size/type for every member, including unnamed ones.

### 4. Decompile callees to map field-to-parameter meaning
**Key technique:** when the target function passes struct fields as arguments to helper functions, decompile those helpers to learn what each field means. Example: `SetFogRange_6F32D0(start, end)` calls `SetRenderState(D3DRS_FOGSTART, start)` — this tells you the struct fields at the source offsets are `fogStart` and `fogEnd`. Also look at D3D/Win32 API constants (`D3DRS_*`, `D3DFOG_*`) for direct semantic clues.

### 5. Decompile callers to see how fields are assigned
Look at one or more callers to see what values get stored into the struct before calling the target. Example: a caller computing `centerX = width * 0.5` and assigning `field_14 = centerX` reveals `field_14` is the sprite's X position/pivot.

### 6. Decompile the constructor/initializer
Find and decompile the constructor or init function (often named `InitVertex_*`, `ctor`, or creating via `MemAllocate` + `memset`). This reveals:
- **Default values** — e.g., `scaleX = 1.0`, `cA = 1.0`, `tuLeft = 0.0`, `tuRight = 1.0` — which immediately distinguish "scale" from "position" and "U start" from "U end"
- **Field grouping** — `memset` ranges → adjacent fields form a sub-struct
- **Resource creation** — `CreateVertexBuffer` calls reveal buffer pointers and sizes (the size tells you the vertex count, e.g. 128 bytes ÷ 32 bytes/vertex = 4 vertices)

### 7. Decompile the caller/sibling functions (for class methods)
- **The caller** — often a public virtual method that sets up fields before delegating
- **The counterpart method** — e.g., if target is "open from memory", find "open from file" to compare
- **The vtable** — read vtable entries with `mcp__ida-pro-mcp__get_int`, then `lookup_funcs` to identify each slot

### 8. Trace field usage — `xrefs_to_field` then `find_bytes` fallback
Use `mcp__ida-pro-mcp__xrefs_to_field` to find all references to a specific struct field.

**Common failure modes and fixes:**

1. **Wrong struct name** — `xrefs_to_field` requires the *actual* struct type the field is declared in, not the parent. For nested structs (e.g., `CSBZGlobal.ObjAt_3CC.field_CB4C` where `ObjAt_3CC` is of type `CGlobal_C78C`), you must query `{struct: "CGlobal_C78C", field: "field_CB4C"}`, NOT the parent `CSBZGlobal`. Find the real containing struct via `search_structs` + `py_eval` UDT dump (step 3), checking member names and types around the target offset.

2. **No xrefs returned** — when fields are accessed via register+offset (decompiled as `p->field_XX` but compiled as `[reg+disp]`), `xrefs_to_field` returns empty. **Do NOT scan instructions via `py_eval`** — iterating `idc.next_head` over the whole `.text` segment **times out** (15s MCP limit). Instead, use `find_bytes` with the little-endian displacement pattern.

3. **`find_bytes` displacement search** — search for the 4-byte little-endian displacement relative to the **sub-object pointer**, not the absolute parent offset. For a field at offset `0xC780` within sub-struct `CGlobal_C78C`, search `"80 C7 00 00"`:
   ```
   mcp__ida-pro-mcp__find_bytes(patterns=["80 C7 00 00", "84 C7 00 00"])
   ```
   Then map matches to functions via `lookup_funcs`, and decompile the most relevant ones. Note: this finds *potential* matches (the 4-byte pattern can occur in other contexts), so verify by decompiling.

4. **Absolute vs sub-object offset** — for a field at offset `0xC780` in `CGlobal_C78C` (which starts at `0x3CC` in `CSBZGlobal`): absolute offset from parent = `0x3CC + 0xC780 = 0xCB4C`. The decompiler shows `field_CB4C` (absolute), but the **byte search** uses the sub-object displacement `0xC780`.

### 8b. Cross-reference source file path strings
The game's error/assertion strings often contain build-path source filenames (e.g., `.\\!_CommonFiles\\GameFadeCtrl.cpp`). Search for these with `find_regex(pattern=".*\\.cpp")` or a keyword from the observed string. The filename directly reveals the class/component name — e.g., `GameFadeCtrl.cpp` → the field is a `pGameFadeCtrl` pointer. This is one of the strongest naming signals available when present.

### 9. Build a field mapping table
For each unnamed field, determine:
| Offset | Type | Expression / Usage | Proposed Name | Reasoning |

- Use Windows/D3D naming conventions when applicable (`b` = BOOL/byte flag, `p` = pointer, `f` = float, `c` = color channel like `cDR` = diffuse red)
- Infer array sizes from struct member sizes (e.g., 5120-byte member ÷ 4 bytes/ptr = 1280 entries)
- Distinguish flags from enums by checking how many distinct values are compared against

### 10. Present findings with context
- Explain the **function's purpose** in one sentence (and note call-site count — broad usage = core/shared function)
- Provide the **function name** proposal
- Provide **local variable** proposals with reasoning (trace each variable's computation)
- Provide the **field table** with rationale, grouped by struct
- Offer to apply the renames in IDA

## COM interface identification

### Identifying GUIDs/IIDs in data
When asked about a `stru_XXXXXX` data symbol (16 bytes), it's likely a GUID/IID:

1. Read 16 bytes with `mcp__ida-pro-mcp__get_bytes`
2. Parse as a Windows GUID (mixed-endian: first DWORD + two WORDs little-endian, last 8 bytes big-endian):

```python
import struct
raw = bytes([0x84, 0xfa, 0x9a, 0x27, 0x81, 0x49, 0xce, 0x11,
             0xa5, 0x21, 0x00, 0x20, 0xaf, 0x0b, 0xe5, 0x60])
d1 = struct.unpack_from('<I', raw, 0)[0]
d2 = struct.unpack_from('<H', raw, 4)[0]
d3 = struct.unpack_from('<H', raw, 6)[0]
d4 = raw[8:16]
guid_str = "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}" % (
    d1, d2, d3, d4[0], d4[1], d4[2], d4[3], d4[4], d4[5], d4[6], d4[7])
```

3. Identify the IID from the GUID value (known COM IIDs like `IID_IDirectSound3DBuffer`, `IID_IDirectSound3DListener`, etc.)
4. Confirm by decompiling the referencing function — look for `QueryInterface(&stru_XXXXXX, &out)` patterns

### Identifying COM interface pointers in structs
When a struct field (e.g., `com_dsound_28`, typed as `int`) is a COM interface pointer:

1. Find where it's assigned — decompile the init/creation function and look for:
   - `CreateSoundBuffer(&dsbd, &this->field_28, NULL)` → field is `IDirectSoundBuffer*`
   - `QueryInterface(this->field_X, &IID_Foo, &this->field_28)` → field is `IFoo*`
2. Confirm by checking vtable offsets used in calls — match against known COM interface vtable layouts:

| Interface | Vtable offset 36 | Vtable offset 44 | Vtable offset 76 |
|-----------|-----------------|-----------------|-----------------|
| `IDirectSoundBuffer8` | `GetStatus` (index 9) | `Lock` (index 11) | `Unlock` (index 19) |

Full `IDirectSoundBuffer8` vtable (byte offset → method name), confirmed from multiple functions:

| Offset | Index | Method | Confirmed in |
|--------|-------|--------|-------------|
| 12 | 3 | `GetCaps` | VU meter sampler |
| 16 | 4 | `GetCurrentPosition` | loop controller |
| 20 | 5 | `GetFormat` | format query |
| 24 | 6 | `GetVolume` | volume getter |
| 36 | 9 | `GetStatus` | IsPlaying / IsLooping |
| 44 | 11 | `Lock` | audio data write |
| 48 | 12 | `Play` | Play wrapper |
| 52 | 13 | `SetCurrentPosition` | seek/positioning |
| 60 | 15 | `SetVolume` | volume setter |
| 72 | 18 | `Stop` | stop playback |
| 76 | 19 | `Unlock` | audio data write |

| Interface | Vtable offset 12 | Struct size passed |
|-----------|-----------------|-------------------|
| `IDirectSound3DBuffer` | `GetAllParameters` (index 3) | 64 (`DS3DBUFFER`) |
| `IDirectSound3DListener` | `GetAllParameters` (index 3) | 76 (`DS3DLISTENER`) |

3. Status/flags bit constants: `DSBSTATUS_PLAYING = 0x1`, `DSBSTATUS_BUFFERLOST = 0x2`, `DSBSTATUS_LOOPING = 0x4`

**IMPORTANT:** The above are the correct DirectSound SDK values. A function testing bit `0x1` from `GetStatus` → `IsPlaying`. A function testing bit `0x4` → `IsLooping`. Do not confuse these — they are easy to swap.

### COM field naming conventions
- `pDSBuffer` — `IDirectSoundBuffer8*`
- `pDS8` — `IDirectSound8*`
- `pDS3DListener` — `IDirectSound3DListener*`
- `pDS3DBuffer` — `IDirectSound3DBuffer*`
- Vtable inspection via `py_eval`: dump all vtable entries, match offsets to method indices, confirm interface identity

## Common patterns in DirectSound / audio classes
- `pDSBuffer` / `com_dsound_28` — `IDirectSoundBuffer8*`, created via `CreateSoundBuffer`
- `dwBufferFlags` (offset 0x2C) — stores `DSBUFFERDESC.dwFlags`; tested for `DSBCAPS_CTRLVOLUME` (0x80), `DSBCAPS_CTRL3D` (0x10), `DSBCAPS_GLOBALFOCUS` (0x8000). **Note:** ensure the field name suffix matches the actual hex offset (e.g., a field at offset 0x2C should be named `..._2C`, not `..._28`)
- `lVolume` — `LONG` in centibels, clamped to [DSBVOLUME_MIN=-10000, DSVOLUME_MAX=0]; stored then passed to `SetVolume`
- `fCurrentLevel` — smoothed amplitude float, updated via low-pass filter (coefficient ~0.35), clamped to [0, 9.0]
- `nMaxLoops` / `nLoopCount` — finite loop counter pair; controller polls `GetCurrentPosition` == 0, increments count, restarts via `Play` until count reaches max
- `dwPlayFlags` — stored play flags passed to `IDirectSoundBuffer::Play` (OR'd with `DSBPLAY_LOOPING` = 0x1 if looping)
- `dwDurationMs` — computed duration in milliseconds (dataBytes / bytesPerSec * 1000)
- `cbMemSize` / `cbDataSize` — total allocation size vs actual audio data size

### VU meter / amplitude level sampler pattern
A function that locks a window of 16-bit samples ahead of the play cursor, sums absolute values, and divides for average level:
- `GetCaps` to get buffer size → ensure cursor isn't near end
- `GetCurrentPosition` → align to `& ~1` (even boundary for 16-bit)
- `Lock` ~442 bytes (= 221 samples) ahead, handle circular wrap (Part1 + Part2)
- Sum absolute values of 220 `__int16` samples
- Divide sum by 110 → average per-sample amplitude
- Caller normalizes (`/ 512 / 2.0`), applies low-pass smoothing, clamps

### Finite loop controller pattern (manual loop counting)
Instead of using DirectSound's built-in `DSBPLAY_LOOPING`, some implementations poll the play cursor during update ticks:
1. Check if `nLoopCount >= nMaxLoops` → return 0 (finished)
2. Check `IsPlaying()` → return 1 (still going)
3. `GetCurrentPosition` → if cursor == 0 (natural end reached), increment `nLoopCount`
4. If still under max, call `Play(bLoop=0, dwPlayFlags)` to restart
5. Return 1 (looped)

### Double-SetVolume refresh trick
When resuming playback, some wrappers call `SetVolume(lVolume - 1)` then `SetVolume(lVolume)` immediately — this forces DirectSound to re-apply the stored volume, which it doesn't always restore automatically after a stop/resume cycle.

## Common patterns in frame timing / FPS tracking
A global timing function called once per frame (often 70+ call sites across all scene update functions):

| Global role | Typical name | Evidence |
|-------------|-------------|----------|
| Current tick time | `g_dwCurrentTime` | Set from `GetTimeComulative()` each frame |
| Previous tick time | `g_dwPrevTime` | Used to compute delta: `current - prev` |
| Frame delta (ms) | `g_dwFrameDeltaMs` | `current - prev`, output to callers |
| FPS accumulator (ms) | `g_dwFpsAccumMs` | Accumulates deltas, resets at >= 1000 |
| FPS frame counter | `g_dwFpsFrameCount` | Increments each call, resets with accumulator |
| Computed FPS (float) | `g_fCurrentFPS` | `frameCount / (accumMs * 0.001)`, recomputed every ~1s |

The function typically takes `(pOutFPS, pOutDeltaMs, dwCurrentTime)` where outputs are optional (NULL-safe). Callers pass `&struct.field_FPS` and `&struct.field_DeltaMs`.

## Common patterns in input systems (DirectInput)
A master input poll called once per frame from the main update loop. Reads keyboard, mouse, and gamepad via DirectInput8, then computes edge-detected button transitions.

### Input state encoding (value written to transition arrays)
| Value | Meaning | Computation |
|-------|---------|-------------|
| 0 | `IDLE` | Not pressed |
| 1 | `HOLD` | Pressed and was pressed last frame |
| 2 | `RELEASE` | Was down last frame, now up |
| 3 | `TRIGGER` | Was up last frame, now down (`current & ~prev`) |

### DirectInput polling pattern (keyboard/mouse/gamepad)
```c
// Poll → if fails, Unacquire → retry Acquire loop (while == DIERR_INPUTLOST 0x8007001E) → GetDeviceState
device->lpVtbl->Poll(device);
if (hr < 0) {
    device->lpVtbl->Unacquire(device);
    while ((hr = device->lpVtbl->Acquire(device)) == 0x8007001E) ;
}
hr = device->lpVtbl->GetDeviceState(device, size, &state);
```

### Input global variable layout
Each input source has a **quad of arrays**: `prev[]`, `current[]`, `transition[]`, `consumed[]`:
- **Keyboard** (256 bytes each): `g_keyStatePrev`, `g_keyState`, `g_keyTransition`, `g_keyConsumed`
- **Mouse buttons** (8 bytes each): `g_mouseBtnPrev`, `g_mouseBtnState`, `g_mouseBtnTransition`, `g_mouseBtnConsumed`
- **Gamepad** (36 entries/device): `prev[36] + current[36] + transition[36]` packed into 108-byte/device struct

The `consumed[]` arrays implement "fire once until release" semantics: `IsHeld`/`IsTriggered` helpers return false if the button was consumed, and consumption clears when the button is released.

### Gamepad raw state (DIJOYSTATE, 44 bytes/device)
Offset layout: dpad[4] (left/right/up/down booleans) + buttons[32] (0x80 bit test) + lX (int) + lY (int).

### Helper function pairs for edge detection
Each input type has a pair: `IsHeld(button)` and `IsJustReleased(button)`:
- `IsHeld`: returns `current != 0 && !consumed` (repeated-fire suppression)
- `IsJustReleased`: returns `(~current & prev) != 0 && !consumed` (falling edge)

## Applying renames in IDA
Use `mcp__ida-pro-mcp__rename` with a batch object to apply all renames at once. Supports four categories in one call:

```python
mcp__ida-pro-mcp__rename(batch={
    "func": {"addr": "0x696220", "name": "UpdateFrameTiming_696220"},
    "local": [
        {"func_addr": "0x696220", "old": "a1", "new": "pOutFPS"},
        {"func_addr": "0x696220", "old": "a2", "new": "pOutDeltaMs"}
    ],
    "data": [
        {"old": "dword_89D604", "new": "g_dwFrameDeltaMs"},
        {"old": "dword_89D610", "new": "g_dwPrevTime"}
    ],
    "stack": [{"func_addr": "0xXXXXXX", "old": "v3", "new": "settings"}]
})
```

- **func**: `{addr, name}` — function rename
- **local**: `{func_addr, old, new}` — local variable rename (within decompiler scope)
- **data**: `{old, new}` — global/data variable rename (old is the current symbol name)
- **stack**: `{func_addr, old, new}` — stack variable rename
- All categories can be passed as single object or array of objects
- Returns `ok: true/false` per item — verify after applying
- After renaming, re-run `decompile` to confirm the changes took effect

## Common patterns in WAV file parsing (Win32 MMIO API)
The game uses a `CWaveFile` class backed by the Windows Multimedia I/O (`mmio*`) API. RIFF four-character codes appear **byte-reversed** in decompilation because they're stored as `DWORD` literals:

| Literal in code | Actual ASCII | Meaning |
|----------------|-------------|---------|
| `'FFIR'` (0x52494646) | `RIFF` | Top-level RIFF container |
| `'EVAW'` (0x57415645) | `WAVE` | WAVE file type |
| `' tmf'` (0x666D7420) | `fmt ` | Format chunk |
| `'atad'` (0x64617461) | `data` | Audio data chunk |
| `' MEM'` (0x4D454D20) | `MEMM` | Memory-backed MMIO IOProc |

### Key MMIO functions and their roles
- `mmioOpenA(filename, &mmioinfo, dwOpenFlags)` — open file or memory buffer
  - `dwOpenFlags = 0x10000` → `MMIO_READWRITE` (read existing)
  - `dwOpenFlags = 0x11002` → `MMIO_READ | MMIO_DENYWRITE | MMIO_ALLOCBUF` (read-only file)
  - For memory-backed: pass `fccIOProc = ' MEM'`, `cchBuffer = size`, `pchBuffer = ptr` in `MMIOINFO`
- `mmioDescend(hmmio, &ck, &parent, flags)` — enter a chunk; `flags = 0x10` (`MMIO_FINDCHUNK`)
- `mmioAscend(hmmio, &ck, 0)` — leave a chunk (updates size header)
- `mmioCreateChunk(hmmio, &ck, flags)` — create new chunk; `flags = 0x20` (`MMIO_CREATERIFF`)
- `mmioRead`/`mmioWrite` — read/write raw bytes from chunk data
- `mmioGetInfo`/`mmioSetInfo` — get/set MMIO buffer state for manual buffered I/O
- `mmioAdvance` — refill internal buffer during byte-by-byte read loops

### WAV class field layout (CWaveFile)
- `field_8` (`dwMode`) — open mode enum: `1` = read existing, else = create/write
- `field_58` (`bReadFromMemory`) — BYTE flag: `true` → read from memory pointer, `false` → read via MMIO
- `field_5C`/`field_60`/`field_64` — `pbData`/`pbDataCur`/`cbDataSize` triplet for memory-backed reads
- `field_4C` (`cbMemSize`) — total memory buffer size backing MMIO
- `field_50` (`cbDataSize`) — audio data chunk size (`mmcki.cksize`)
- `field_54` (`dwDurationMs`) — duration = `cbDataSize / nAvgBytesPerSec * 1000.0`
- `field_48` (`pWavFormat`) — pointer to heap-allocated `WAVEFORMATEX`

### WAVEFORMATEX parsing pattern
Read 16 bytes of `fmt` chunk base header, then branch on `wFormatTag`:
- `wFormatTag == 1` (PCM): allocate 18 bytes, set `cbSize = 0` (no extra data)
- `wFormatTag != 1` (compressed): read 2-byte `cbSize`, allocate `18 + cbSize`, read `cbSize` extra bytes

### Dual read path pattern
Read functions typically have two branches gated by `bReadFromMemory`:
- **Memory path:** `memcpy` from `pbDataCur` with bounds clamp against `pbData + cbDataSize`
- **MMIO path:** `mmioGetInfo` → byte-by-byte loop with `mmioAdvance` on buffer exhaustion → `mmioSetInfo`

## Common patterns in Gaussian blur / post-processing shader kernels
The game's glare/bloom effect generates Gaussian blur tap tables at init time for GPU shaders. Two generator variants exist:

### 2D Gaussian kernel generator (`BuildGaussianKernel`)
- 5×5 sample grid with diamond mask (`|i| + |j| <= 2`) → 13 taps instead of 25
- Each tap stores: float4 UV offset (`i/width, j/height, 0, 0`) and float4 weight (RGBA replicated)
- Optional normalization: divide all weights by their sum
- Weight formula: `G(x,y) = exp(-(x²+y²)/(2σ²)) / (2πσ²)`

### 1D separable Gaussian kernel generator (`BuildGaussianKernel1D`)
- 15 taps: center (0) + positive (1-7) + mirrored negative (8-14)
- Called twice (once per axis) for separable H+V blur — more efficient than single 2D pass
- Same weight formula but with `y=0`
- Layout: `offsets[0]=0, offsets[1..7]=+i/texSize, offsets[8..14]=-offsets[1..7]`

### Identifying math library wrappers
Small leaf functions wrapping a single FPU instruction:
| Signature | Likely math function | ID instruction |
|-----------|---------------------|----------------|
| `double __cdecl f(float)` | `sqrtf` / `expf` / `logf` / `sinf` | `fsqrt` / `F2XM1` |
| Check `call` target with 1 float arg | Common: `expf`, `sqrtf`, `powf` | |

Confirm by decompiling — if body is `return exp(a1)` or `return sqrt(a1)`, name accordingly.

## D3DCOLOR packing tricks
D3D render-state color values use `D3DCOLOR` (0xAARRGGBB). A common decompiler pattern:

```c
color = (unsigned __int64)(255.0 * b)          // blue  → bits 0-7
      + ((unsigned int)(255.0 * r) << 16)      // red   → bits 16-23
      - 0x1000000                               // = +0xFF000000 (mod 2³²) → alpha = 255
      + ((unsigned int)(255.0 * g) << 8);       // green → bits 8-15
```

The `- 0x1000000` is an obfuscated way to set alpha to `0xFF` (opaque) via unsigned modular arithmetic. When you see this pattern with float-to-int conversions and `<<8`/`<<16` shifts, it's building a `D3DCOLOR` from normalized RGB floats.

### D3D render state setter/getter family pattern
Render state values are typically cached in a global `DWORD` alongside the `SetRenderState` call:
- Setter (from DWORD): `SetRenderState(state, value); g_dwCache = value;`
- Setter (from floats): compute D3DCOLOR, call above
- Getter: `return g_dwCache;` (optionally writes to out param)
These come in families — fog color, ambient color, material color, etc.

## Common patterns in game/graphics C++ classes
- `field_8` as a mode/open-type enum (0=read/write, 1=read existing)
- `field_4C`/`field_50` as sizes matching `cchBuffer`/`cksize`
- `field_58` as a BOOL for "use memory buffer vs MMIO"
- `field_5C`/`field_60`/`field_64` as pointer+cursor+size triplets for buffered I/O
- `field_54` as a computed duration (bytes / bytesPerSec * 1000)

## Common patterns in D3D sprite/vertex structs
- `bFlipV` / `bField0` — boolean that swaps tv top/bottom (vertical texture flip)
- `srcWidth`/`srcHeight` (`nX`/`nY`) — base dimensions × texture scale
- `posX`/`posY`/`posZ` — position written directly to vertex.x/y/z
- `originX`/`originY` — anchor factor (0–1) multiplied by dimensions to compute the top-left corner
- `scaleX`/`scaleY` — rendered size multiplier (default 1.0)
- `cA`/`cDR`/`cDG`/`cDB` — diffuse color channels (alpha + RGB), `cSR`/`cSG`/`cSB` — specular
- `tuLeft`/`tuRight`/`tvTop`/`tvBottom` — UV bounds (default 0.0/1.0/0.0/1.0)
- `texScale` — texture resolution multiplier used for half-texel correction
- `bVisible` — visibility flag set by UI hit-test code
- `scrollSpeedX`/`scrollSpeedY` + `scrollOffsetX`/`scrollOffsetY` — auto-scroll parameter pairs

## Common patterns in scene stack / fade transition management
The game uses a scene stack (`sceneStack_1A8` / `CSceneStack`) with up to 10 scenes, each having a `CSceneCallbacks` struct of function pointers (`pfnNew_08`, `pfnMainScene_0C`, `pfnChildScene_10`, `pfn3DRender_14`, `pfnMainUIRender_18`, `pfnChildUIRender_1C`, `pfnCleanUp_20`). The fade transition system gates whether scene push/pop operations are immediate or deferred.

### Fade-gated scene transition pattern
Three sibling functions (`PushScene`, `PushSceneWState`, `PopSceneMaybe`) share the same dual-path structure gated by `a2 && (pFadeCtrl || pFadeSprite)`:

| Condition | Path | Behavior |
|-----------|------|----------|
| Fade active (`a2 && (pFadeCtrl \|\| pFadeSprite)`) | **Deferred** | Record into secondary transition buffer `ArrObj2C_3D8[5]` with a marker (`field_0`: 1=push+state, 2=pop, 3=push), advancing `nIndexSceneOf4_3D5` |
| Fade inactive | **Immediate** | Operate on primary `sceneStack_1A8` directly: push/pop/cleanup scenes, invoke callback function pointers, update render flags |

### Fade controller fields (in `CGlobal_C78C` sub-struct of `CSBZGlobal`)
- `field_CB4C` → `pGameFadeCtrl` — pointer to fade animation controller, loaded from `data/base.pp` + `data/base/fade.xx` via `LoadXX_70DDC0`. Source string: `GameFadeCtrl.cpp`. Created in init, freed via `sub_6E5010` in cleanup.
- `field_CB50` → `pFadeSpriteQuad` — pointer to `CStructImage2D_80` full-screen overlay sprite. Alpha/opacity at offset +0x30 (48), updated per-frame via `UpdateSpriteQuadVB_707570`. Freed via `sub_707480`.
- `nIndexSceneOf4_3D5` — index into deferred transition buffer (max 5)
- `ArrObj2C_3D8` — `CStruct_2C[5]` deferred transition entries; `field_0` marks operation type (1=push+state, 2=pop, 3=push)

### Scene stack lifecycle helpers
- `sub_643AA0` — pop all scenes from stack (reset `bKilled`, loop `PopSceneMaybe`)
- `sub_643A20` — zero `sceneStack_1A8.scenes` and init `childIndices` to `-1`
- `sub_643940` — update scene ID history ring buffer (`arrSceneIDsStack[6]`)
- `sub_643B40` — recompute render flags (`bRender3D`, `bRenderUI`, `bAnaglyphPass`) from current scene's callbacks
- `Render_642BC0` — main frame: invoke PreFrame → child scenes → main scene → D3D Clear/BeginScene → 3D render → UI render → EndScene/Present

## Tools used
- `mcp__ida-pro-mcp__decompile` — primary analysis (target, callees, callers, constructor)
- `mcp__ida-pro-mcp__xrefs_to` — find callers/references
- `mcp__ida-pro-mcp__callees` — find what a function calls
- `mcp__ida-pro-mcp__search_structs` — find struct names by substring
- `mcp__ida-pro-mcp__py_eval` — dump struct layouts via `ida_typeinf` (NOT `ida_struct`)
- `mcp__ida-pro-mcp__xrefs_to_field` — find references to specific struct fields (fails for register+offset access — use `find_bytes` fallback)
- `mcp__ida-pro-mcp__find_bytes` — search byte patterns for displacement xrefs when `xrefs_to_field` returns empty (search sub-object offset, not absolute)
- `mcp__ida-pro-mcp__find_regex` — search strings; key for finding source-file path strings that reveal class names
- `mcp__ida-pro-mcp__lookup_funcs` — map addresses to function names (for `find_bytes` matches)
- `mcp__ida-pro-mcp__rename` — apply renames to functions, globals, locals, stack vars (batch)
- `mcp__ida-pro-mcp__get_int` — read vtable entries
- `mcp__ida-pro-mcp__get_bytes` — read raw bytes (16 for GUIDs/IIDs, 4 for vtable entries)
- `mcp__ida-pro-mcp__list_globals` — find vtables by class name
- `mcp__ida-pro-mcp__find` — search for immediate value usage (field offsets)
