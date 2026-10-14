# AGENTS.md


## Python
Python using `py` intead.


## Extracting Decompiled C/C++

When extract a c/c++ function from IDA Pro MCP, put output into `./hnhmods/src`.
The extracted function should have clean control flow, remove GOTO if possible,
and explain the steps with comments. Remove the original location address comments per line. 

The source should contain register_{functionNameAddr}. For example when `functionNameAddr` is `WinMain_485CE0` then: 

```c
void register_WinMain_485CE0() {
    LPVOID addr = (LPVOID)0x485CE0; // <- the function address
    LPVOID* orig = NULL;
    MH_STATUS res = MH_CreateHook(addr, reinterpret_cast<LPVOID>(WinMain_485CE0), reinterpret_cast<LPVOID*>(&orig));
    if (res == MH_OK) res = MH_EnableHook(addr);
    DebugLog("[HNH] Hooked WinMain_485CE0; %s\n", MH_StatusToString(res));
}
```

At the top of file should have:

```c
#include "pch.h"
#include "console.hpp"

```

## Create IDA Python Script

The script file should be written into `./idascripts` sub folder. The script should compatible with IDA Pro 9.1.
To iterate over function use `idautils.FuncItems(func_ea)` instead.
## Others

