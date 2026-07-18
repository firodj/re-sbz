# Decompiling the constructor of new class

The decompiled code:

```cpp
CD3duGlobals *v0, pv;
CD3duGlobals *g_pGlobals;
int v8

v0 = (CD3duGlobals *)CD3duAlloc::operator new(0x18u);
pv = v0;
LOBYTE(v8) = 1;
if ( v0 )
    v1 = CD3duGlobals::CD3duGlobals(v0);
else
    v1 = 0;
LOBYTE(v8) = 0;
CD3duGlobals *g_pGlobals = v1;
```

The expected source code:

```cpp
CD3duGlobals *g_pGlobals = new CD3duGlobals();
```

The `CD3duGlobals` is child of `CD3duAlloc`.
The operator `new` using an overriden `CD3duAlloc::operator new`.
The size of `CD3duGlobals` is `0x18u` bytes.

# Decompiling the custom exception class.

The decompiled code:

```cpp
_BYTE pExceptionObject[136];
CD3duAlloc v5[136];

CD3duAlloc::CD3duAlloc(v5);
*(_DWORD *)&v5[128] = 0xC8770BC6;
strcpy((char *)v5, "D3DXInitialize - Could not start up D3DX.");
*(_DWORD *)&v5[132] = 20;
qmemcpy(pExceptionObject, v5, sizeof(pExceptionObject));
_CxxThrowException(pExceptionObject, &_TI2_AVCD3DXException__);
```

The expected source code:

```cpp
throw CD3DXException("D3DXInitialize - Could not start up D3DX.", 0xC8770BC6, 20);
```

The exception class `CD3DXException` is child of `CD3duAlloc`. The size of
`CD3DXException` is `136` bytes. The exception constructor can be deducted into:

```cpp
class CD3DXException: CD3duAlloc
{
public:
  CD3DXException(const char* _message, int _error, int _line);
  
  char message[128];
  int error;
  int line;
};
```
