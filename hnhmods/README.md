# HNH - Hana No Hime

## Game Engine Breakdown

- Part I: [Methodology & Boot Sequence](./docs/BOOTSEQUENCE.md)
- Part II: [Core Architecture & Memory Foundations](./docs/ARCHITECTURE.md)
- Part III: [Core Runtime Systems](./docs/RUNTIME.md)
- Part IV: [Rendering & Pipeline Graphics](./docs/GRAPHICS.md)
- Part V: [Gameplay Subsystems](./docs/SUBSYSTEMS.md)
- Part VI: [File Format Specifications](./docs/FORMATS.md)
- Part VII: [Combat & Scene Systems](./docs/COMBAT.md)
- Part VIII: [Text, Font & UI Pipeline](./docs/TEXTUI.md)
- Part IX: [Appendix - Globals Registry & Reconstruction Status](./docs/APPENDIX.md)
- Companion: [Character Pipeline - SEK/ODF Data to Rendered Vertices](./docs/CHARACTER.md)

NOTE: The standalone application start from [exemain.cpp](./exemain.cpp), redirecting all function into its implemented function which has prefix `hook_`,
then execute the game `WinMain_485CE0` function. All sources in the [src](./src/) folder. For the game folder/asset in the `APPDIR` folder defined in [PropertySheet.props](./PropertySheet.props). The project configuration in [hnhmods.vcxproj](./hnhmods.vcxproj) with **DebugBinDX7|Win32** configuration. 

## Others

### Video Re-Encoders

The game video using **IV50** encoders which is legacy encoders. To solve unplayable video, either enable **IV50** decoders or
re-encode to **Xvid**.
Install **ffdshow_trytout** 32-bit, and **ffmpeg** to re-encode.

```
$ ffmpeg -i op.avi -c:v libxvid -q:v 3 -c:a pcm_s16le op_xvid.avi
$ ffmpeg -i ED.avi -c:v libxvid -q:v 3 -c:a pcm_s16le ED_xvid.avi
```


