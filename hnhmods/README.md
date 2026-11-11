# HNH - Hana No Hime

## Game Engine Breakdown

- Part I: [Methodology & Boot Sequence](./docs/BOOTSEQUENCE.md)
- Part II: [Core Architecture & Memory Foundations](./docs/ARCHITECTURE.md)
- Part III: [Core Runtime Systems](./docs/RUNTIME.md)
- Part IV: [Rendering & Pipeline Graphics](./docs/GRAPHICS.md)
- Part V: [Gameplay Subsystems](./docs/SUBSYSTEMS.md)

NOTE: The standalone application start from [exemain.cpp](./exemain.cpp), redirecting all function into its implemnted function which has prefix `hook_`,
then execute the game `WinMain_485CE0` function. All souces in the [src](./src/) folders. For the the game folder/asset in the `APP_DIR` fodler diefined in [PropertSheet.props](./PropertySheet.props). The project configuration in [hnhmods.vcproj](./hnhmods.vcxproj) with **DebugBinDX7|Win32** configuration. 

## Others

### Video Re-Encoders

The game video using **IV50** encoders with is legacy encoders. To solve unplayable video, either enable **IV50** decoders or
re-encode to **Xvid**.
Install **ffdshow_trytout** 32-bit, andd **ffmpeg** to re-encode.

```
$ ffmpeg -i op.avi -c:v libxvid -q:v 3 -c:a pcm_s16le op_xvid.avi
$ ffmpeg -i ED.avi -c:v libxvid -q:v 3 -c:a pcm_s16le ED_xvid.avi
```


