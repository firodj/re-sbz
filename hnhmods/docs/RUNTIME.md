# Part III: Core Runtime Systems

## The Main Game Loop

EXAMPLE: Fixed vs. variable timestep logic, delta time handling, input polling intervals, and frame pacing.
NOTES:
- each scene function has its game loop, and using `DoPeekMessages`, and `GetInputState_481B37`, etc. to read from input thread.

## Resource Management & Asset Loading

EXAMPLE: Virtual File Systems (VFS), packed archive formats (`.pak`/`.dat`), stream decompression (zlib/LZ4), asset hot-reloading, and reference counting.
NOTES:
- using unpacked data, exactly as `.bmp`, etc.
- the asset structure setup by `SetupDirPath_486630`, and data types.
- the ref count into variables `g_nRef...`
- game story and script parse by `ParseExecStr_487D89` by reading `.str` file.

## Scene Graph & Entity Management

EXAMPLE: Entity object layout, spatial node hierarchies, component updates, and memory locality optimizations.
NOTES:
- Scene root in `g_pSceneFrame`, and for camera `g_pCameraFrame`.
- Populated scene being object pool into `g_pObjectTable`,  `g_nObjectTableCount`, `g_pObjPool`.

