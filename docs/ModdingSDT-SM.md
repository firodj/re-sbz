# School Mate (.sdt) Script Block Documentation

Source: https://wiki.anime-sharing.com/hgames/index.php?title=School_Mate/Modding/Sdt_codes

*Based on code analysis by Alamar.*

This document details the data types and structures contained within each script block (opcode) of `School Mate` `.sdt` files.

---

## Block 0: Comment

* **Description:** Contains developer comments.
* **Structure:**
* `Text` — Comment text



---

## Block 1: Dialogue

* **Description:** Handles on-screen dialogue, speaker identification, and audio playback.
* **Structure:**
* `Int32` — Incremented for each unique Block 1 instance.
* `Text` — Name of the speaker (Set to `設定１` for no name).
* `Text` — Audio file name to play (Empty string for no sound).
* `Int32` — Number of text lines.



> ### Repeated Section (Per Line of Text)
> 
> 
> * `Byte` — Flag (Always `0` in known `.sdt` files).
> * `Byte`, `Byte`, `Int32`, `Int32` — *Read only if the above flag byte is non-zero (Never observed in existing files).*
> * `Text` — Line of text.
> 
> 

---

## Block 2: Screen Color Blend (Full Screen)

* **Description:** Gradually blends the entire screen from/to a specified color over a set duration. The screen stays blended until another Block 2 restores it.
* **Structure:**
* `Byte` — Mode (`0` = Blend from color to scene; `1` = Blend full screen to color).
* `Int32` — Color value.
* `Int32` — Blend duration (in seconds).



---

## Block 3: Unused

* **Description:** Does not appear in known `.sdt` files.

---

## Block 4: Target/Environment Definition

* **Description:** Purpose fully unknown, but uses fixed/predictable pairs.
* **Structure:**
* `Int32` — Always `2`.
* `Text` — Value is always either `"NULL"` or `"siro.ema"`.
* `Int32` — Always `0`.
* `Int32` — Always `0`.
* `Int32`, `Int32` — Resolution/Coordinates. Always `[64, 48]` when text is `"NULL"`, and `[1024, 768]` when text is `"siro.ema"`.



---

## Block 5: Unused

* **Description:** Does not appear in known `.sdt` files.

---

## Block 6: Sound Load / Unload

* **Description:** Specifies the filename of an audio asset to load or unload. When loading a file (not `"NULL"`), it is consistently followed by Block 7 and Block 8.
* **Structure:**
* `Int32` — Unique ID assigned to the loaded sound.
* `Text` — Sound file name (Set to `"NULL"` to unload the asset).
* `Byte` — Unknown.
* `Byte` — Unknown.



---

## Block 7: Sound Processing

* **Description:** Purpose unknown. Often occurs sequentially between Block 6 and Block 8. In existing files, `count7a` is always `1` and `count7b` is always `1` or `2`.
* **Pseudocode Logic:**
```csharp
int count7a = ReadInt32(reader);
for (int i = 0; i < count7a; i++)
{
    byte count7b = ReadByte(reader);
    ReadInt32(reader);
    ReadInt32(reader);

    if ((count7b > 0) && (count7b <= 2))
    {
        ReadInt32(reader);
    }
}

```



---

## Block 8: Play Sound

* **Description:** Triggers playback for audio initialized in Block 6.
* **Structure:**
* `Int32` — Sound ID (Matches the ID specified in a preceding Block 6).



---

## Block 9: Script Link / Transfer

* **Description:** Transfers script execution to a different `.sdt` file.
* **Structure:**
* `Text` — Filename of the target `.sdt` file.
* `Int32` — Unknown (Typically `0`).



---

## Block 10: Jump Label

* **Description:** Marks a label location inside the file to serve as a destination for jumps (See Block 22).
* **Structure:**
* `Text` — Label name.



---

## Block 11: Branching Menu

* **Description:** Displays a multiple-choice selection menu to the player.
* **Structure:**
* `Int32` — Number of choices (Always `2` in known files).



> ### Repeated Section (Per Choice)
> 
> 
> * `Int32` — Number of commands to execute if this option is chosen.
> * `Text` — Display text for the menu option.
> * `Data` — Script commands nested inside this choice branch.
> 
> 

---

## Block 12: Scene Termination Marker

* **Description:** Purpose unknown. Always appears at the very end of a file and is sequentially followed by Block 13, Block 25, and Block 9.
* **Structure:**
* `Text` — File reference (Always `"sm00_00.pp"` in known files).
* `Int32` — Constant identifier (Always `4294967295`).



---

## Block 13: End-of-File Hook

* **Description:** Purpose unknown. Only observed immediately following Block 12.

---

## Block 14: Scene Transition Logic

* **Description:** Controls environmental states, clothing, and scene destinations via sub-opcodes.
* **Structure:**
* `Int32` — Sub-opcode
* `Byte` — Always `0`
* `Byte` — Always `0`
* `Int32` — Parameter



### Sub-opcodes & Parameters

* **`1`** — Determines H-scene type. *(Note: Checked elsewhere; changing this manually can corrupt behavior).*
* **`3`** — Returns execution to the Main Menu. Parameter is always `1`.
* **`4`** — Dictates character outfit configurations:
* `0` = Underwear + stockings, shoes, etc.
* `1` = Default bikini.
* `2` = Personal dress.
* `3` = School uniform.
* `FF` = Player's active choice.


* **`5`** — Sets scene location. Parameter coordinates with specific indices mapped in `adv_load_map.lst`.

---

## Block 15: Conditional Branching (If/Then/Else)

* **Description:** Evaluates conditions to execute different blocks of script data.
* **Structure:**
* `Int32` — Loop count for **Loop A** (Always `1` in known files).
* `Byte` — Always `0`.
* `Int32` — Number of commands to run if the condition evaluates to **True**.
* `Int32` — Number of commands to run if the condition evaluates to **False** (Always `0` in known files).



> ### Loop A Structure
> 
> 
> * `Int32`, `Byte`, `Byte`, `Byte`, `Int32` — Unknown; likely the variable comparison test parameters.
> 
> 

* **`Data`** — Nested script commands for the **True** branch.
* **`Data`** — Nested script commands for the **False** branch.

---

## Blocks 16 – 19: Unused

* **Description:** Not present in any known `.sdt` files.

---

## Block 20: Terminate Script

* **Description:** Explicitly stops script execution.

---

## Block 21: Unused

* **Description:** Not present in any known `.sdt` files.

---

## Block 22: Jump to Label

* **Description:** Instantly redirects script execution to a local target label defined by Block 10.
* **Structure:**
* `Text` — Name of the destination label.



---

## Blocks 23 – 24: Unused

* **Description:** Not present in any known `.sdt` files.

---

## Block 25: Delay / Wait

* **Description:** Pauses execution for a set duration. Can be skipped prematurely by clicking the mouse.
* **Structure:**
* `Int32` — Wait duration (in milliseconds).



---

## Block 26: Asset Loader (.lst Parser)

* **Description:** Populates map assets or character models into the scene environment using references from configuration files.
* **Structure:**
* `Int32` — File index target (`0` = `adv_load_map.lst`; `1` or higher = `adv_load_cha.lst`. Max observed value is `3`).
* `Byte` — Category flag (`0` if Int32 is 0 [Map], `1` otherwise [Character]).
* `Text` — Reference filename (Either `"adv_load_cha.lst"`, `"adv_load_map.lst"`, or `"NULL"`).
* `Int32` — Line index target inside the specified `.lst` file (Value is `0` if text is `"NULL"`).



### Context Rules

* **With `adv_load_map.lst`:** Spawns/renders the 3D location scenery.
* **With `adv_load_cha.lst`:** Spawns actors into the scene. The `Int32` creates an ID number used by subsequent commands to manipulate that specific character.
* **With `"NULL"`:** Unloads the active map or character ID from memory. *Crucial:* Maps are always explicitly unloaded before loading a replacement asset.

---

## Block 27: Sequence Flow Linker

* **Description:** Purpose unknown. In random samples, it is consistently preceded by either Block 30 or another instance of Block 27.
* **Structure:**
* `Int32`
* `Byte`
* `Int32`
* `Int32`



---

## Block 28: Head Transform

* **Description:** Believed to control head-tilting movements. Often directly followed by Block 29.
* **Structure:**
* `Int32` — Character ID
* `Int32`
* `Int32`
* `Byte`



---

## Block 29: Actor Asset Hook

* **Description:** Purpose unknown.
* **Structure:**
* `Int32` — Unknown.
* `Int32` — Always `0xFFFFFFFF` if the subsequent text string is `"NULL"`.
* `Text` — Frequently `"NULL"`; otherwise references a specific model mesh name.



---

## Block 30: Character Pose Update

* **Description:** Switches an active character model to a different default pose stance.
* **Structure:**
* `Int32` — Character ID (Assigned originally by Block 26).
* `Int32` — Always `-1` (`0xFFFFFFFF`).
* `Int32` — Target Pose ID to switch to.
* `Byte` — Always `0`.
* `Byte` — Always `0`.
* `Byte` — Always `0` or `1`.



---

## Block 31: Character Expressions

* **Description:** Modifies facial components (eyes/mouth) on a specific character.
* **Structure:**
* `Int32` — Character ID (Assigned originally by Block 26).
* `Int32` — Eye expression code index.
* `Int32` — Mouth expression code index.
* `Byte` — Always `0` or `1`.



---

## Block 32: Character Position Vector

* **Description:** Coordinates character displacement/walking across the scene space. Frequently preceded by Block 26 and immediately followed by Block 33.
* **Structure:**
* `Int32` — Character ID.
* `Byte` — Always `0`.
* `Int32` — Walking animation duration (in milliseconds).
* `Float32` — X-Coordinate / Vector position.
* `Float32` — Y-Coordinate / Vector position.
* `Float32` — Z-Coordinate / Vector position.
* `Byte` — Always `0` or `1`.



---

## Block 33: Character Rotation / Facing Direction

* **Description:** Forces a character model to turn toward a specific rotation angle.
* **Structure:**
* `Int32` — Character ID.
* `Byte`
* `Int32` — Rotation duration (in milliseconds).
* `Int32` — Usually `0`.
* `Float32` — Target direction angle (Unsure if relative or absolute).
* `Int32` — Usually `0`.
* `Byte`



---

## Blocks 34 – 35: Unused

* **Description:** Not present in any known `.sdt` files.

---

## Block 36: Mesh Component Controller

* **Description:** Dynamically attaches or removes individual model sub-meshes (such as individual garments or accessories) onto a character.
* **Structure:**
* `Int32` — Character ID.
* `Int32`
* `Text` — Target mesh name located within the character's `.xx` skeleton asset (Definitions extracted via `cat*.lst` files).
* `Byte` — State modifier (`0` = Add/Show mesh; `1` = Remove/Hide mesh).



---

## Block 37: Unused Variable

* **Description:** Purpose unknown.
* **Structure:**
* `Float32`



---

## Block 38: Camera FOV / Depth Transform

* **Description:** Executes linear camera zooms or translations forward and backward.
* **Structure:**
* `Byte` — Always `0`.
* `Int32` — Camera translation duration.
* `Float32`
* `Float32`
* `Float32`
* `Byte` — Always `0` or `1`.



---

## Block 39: Camera Spatial Positioning

* **Description:** Updates camera placement vectors.
* **Structure:**
* `Byte`
* `Int32` — Move duration.
* `Int32` — Possibly float data.
* `Int32` — Possibly float data.
* `Int32` — Possibly float data.
* `Byte`



---

## Block 40: Unused

* **Description:** Not present in any known `.sdt` files.

---

## Block 41: Coordinates Anchor

* **Description:** Purpose unknown.
* **Structure:**
* `Int32`
* `Int32`



---

## Blocks 42 – 47: Sequential Script Hooks

* **Description:** Purposes unknown. These numbers routinely follow each other sequentially (`42` $\rightarrow$ `43` $\rightarrow$ `44` $\rightarrow$ `45` $\rightarrow$ `46` $\rightarrow$ `47`) in script streams.

---

## Block 48: Scene Overlay Blend (Background Only)

* **Description:** Gradually blends the environmental background to/from a color. Unlike Block 2, **dialogue text boxes remain clearly visible** above the tint layer. The color overlay remains persistent until another Block 48 overrides it.
* **Structure:**
* `Byte` — Mode (`1` = Blend to color; `0` = Fade away from color).
* `Int32` — Color value.
* `Int32` — Transition duration (in milliseconds).



---

## Block 49: End Sequence Flag

* **Description:** Purpose unknown.
* **Structure:**
* `Int32`
* `Byte`