# Part V: Gameplay Subsystems

## Physics & Spatial Queries

EXAMPLE: Broadphase/narrowphase collision routines, spatial partitioning trees, and Raycast/Sweep query setups.

## Audio Engine Processing

EXAMPLE: Mixer node setup, hardware channel streaming, spatial 3D audio calculations, and voice pooling.
NOTES:
- use DirectSound inited via `HanaDS_Init`.

## Input Architecture:

EXAMPLE: Hardware device abstraction, raw input event buffers, action-mapping tables, and frame-buffered input states.
NOTES:
- thread for reading input via `RestartInputThread` with two modes: `InputThread_Absolute_4817B1` and `InputThread_Relative_481637`.

## Scripting & Modding Hooks

EXAMPLE: Embedded VM runtimes, API binding registration, foreign function interfaces (FFI), and exposed event hooks.
