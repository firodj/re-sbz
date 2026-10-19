#pragma once

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

class D3DXContext : public IUnknown {};
typedef D3DXContext* LPD3DXCONTEXT;

struct D3DXVECTOR3 {
    float x, y, z;
};

typedef struct D3DXQUATERNION {
    float x, y, z, w;
} D3DXQUATERNION;

typedef struct D3DXMATRIX {
    union
    {
        float m[4][4];
#ifdef __cplusplus
        struct
        {
            float _11;
            float _12;
            float _13;
            float _14;
            float _21;
            float _22;
            float _23;
            float _24;
            float _31;
            float _32;
            float _33;
            float _34;
            float _41;
            float _42;
            float _43;
            float _44;
        };
#endif //__cplusplus
    };
} D3DXMATRIX;


// D3DX_VIDEOMODEDESC from d3dxcore.h size=0x10
struct D3DX_VIDMODEDESC {
  DWORD width;
  DWORD height;
  DWORD bpp;
  DWORD refreshRate;
};

#define D3DX_DRIVERDESC_LENGTH    256

// D3DX_DEVICEDESC from d3dxcore.h. sizeof=0x140
struct D3DX_DEVICEDESC {
  DWORD       deviceIndex;
  DWORD       hwLevel;
  GUID        ddGuid;
  GUID        d3dDeviceGuid;
  GUID        ddDeviceID;
  char        driverDesc[D3DX_DRIVERDESC_LENGTH];
  HMONITOR    monitor;
  BOOL        onPrimary;
};

extern LPDIRECT3DDEVICE7 &g_pD3DDevice_4D6C78;
extern D3DDEVICEDESC7 g_d3dDevDesc_4D6980;
extern LPDIRECTSOUND3DLISTENER& g_pds3DListener; // 0x52F3C4

extern HRESULT(*D3DXUninitialize)(void);
extern HRESULT (*D3DXInitialize)(void); 
extern DWORD (*D3DXGetDeviceCount)(void);
extern DWORD (*D3DXGetMaxNumVideoModes)(DWORD deviceIndex, DWORD flags);
extern HRESULT (*D3DXGetDeviceDescription)(DWORD deviceIndex, D3DX_DEVICEDESC* pd3dxDeviceDesc); 
extern HRESULT (*D3DXGetVideoMode)(DWORD deviceIndex, DWORD flags, DWORD modeIndex, D3DX_VIDMODEDESC* pModeList);
extern HRESULT (*D3DXGetDeviceCaps)(DWORD deviceIndex, D3DX_VIDMODEDESC* pVidMode, D3DDEVICEDESC7* pD3DCaps, DDCAPS* pDDHALCaps, DDCAPS* pDDHELCaps); 
extern void (__stdcall* D3DXGetErrorString)(int hr, size_t strLength, char* str);
