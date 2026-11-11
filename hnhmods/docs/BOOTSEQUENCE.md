# Methodology & Boot Sequence

## Reverse Engineering Tooling & Setup

EXAMPLE: Static analysis (Ghidra/IDA Pro), dynamic tracing/emulation (Unicorn, Capstone, Triton DBA), memory inspection, and string symbol recovery.

The game released around 2001, using DirectX 7 technology with executable file is `HaNaHiMe.exe`.

Static analysis using IDA Pro 9.1, using MCP and Coding Agent for analyze function
and give proper name for function name for variable names. For debugging using Local Windows debugger occasionaly, and
utilize debug logging on hooked functions.

The methodology is insipred by [GTA/re3](https://github.com/GTAmodding/re3/tree/master/) using
[Ship of Theseus](https://en.wikipedia.org/wiki/Ship_of_Theseus) philosophy.

By utilize existing tool [Universal-Dear-ImGui-Hook](https://github.com/Sh0ckFR/Universal-Dear-ImGui-Hook) we can swap original function
with the decompiled version, and test one by one, until all of covered function tested.

Not all functions need to replaced, but covered by following call graphs from entrpont and saved into [GOALS](../GOALS.md).
The orphaned function / non-referenced xref will be skipped. The updated list is in the [CURRENT](../CURRENT.md).

## Engine Bootstrapping & Initialization

EXAMPLE: Binary entry point (`WinMain`), subsystem boot dependency graph, static initializers, context creation (DirectX/Vulkan/OpenGL context), and config file parsing.
NOTES:
- entrypoint `WinMain_485CE0`.
- config file: `Tea.ex`.
- system save: `data0.dat`.
- init: `D3DInit_40A894` using `D3DXCreateContextEx`, and `g_pRenderCtx`.
- DRM check: `CDROM_VerifyDiscSignature`.