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


extern LPDIRECT3DDEVICE7 &g_pD3DDevice_4D6C78;
extern D3DDEVICEDESC7 g_d3dDevDesc_4D6980;
extern LPDIRECTSOUND3DLISTENER& g_pds3DListener; // 0x52F3C4