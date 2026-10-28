#pragma once

#define DIRECT3D_VERSION 0x0700
#define DIRECTINPUT_VERSION 0x0700

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
#include <dinput.h>
#include <timeapi.h>

#include "../d3dx/ida_defs.hpp"

#include "minhook/include/MinHook.h"

//-------------------------------------------------------------------------
// ID3DXContext interface: 
//
// This encapsulates all the stuff that the app might 
// want to do at initialization time and any global control over d3d and 
// ddraw.
//-------------------------------------------------------------------------

typedef struct ID3DXContext *LPD3DXCONTEXT;
DECLARE_INTERFACE_(ID3DXContext, IUnknown)
{
  //
  // IUnknown methods 
  //
  STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID* ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef)(THIS) PURE;
  STDMETHOD_(ULONG,Release)(THIS) PURE;

  // Get the DDraw and Direct3D objects to call DirectDraw or
  // Direct3D Immediate Mode functions. 
  // If the objects don't exist (because they have not
  // been created for some reason) NULL is returned.
  // All the objects returned in the following Get* functions
  // are addref'ed. It is the application's responsibility to
  // release them when no longer needed.
  STDMETHOD_(LPDIRECTDRAW7,GetDD)(THIS) PURE;
  STDMETHOD_(LPDIRECT3D7,GetD3D)(THIS) PURE;
  STDMETHOD_(LPDIRECT3DDEVICE7,GetD3DDevice)(THIS) PURE;

  // Get the various buffers that get created at the init time
  // These are addref'ed as well. It is the application's responsibility
  // to release them before the app quits or when it needs a resize.
  STDMETHOD_(LPDIRECTDRAWSURFACE7,GetPrimary)(THIS) PURE;
  STDMETHOD_(LPDIRECTDRAWSURFACE7,GetZBuffer)(THIS) PURE;
  STDMETHOD_(LPDIRECTDRAWSURFACE7,GetBackBuffer)(THIS_ DWORD which) PURE;

  // Get the associated window handles 
  STDMETHOD_(HWND,GetWindow)(THIS) PURE;
  STDMETHOD_(HWND,GetFocusWindow)(THIS) PURE;

  // 
  // Various Get methods, in case the user had specified default
  // parameters
  //
  STDMETHOD(GetDeviceIndex)(THIS_ 
    LPDWORD pDeviceIndex, 
    LPDWORD pHwLevel) PURE;

  STDMETHOD_(DWORD, GetNumBackBuffers)(THIS) PURE;

  STDMETHOD(GetNumBits)(THIS_
    LPDWORD pColorBits, 
    LPDWORD pDepthBits,
    LPDWORD pAlphaBits, 
    LPDWORD pStencilBits) PURE;

  STDMETHOD(GetBufferSize)(THIS_ 
    LPDWORD pWidth, 
    LPDWORD pHeight) PURE;

  // Get the flags that were used to create this context
  STDMETHOD_(DWORD, GetCreationFlags)(THIS) PURE;
  STDMETHOD_(DWORD, GetRefreshRate)(THIS) PURE;

  // Restoring surfaces in case stuff is lost
  STDMETHOD(RestoreSurfaces)(THIS) PURE;

  // Resize all the buffers to the new width and height
  STDMETHOD(Resize)(THIS_ DWORD width, DWORD height) PURE;

  // Update the frame using a flip or a blit,
  // If the D3DX_UPDATE_NOVSYNC flag is set, blit is used if the 
  // driver cannot flip without waiting for vsync in full-screen mode.
  STDMETHOD(UpdateFrame)(THIS_ DWORD flags) PURE;

  // Render a string at the specified coordinates, with the specified 
  // colour. This is only provided as a convenience for 
  // debugging/information during development.
  // topLeftX and topLeftY represent the location of the top left corner
  // of the string, on the render target.  
  // The coordinate and color parameters each have a range of 0.0-1.0
  STDMETHOD(DrawDebugText)(THIS_
    float topLeftX, 
    float topLeftY,
    D3DCOLOR color,
    LPSTR pString) PURE;

  // Clears to the current viewport
  // The following are the valid flags:
  // D3DCLEAR_TARGET  (to clear the render target )
  // D3DCLEAR_ZBUFFER (to clear the depth-buffer )
  // D3DCLEAR_STENCIL (to clear the stencil-buffer )
  STDMETHOD(Clear)(THIS_ DWORD ClearFlags) PURE;

  STDMETHOD(SetClearColor)(THIS_ D3DCOLOR color ) PURE;
  STDMETHOD(SetClearDepth)(THIS_ float z) PURE;
  STDMETHOD(SetClearStencil)(THIS_ DWORD stencil) PURE;
};


class D3DXVECTOR3 {
public:
    float x, y, z;
    D3DXVECTOR3() : x(0), y(0), z(0) {};
    D3DXVECTOR3(float _x, float _y, float _z) {
      Vec3Set(_x, _y, _z);
    };

    void Vec3Set(float _x, float _y, float _z) {
      x = _x; y = _y; z = _z;
    }
};

class D3DXVECTOR2 {
public:
  D3DXVECTOR2() : x(0), y(0) {};
  D3DXVECTOR2(float _x, float _y) {
    x = _x, y = _y;
  };
  float x, y;
};

typedef struct D3DXVECTOR4 {
  float x, y, z, w;
} D3DXVECTOR4;

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



//===========================================================================
//
//    Matrix Stack
//
//===========================================================================

typedef struct ID3DXMatrixStack *LPD3DXMATRIXSTACK;
DECLARE_INTERFACE_(ID3DXMatrixStack, IUnknown)
{
  //
  // IUnknown methods
  //
  STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef)(THIS) PURE;
  STDMETHOD_(ULONG,Release)(THIS) PURE;

  //
  // ID3DXMatrixStack methods
  //

  // Pops the top of the stack, returns the current top
  // *after* popping the top.
  STDMETHOD(Pop)(THIS) PURE;

  // Pushes the stack by one, duplicating the current matrix.
  STDMETHOD(Push)(THIS) PURE;

  // Loads identity in the current matrix.
  STDMETHOD(LoadIdentity)(THIS) PURE;

  // Loads the given matrix into the current matrix
  STDMETHOD(LoadMatrix)(THIS_ const D3DXMATRIX* pM ) PURE;

  // Right-Multiplies the given matrix to the current matrix.
  // (transformation is about the current world origin)
  STDMETHOD(MultMatrix)(THIS_ const D3DXMATRIX* pM ) PURE;

  // Left-Multiplies the given matrix to the current matrix
  // (transformation is about the local origin of the object)
  STDMETHOD(MultMatrixLocal)(THIS_ const D3DXMATRIX* pM ) PURE;

  // Right multiply the current matrix with the computed rotation
  // matrix, counterclockwise about the given axis with the given angle.
  // (rotation is about the current world origin)
  STDMETHOD(RotateAxis)
    (THIS_ const D3DXVECTOR3* pV, float angle) PURE;

  // Left multiply the current matrix with the computed rotation
  // matrix, counterclockwise about the given axis with the given angle.
  // (rotation is about the local origin of the object)
  STDMETHOD(RotateAxisLocal)
    (THIS_ const D3DXVECTOR3* pV, float angle) PURE;

  // Right multiply the current matrix with the computed rotation
  // matrix. All angles are counterclockwise. (rotation is about the
  // current world origin)

  // The rotation is composed of a yaw around the Y axis, a pitch around
  // the X axis, and a roll around the Z axis.
  STDMETHOD(RotateYawPitchRoll)
    (THIS_ float yaw, float pitch, float roll) PURE;

  // Left multiply the current matrix with the computed rotation
  // matrix. All angles are counterclockwise. (rotation is about the
  // local origin of the object)

  // The rotation is composed of a yaw around the Y axis, a pitch around
  // the X axis, and a roll around the Z axis.
  STDMETHOD(RotateYawPitchRollLocal)
    (THIS_ float yaw, float pitch, float roll) PURE;

  // Right multiply the current matrix with the computed scale
  // matrix. (transformation is about the current world origin)
  STDMETHOD(Scale)(THIS_ float x, float y, float z) PURE;

  // Left multiply the current matrix with the computed scale
  // matrix. (transformation is about the local origin of the object)
  STDMETHOD(ScaleLocal)(THIS_ float x, float y, float z) PURE;

  // Right multiply the current matrix with the computed translation
  // matrix. (transformation is about the current world origin)
  STDMETHOD(Translate)(THIS_ float x, float y, float z ) PURE;

  // Left multiply the current matrix with the computed translation
  // matrix. (transformation is about the local origin of the object)
  STDMETHOD(TranslateLocal)(THIS_ float x, float y, float z) PURE;

  // Obtain the current matrix at the top of the stack
  STDMETHOD_(D3DXMATRIX*, GetTop)(THIS) PURE;
};


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

typedef enum _D3DX_SURFACEFORMAT {
  D3DX_SF_UNKNOWN    = 0,
  D3DX_SF_R8G8B8     = 1,
  D3DX_SF_A8R8G8B8   = 2,
  D3DX_SF_X8R8G8B8   = 3,
  D3DX_SF_R5G6B5     = 4,
  D3DX_SF_R5G5B5     = 5,
  D3DX_SF_PALETTE4   = 6,
  D3DX_SF_PALETTE8   = 7,
  D3DX_SF_A1R5G5B5   = 8,
  D3DX_SF_X4R4G4B4   = 9,
  D3DX_SF_A4R4G4B4   =10,
  D3DX_SF_L8         =11,
  D3DX_SF_A8L8       =12,
  D3DX_SF_U8V8       =13,
  D3DX_SF_U5V5L6     =14,
  D3DX_SF_U8V8L8     =15,
  D3DX_SF_UYVY       =16,
  D3DX_SF_YUY2       =17,
  D3DX_SF_DXT1       =18,
  D3DX_SF_DXT3       =19,
  D3DX_SF_DXT5       =20,
  D3DX_SF_R3G3B2     =21,
  D3DX_SF_A8         =22,
  D3DX_SF_TEXTUREMAX =23,

  D3DX_SF_Z16S0      =256,
  D3DX_SF_Z32S0      =257,
  D3DX_SF_Z15S1      =258,
  D3DX_SF_Z24S8      =259,
  D3DX_SF_S1Z15      =260,
  D3DX_SF_S8Z24      =261,
  D3DX_SF_DEPTHMAX   =262,

  D3DX_SF_FORCEMAX  = (DWORD)(-1)
} D3DX_SURFACEFORMAT;

#define D3DX_DEFAULT ULONG_MAX

typedef enum _D3DX_FILTERTYPE {
  D3DX_FT_POINT    = 0x01,
  D3DX_FT_LINEAR   = 0x02,
  D3DX_FT_DEFAULT  = D3DX_DEFAULT
} D3DX_FILTERTYPE;

struct D2D_RECT_F
{
  FLOAT left;
  FLOAT top;
  FLOAT right;
  FLOAT bottom;
};

#define D3DX_UPDATE_NOVSYNC (1<<0)

LPVOID hnh_Hook(DWORD dwAddr, LPVOID pDetour, const char* pszName);

extern HRESULT(*D3DXUninitialize)(void);
extern HRESULT (*D3DXInitialize)(void); 
extern DWORD (*D3DXGetDeviceCount)(void);
extern DWORD (__stdcall *D3DXGetMaxNumVideoModes)(DWORD deviceIndex, DWORD flags);
extern HRESULT (__stdcall *D3DXGetDeviceDescription)(DWORD deviceIndex, D3DX_DEVICEDESC* pd3dxDeviceDesc); 
extern HRESULT (__stdcall *D3DXGetVideoMode)(DWORD deviceIndex, DWORD flags, DWORD modeIndex, D3DX_VIDMODEDESC* pModeList);
extern HRESULT (__stdcall *D3DXGetDeviceCaps)(DWORD deviceIndex, D3DX_VIDMODEDESC* pVidMode, D3DDEVICEDESC7* pD3DCaps, DDCAPS* pDDHALCaps, DDCAPS* pDDHELCaps); 
extern void (__stdcall* D3DXGetErrorString)(int hr, size_t strLength, char* str);

extern D3DXVECTOR3* (__stdcall*D3DXVec3Normalize)(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV);
extern D3DXMATRIX* (__stdcall*D3DXMatrixRotationAxis)(D3DXMATRIX* pOut, const D3DXVECTOR3* pV, float angle);
extern double (__cdecl*Vec3LengthSq)(const D3DXVECTOR3* pV);
extern void (__thiscall * Vec3Set)(D3DXVECTOR3 *This, float x, float y, float z);
extern double (__cdecl*Vec3Dot)(const D3DXVECTOR3* pV1, const D3DXVECTOR3* pV2);
extern double (__cdecl*DegreesToRadians)(float deg);
extern HRESULT (__cdecl*MatrixPerspectiveFovLH)(D3DXMATRIX* pMat, float fovy, float aspect, float zn, float zf);
extern D3DXMATRIX* (__cdecl*D3DXMatrixIdentity)(D3DXMATRIX* pOut);
extern D3DXMATRIX* (__stdcall*D3DXMatrixMultiply)(D3DXMATRIX* pOut, const D3DXMATRIX* pM1, const D3DXMATRIX* pM2);
extern HRESULT (__cdecl*MatrixLookAtLH)(D3DXMATRIX* pMat, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt, const D3DXVECTOR3* pUp);
extern D3DXMATRIX* (__stdcall*D3DXMatrixInverse)(D3DXMATRIX* pOut, float* pfDeterminant, const D3DXMATRIX* pM);
extern HRESULT(__stdcall* D3DXCreateContextEx)(unsigned int devIndex, unsigned int flags, HWND hwnd, HWND focusWnd, int nNumColorBits, int nNumAlphaBits, int nNumDepthBits, int nNumStencilBits, int nNumBackBuffers, int nWidth, int nHeight, int nRefreshRate, LPD3DXCONTEXT* ppCtx);
extern HRESULT(__stdcall* D3DXCreateContext)(unsigned int devIndex, unsigned int flags, HWND hWnd, int width, int height, LPD3DXCONTEXT* ppCtx);
extern HRESULT(__stdcall* D3DXGetSurfaceFormat)(int devIndex, D3DX_VIDMODEDESC* pVidMode, unsigned int surfClassFlags, unsigned int which, D3DX_SURFACEFORMAT* pFormat);
extern DWORD(__stdcall* D3DXGetMaxSurfaceFormats)(int devIndex, _DWORD* pVidMode, int surfClassFlags);
extern HRESULT(__stdcall* D3DXCreateMatrixStack)(DWORD flags, LPD3DXMATRIXSTACK* ppStack);
extern HRESULT(__stdcall* D3DXCreateTexture)(LPDIRECT3DDEVICE7 pd3dDevice, LPDWORD pFlags, LPDWORD pWidth, LPDWORD pHeight, _D3DX_SURFACEFORMAT* pPixelFormat, LPDIRECTDRAWPALETTE pDDPal, LPDIRECTDRAWSURFACE7* ppDDSurf, LPDWORD pNumMipMaps);
extern HRESULT(__stdcall* D3DXCheckTextureRequirements)(LPDIRECT3DDEVICE7 pd3dDevice, LPDWORD pFlags, LPDWORD pWidth, LPDWORD pHeight, _D3DX_SURFACEFORMAT* pPixelFormat);
extern HRESULT(__stdcall* D3DXLoadTextureFromSurface)(LPDIRECT3DDEVICE7 pd3dDevice, LPDIRECTDRAWSURFACE7 pTexture, DWORD mipMapLevel, LPDIRECTDRAWSURFACE7 pSurfaceSrc, LPRECT pSrcRect, LPRECT pDestRect, _D3DX_FILTERTYPE filterType);
extern HRESULT(__stdcall* D3DXDrawSpriteSimple)(LPDIRECTDRAWSURFACE7 pd3dTexture,
  LPDIRECT3DDEVICE7 pd3dDevice, const D3DXVECTOR3* ppointDest,
  float alpha, float scale, float angleRad,
  const D3DXVECTOR2* pOffset, const RECT* pSourceRect);
extern HRESULT(__stdcall* D3DXDrawSprite3D)(LPDIRECTDRAWSURFACE7 pd3dTexture,
  LPDIRECT3DDEVICE7 pd3dDevice, const D3DXVECTOR4 quad[4], float alpha,
  const RECT* pSourceRect);

extern BOOL(__cdecl* DIMouseKeybInit_43AF33)(int bAcquire);
extern BOOL(__cdecl* DInputInit_43AD67)(HINSTANCE hinst, HWND hWnd);
