#define DIRECT3D_VERSION 0x0700
// add headers that you want to pre-compile here
#include "framework.h"

#include <vector>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdarg>

#include <d3d.h>
#include <mmreg.h>  // Declares WAVEFORMATEX
#include <dsound.h>
#include <ddraw.h>
#include <timeapi.h>


#include "minhook/include/MinHook.h"


extern LPDIRECT3DDEVICE7 &g_pD3DDevice_4D6C78;
extern D3DDEVICEDESC7 g_d3dDevDesc_4D6980;
extern LPDIRECTSOUND3DLISTENER& g_pds3DListener; // 0x52F3C4