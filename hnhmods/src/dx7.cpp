#include "dx7.hpp"
#include "console.hpp"
#include "hnh_hook.hpp"

// === GLOBAL FUNCTIONS ===

#ifdef _USRDLL
HRESULT (*D3DXUninitialize)(void) = reinterpret_cast<HRESULT(*)(void)>(0x48B8F0);
HRESULT (*D3DXInitialize)(void) = (HRESULT(*)(void))0x48B80B; 
DWORD (*D3DXGetDeviceCount)(void) = (DWORD(*)(void))0x48B9DD;
DWORD (__stdcall *D3DXGetMaxNumVideoModes)(DWORD deviceIndex, DWORD flags) = (DWORD (__stdcall*)(DWORD, DWORD))0x48BBF0; 
HRESULT (__stdcall *D3DXGetDeviceDescription)(DWORD deviceIndex, D3DX_DEVICEDESC* pd3dxDeviceDesc) = (HRESULT(__stdcall*)(DWORD, D3DX_DEVICEDESC*))0x48BA9E; 
HRESULT (__stdcall *D3DXGetVideoMode)(DWORD deviceIndex, DWORD flags, DWORD modeIndex, D3DX_VIDMODEDESC* pModeList) = (HRESULT (__stdcall*)(DWORD, DWORD, DWORD, D3DX_VIDMODEDESC*))0x48BCDE;
HRESULT (__stdcall *D3DXGetDeviceCaps)(DWORD deviceIndex, D3DX_VIDMODEDESC* pVidMode, D3DDEVICEDESC7* pD3DCaps, DDCAPS* pDDHALCaps, DDCAPS* pDDHELCaps) = (HRESULT (__stdcall*)(DWORD, D3DX_VIDMODEDESC*, D3DDEVICEDESC7*, DDCAPS*, DDCAPS*))0x48C28D; 
void(__stdcall *D3DXGetErrorString)(int hr, size_t strLength, char* str) = reinterpret_cast<void (__stdcall*)(int hr, size_t strLength, char* str)>(0x48c80e);

HRESULT (__stdcall* D3DXCreateContextEx)(unsigned int devIndex, unsigned int flags, HWND hwnd, HWND focusWnd, int nNumColorBits, int nNumAlphaBits, int nNumDepthBits, int nNumStencilBits, int nNumBackBuffers, int nWidth, int nHeight, int nRefreshRate, LPD3DXCONTEXT* ppCtx)
= reinterpret_cast<HRESULT  (__stdcall*)(unsigned int devIndex, unsigned int flags, HWND hwnd, HWND focusWnd, int nNumColorBits, int nNumAlphaBits, int nNumDepthBits, int nNumStencilBits, int nNumBackBuffers, int nWidth, int nHeight, int nRefreshRate, LPD3DXCONTEXT *ppCtx)>(0x48C5ED);

HRESULT(__stdcall* D3DXCreateContext)(unsigned int devIndex, unsigned int flags, HWND hWnd, int width, int height, LPD3DXCONTEXT* ppCtx)
= reinterpret_cast<HRESULT(__stdcall*)(unsigned int devIndex, unsigned int flags, HWND hWnd, int width, int height, LPD3DXCONTEXT * ppCtx)>(0x48C3D6);

HRESULT(__stdcall* D3DXGetSurfaceFormat)(int devIndex, D3DX_VIDMODEDESC* pVidMode, unsigned int surfClassFlags, unsigned int which, D3DX_SURFACEFORMAT* pFormat)
= reinterpret_cast<HRESULT (__stdcall*)(int devIndex, D3DX_VIDMODEDESC *pVidMode, unsigned int surfClassFlags, unsigned int which, D3DX_SURFACEFORMAT *pFormat)>(0x48C09A);

HRESULT(__stdcall* D3DXGetMaxSurfaceFormats)(int devIndex,   D3DX_VIDMODEDESC* pDesc, int surfClassFlags)
= reinterpret_cast<HRESULT (__stdcall*)(int devIndex, D3DX_VIDMODEDESC* pDesc, int surfClassFlags)>(0x48BF99);

D3DXVECTOR3 * (__stdcall * D3DXVec3Normalize)(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV) = reinterpret_cast<D3DXVECTOR3 *(__stdcall*)(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV)>(0x48A1EC);
D3DXMATRIX * (__stdcall * D3DXMatrixRotationAxis)(D3DXMATRIX *pOut, const D3DXVECTOR3 *pV, float angle) = reinterpret_cast<D3DXMATRIX *(__stdcall*)(D3DXMATRIX *pOut, const D3DXVECTOR3 *pV, float angle)>(0x48ABB3);

D3DXMATRIX * (__stdcall * D3DXMatrixMultiply)(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2) = reinterpret_cast<D3DXMATRIX *(__stdcall*)(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2)>(0x48A4BC);
D3DXMATRIX * (__stdcall * D3DXMatrixInverse)(D3DXMATRIX *pOut, float *pfDeterminant, const D3DXMATRIX *pM) = reinterpret_cast<D3DXMATRIX *(__stdcall*)(D3DXMATRIX *pOut, float *pfDeterminant, const D3DXMATRIX *pM)>(0x48A5BD);
HRESULT (__stdcall *D3DXCreateMatrixStack)(DWORD flags, LPD3DXMATRIXSTACK* ppStack) = reinterpret_cast<HRESULT (__stdcall*)(DWORD flags, LPD3DXMATRIXSTACK* ppStack)>(0x48B793);

HRESULT(__stdcall*D3DXCreateTexture)(LPDIRECT3DDEVICE7 pd3dDevice, LPDWORD pFlags, LPDWORD pWidth, LPDWORD pHeight, _D3DX_SURFACEFORMAT* pPixelFormat, LPDIRECTDRAWPALETTE pDDPal, LPDIRECTDRAWSURFACE7* ppDDSurf, LPDWORD pNumMipMaps)
= reinterpret_cast<HRESULT(__stdcall*)(LPDIRECT3DDEVICE7 pd3dDevice, LPDWORD pFlags, LPDWORD pWidth, LPDWORD pHeight, _D3DX_SURFACEFORMAT * pPixelFormat, LPDIRECTDRAWPALETTE pDDPal, LPDIRECTDRAWSURFACE7 * ppDDSurf, LPDWORD pNumMipMaps)>(0x48E1AC);
HRESULT(__stdcall*D3DXCheckTextureRequirements)(LPDIRECT3DDEVICE7 pd3dDevice, LPDWORD pFlags, LPDWORD pWidth, LPDWORD pHeight, _D3DX_SURFACEFORMAT* pPixelFormat)
= reinterpret_cast<HRESULT(__stdcall*)(LPDIRECT3DDEVICE7 pd3dDevice, LPDWORD pFlags, LPDWORD pWidth, LPDWORD pHeight, _D3DX_SURFACEFORMAT* pPixelFormat)>(0x48DD5B);
HRESULT(__stdcall* D3DXLoadTextureFromSurface)(LPDIRECT3DDEVICE7 pd3dDevice, LPDIRECTDRAWSURFACE7 pTexture, DWORD mipMapLevel, LPDIRECTDRAWSURFACE7 pSurfaceSrc, LPRECT pSrcRect, LPRECT pDestRect, _D3DX_FILTERTYPE filterType)
= reinterpret_cast<HRESULT(__stdcall*)(LPDIRECT3DDEVICE7 pd3dDevice, LPDIRECTDRAWSURFACE7 pTexture, DWORD mipMapLevel, LPDIRECTDRAWSURFACE7 pSurfaceSrc, LPRECT pSrcRect, LPRECT pDestRect, _D3DX_FILTERTYPE filterType)>(0x48FCAE);
HRESULT (__stdcall* D3DXDrawSpriteSimple)(LPDIRECTDRAWSURFACE7 pd3dTexture,
  LPDIRECT3DDEVICE7 pd3dDevice, const D3DXVECTOR3* ppointDest,
  float alpha, float scale, float angleRad,
  const D3DXVECTOR2* pOffset, const RECT* pSourceRect)
= reinterpret_cast<HRESULT (__stdcall*)(LPDIRECTDRAWSURFACE7,
    LPDIRECT3DDEVICE7, const D3DXVECTOR3*,
    float, float, float,
    const D3DXVECTOR2*, const RECT *)>(0x4900CA);
HRESULT (__stdcall*D3DXDrawSprite3D)(LPDIRECTDRAWSURFACE7 pd3dTexture,
  LPDIRECT3DDEVICE7 pd3dDevice,const D3DXVECTOR4 quad[4],float alpha,
  const RECT* pSourceRect)
= reinterpret_cast<HRESULT(__stdcall*)(LPDIRECTDRAWSURFACE7,
    LPDIRECT3DDEVICE7,const D3DXVECTOR4[4], float, const RECT*)>(0x4902CF);
D3DXQUATERNION* (__stdcall*D3DXQuaternionSlerp)(D3DXQUATERNION* pOut,
  const D3DXQUATERNION* pQ1, const D3DXQUATERNION* pQ2, float t)
= reinterpret_cast<D3DXQUATERNION*(__stdcall*)(D3DXQUATERNION*,
  const D3DXQUATERNION*, const D3DXQUATERNION*, float)>(0x48B0D0);
D3DXMATRIX* (__stdcall*D3DXMatrixRotationQuaternion)(D3DXMATRIX* pOut,
  const D3DXQUATERNION* pQ) = reinterpret_cast<D3DXMATRIX* (__stdcall*)(D3DXMATRIX* pOut,
  const D3DXQUATERNION* pQ)>(0x48ACCC);
D3DXMATRIX* (__stdcall*D3DXMatrixTranslation)(D3DXMATRIX* pOut,  float x,  float y,
  float z) = reinterpret_cast<D3DXMATRIX* (__stdcall*)(D3DXMATRIX* pOut,  float x,  float y,
  float z)>(0x48A9C8); 
D3DXMATRIX* (__stdcall*D3DXMatrixScaling)(D3DXMATRIX* pOut,  float sx,  float sy,
  float sz) = reinterpret_cast<D3DXMATRIX* (__stdcall*)(D3DXMATRIX* pOut,  float sx,  float sy,
  float sz)>(0x48A968); 

D3DXVECTOR3* (__stdcall*D3DXVec3TransformCoord)(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM) = reinterpret_cast<D3DXVECTOR3* (__stdcall*)(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM)>(0x48A2A0);
D3DXVECTOR3* (__stdcall* D3DXVec3TransformNormal)(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV, const D3DXMATRIX* pM) = reinterpret_cast<D3DXVECTOR3* (__stdcall*)(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV,  const D3DXMATRIX* pM)>(0x48A38E);

HRESULT(__stdcall* D3DXCreateTextureFromFile)(LPDIRECT3DDEVICE7 pd3dDevice,
  LPDWORD pFlags, LPDWORD pWidth, LPDWORD pHeight,
  D3DX_SURFACEFORMAT* pPixelFormat, LPDIRECTDRAWPALETTE pDDPal,
  LPDIRECTDRAWSURFACE7* ppDDSurf, LPDWORD pNumMipMaps, LPSTR pSrcName,
  D3DX_FILTERTYPE filterType) =
  reinterpret_cast<HRESULT (__stdcall*)(  LPDIRECT3DDEVICE7 pd3dDevice,
    LPDWORD pFlags,  LPDWORD pWidth,  LPDWORD pHeight,
    D3DX_SURFACEFORMAT* pPixelFormat,  LPDIRECTDRAWPALETTE pDDPal,
    LPDIRECTDRAWSURFACE7* ppDDSurf,  LPDWORD pNumMipMaps,  LPSTR pSrcName,
    D3DX_FILTERTYPE filterType)>(0x48E807);


D3DXQUATERNION* (__stdcall* D3DXQuaternionRotationMatrix)(D3DXQUATERNION* pOut,
  const D3DXMATRIX* pM) = reinterpret_cast<D3DXQUATERNION* (__stdcall*)(D3DXQUATERNION* pOut,
    const D3DXMATRIX* pM)>(0x48AEAD);
#endif

// === FUNCTION IMPLEMENATIONS ===

D3DXVECTOR3 *__stdcall hook_D3DXVec3Normalize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV)
{
  // Guard clause: Validate input pointers
  if (!pOut || !pV)
    return NULL;

  ONCELOG("[HNH:SUB] %s\n", "D3DXVec3Normalize_48A1EC");
    
  // Calculate squared length of the vector
  float lengthSq = (pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z);

  // Handle near-zero length vectors to prevent division by zero / NaN
  if (lengthSq <= 1e-10f)
  {
    pOut->x = 0.0f;
    pOut->y = 0.0f;
    pOut->z = 0.0f;
    return pOut;
  }

  // Optimization: If the vector is already normalized (length ~~ 1.0), 
  // simply copy it over if it's not the same pointer.
  const float epsilon = 1e-5f;
  if (lengthSq >= (1.0f - epsilon) && lengthSq <= (1.0f + epsilon))
  {
    if (pOut != pV)
    {
      pOut->x = pV->x;
      pOut->y = pV->y;
      pOut->z = pV->z;
    }
    return pOut;
  }

  // Compute inverse length and scale components
  float invLength = 1.0f / sqrtf(lengthSq);

  pOut->x = pV->x * invLength;
  pOut->y = pV->y * invLength;
  pOut->z = pV->z * invLength;

  return pOut;
}

D3DXMATRIX *__stdcall hook_D3DXMatrixRotationAxis(D3DXMATRIX *pOut, const D3DXVECTOR3 *pV, float angle)
{
  if (!pOut || !pV)
    return NULL;

  ONCELOG("[HNH:SUB] %s\n", "D3DXMatrixRotationAxis_48ABB3");

  // Normalize the rotation axis vector
  D3DXVECTOR3 axis = *pV;
  D3DXVec3Normalize(&axis, &axis);

  float x = axis.x;
  float y = axis.y;
  float z = axis.z;

  float c = cosf(angle);
  float s = sinf(angle);
  float t = 1.0f - c;

  // Precompute component products for matrix clarity
  float xyT = x * y * t;
  float xzT = x * z * t;
  float yzT = y * z * t;

  float xs = x * s;
  float ys = y * s;
  float zs = z * s;

  // Row 1
  pOut->_11 = (x * x * t) + c;
  pOut->_12 = xyT + zs;
  pOut->_13 = xzT - ys;
  pOut->_14 = 0.0f;

  // Row 2
  pOut->_21 = xyT - zs;
  pOut->_22 = (y * y * t) + c;
  pOut->_23 = yzT + xs;
  pOut->_24 = 0.0f;

  // Row 3
  pOut->_31 = xzT + ys;
  pOut->_32 = yzT - xs;
  pOut->_33 = (z * z * t) + c;
  pOut->_34 = 0.0f;

  // Row 4 (Homogeneous coordinates)
  pOut->_41 = 0.0f;
  pOut->_42 = 0.0f;
  pOut->_43 = 0.0f;
  pOut->_44 = 1.0f;

  return pOut;
}

D3DXMATRIX *__stdcall hook_D3DXMatrixMultiply(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2)
{
  ONCELOG("[HNH:SUB] %s\n", "D3DXMatrixMultiply_48A4BC");

  if (!pOut || !pM1 || !pM2)
    return nullptr;
 
  // Use a temporary local matrix to safely handle aliasing 
  // (if pOut points to the same memory as pM1 or pM2)
  D3DXMATRIX tempResult;

  // Access matrices as 2D arrays of floats (4x4)
  const float(*m1)[4] = reinterpret_cast<const float(*)[4]>(pM1);
  const float(*m2)[4] = reinterpret_cast<const float(*)[4]>(pM2);
  float(*out)[4] = reinterpret_cast<float(*)[4]>(&tempResult);

  // Standard 4x4 matrix multiplication: tempResult = m1 * m2
  for (int row = 0; row < 4; ++row)
  {
    for (int col = 0; col < 4; ++col)
    {
      out[row][col] = m1[row][0] * m2[0][col] +
        m1[row][1] * m2[1][col] +
        m1[row][2] * m2[2][col] +
        m1[row][3] * m2[3][col];
    }
  }

  // Copy the final result back to the output pointer safely
  *pOut = tempResult;
  return pOut;

  return pOut;
}

D3DXMATRIX* WINAPI hook_D3DXMatrixInverse(D3DXMATRIX *pOut, float *pfDeterminant, const D3DXMATRIX *pM)
{
  if (!pOut || !pM)
    return 0;

  ONCELOG("[HNH:SUB] %s\n", "D3DXMatrixInverse_48A5BD");

  // 1. Cache source matrix elements
  float m11 = pM->_11, m12 = pM->_12, m13 = pM->_13, m14 = pM->_14;
  float m21 = pM->_21, m22 = pM->_22, m23 = pM->_23, m24 = pM->_24;
  float m31 = pM->_31, m32 = pM->_32, m33 = pM->_33, m34 = pM->_34;
  float m41 = pM->_41, m42 = pM->_42, m43 = pM->_43, m44 = pM->_44;

  // 2. Compute intermediate 2x2 determinants and minors
  float minor_12_12 = m22 * m11 - m21 * m12;
  float invDeta     = m32 * m11 - m31 * m12;
  float minor_14_12 = m42 * m11 - m41 * m12;
  float minor_14_22 = m32 * m21 - m31 * m22;
  float minor_14_32 = m42 * m21 - m41 * m22;
  float minor_14_34 = m42 * m31 - m41 * m32;

  // 3. Compute row 0 cofactors
  float cofactor_00 = minor_14_22 * m13 - invDeta * m23 + minor_12_12 * m33;
  float cofactor_01 = minor_14_12 * m23 - minor_12_12 * m43 - minor_14_32 * m13;
  float cofactor_02 = minor_14_34 * m13 - minor_14_12 * m33 + invDeta * m43;
  float cofactor_03 = minor_14_32 * m33 - minor_14_22 * m43 - minor_14_34 * m23;

  // 4. Compute row 1 cofactors
  float cofactor_10 = invDeta * m24 - minor_12_12 * m34 - minor_14_22 * m14;
  float cofactor_11 = minor_14_32 * m14 - minor_14_12 * m24 + minor_12_12 * m44;
  float cofactor_12 = minor_14_12 * m34 - invDeta * m44 - minor_14_34 * m14;
  float cofactor_13 = minor_14_34 * m24 - minor_14_32 * m34 + minor_14_22 * m44;

  // 5. Additional sub-expressions for rows 2 and 3
  float sub_12_12 = m24 * m13 - m23 * m14;
  float sub_invDet = m34 * m13 - m33 * m14;
  float sub_v15    = m44 * m13 - m43 * m14;

  float sub_v16 = m34 * m23 - m33 * m24;
  float sub_v17 = m44 * m23 - m43 * m24;
  float sub_v19 = m44 * m33 - m43 * m34;

  float minor_34_34 = sub_invDet * m22 - sub_12_12 * m32 - sub_v16 * m12;
  float minor_33_33 = sub_v17 * m12 - sub_v15 * m22 + sub_12_12 * m42;
  float minor_23_34 = sub_v15 * m32 - sub_invDet * m42 - sub_v19 * m12;
  float cofactor_23 = sub_v19 * m22 - sub_v17 * m32 + sub_v16 * m42;

  float pOuta = sub_v16 * m11 - sub_invDet * m21 + sub_12_12 * m31;
  float pMa   = sub_v15 * m21 - sub_12_12 * m41 - sub_v17 * m11;
  float m22_res = sub_v19 * m11 - sub_v15 * m31 + sub_invDet * m41;
  float minor_14_22_b = sub_v17 * m31 - sub_v16 * m41 - sub_v19 * m21;

  // 6. Compute Determinant
  float determinant = minor_23_34 * m21 + minor_33_33 * m31 + minor_34_34 * m41 + cofactor_23 * m11;

  if (pfDeterminant)
    *pfDeterminant = determinant;

  float invDet = 1.0f / determinant;
  if (!_finite(invDet))
    return 0;

  // 7. Populate output inverse matrix
  pOut->_11 = invDet * cofactor_23;
  pOut->_12 = invDet * minor_23_34;
  pOut->_13 = invDet * minor_33_33;
  pOut->_14 = invDet * minor_34_34;

  pOut->_21 = invDet * minor_14_22_b;
  pOut->_22 = invDet * m22_res;
  pOut->_23 = invDet * pMa;
  pOut->_24 = invDet * pOuta;

  pOut->_31 = cofactor_13 * invDet;
  pOut->_32 = cofactor_12 * invDet;
  pOut->_33 = cofactor_11 * invDet;
  pOut->_34 = cofactor_10 * invDet;

  pOut->_41 = cofactor_03 * invDet;
  pOut->_42 = cofactor_02 * invDet;
  pOut->_43 = cofactor_01 * invDet;
  pOut->_44 = cofactor_00 * invDet;

  return pOut;
}

void register_D3DXMatrixIdentity_40E450(int cfg) {
  if (cfg != 2) hnh_HookOff();
  hnh_Hook(0x48A1EC, hook_D3DXVec3Normalize, "D3DXVec3Normalize_48A1EC"); // [OK] 
  hnh_Hook(0x48ABB3, hook_D3DXMatrixRotationAxis, "D3DXMatrixRotationAxis_48ABB3"); // [OK] 
  hnh_Hook(0x48A4BC, hook_D3DXMatrixMultiply, "D3DXMatrixMultiply_48A4BC"); // [OK] 
  hnh_Hook(0x48A5BD, hook_D3DXMatrixInverse, "D3DXMatrixInverse_48A5BD"); // [OK] 
  hnh_HookOn();
}
