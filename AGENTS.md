# AGENTS.md

## Python
Python using `py` intead.

## Extracting Decompiled C/C++

When extract a c/c++ function from IDA Pro MCP, put output into `./hnhmods/src`.
The extracted function name should have prefixed with `hook_`.
The extracted function should have clean control flow, 
remove GOTO if possible, better function arguments name, better local variable name,
and explain the steps with comments. The file should save with Shift-JIS encoding.

Add reference to global variables by direct access to its addres, for example if the function has access to variable `g_dwFogEnable_4D6CF4` with type `DWORD` at address `0x4D6CF4`, then it will have:

```c
DWORD &g_dwFogEnable_4D6CF4 = *(DWORD *)0x4D6CF4;
```

For function call other than API call, you might add references to target function address, for example if the function have access to function `DInputInit_43AD67` where the function adrress is `0x43AD67` then it will have:

```c
int (__cdecl *DInputInit_43AD67)(HINSTANCE hinst, HWND hWnd) = reinterpret_cast<int(__cdecl*)(HINSTANCE, HWND)>(0x43AD67);
void (__cdecl *GameMainLoop_47F013)(void) = reinterpret_cast<void(__cdecl*)(void)>(0x47F013);
```

If the function have more than 1 Xrefs, add Minhook register function called register_{hook_FunctionNameAddr}. For example when `functionNameAddr` is `WinMain_485CE0` then call the `hnh_Hook` with `hook_WinMain_485CE0` such: 

```c
void register_WinMain_485CE0() {
    hnh_Hook(0x485CE0, hook_WinMain_485CE0, "WinMain_485CE0");
}
```

where:
- arg1: 0x485CE0 <- function address
- arg2: WinMain_485CE0 <- the function name
- arg3: "WinMain_485CE0" <- function name in string

At the top of file should have and not using precompiled header (.pch):

```c
#include "dx7.hpp"
#include "console.hpp"
#include "shared.hpp"
```

You don'n need to dig deeper to verify by try to compiling it.

## Create IDA Python Script

The script file should be written into `./idascripts` sub folder. The script should compatible with IDA Pro 9.1.
To iterate over function use `idautils.FuncItems(func_ea)` instead.

## Others

