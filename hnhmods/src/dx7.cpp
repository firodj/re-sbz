#include "dx7.hpp"
#include "console.hpp"

#include <cmath>
#include <corecrt_math_defines.h>

// === GLOBAL FUNCTIONS ===
HRESULT (*D3DXUninitialize)(void) = reinterpret_cast<HRESULT(*)(void)>(0x48B8F0);
HRESULT (*D3DXInitialize)(void) = (HRESULT(*)(void))0x48B80B; 
DWORD (*D3DXGetDeviceCount)(void) = (DWORD(*)(void))0x48B9DD;
DWORD (__stdcall *D3DXGetMaxNumVideoModes)(DWORD deviceIndex, DWORD flags) = (DWORD (__stdcall*)(DWORD, DWORD))0x48BBF0; 
HRESULT (__stdcall *D3DXGetDeviceDescription)(DWORD deviceIndex, D3DX_DEVICEDESC* pd3dxDeviceDesc) = (HRESULT(__stdcall*)(DWORD, D3DX_DEVICEDESC*))0x48BA9E; 
HRESULT (__stdcall *D3DXGetVideoMode)(DWORD deviceIndex, DWORD flags, DWORD modeIndex, D3DX_VIDMODEDESC* pModeList) = (HRESULT (__stdcall*)(DWORD, DWORD, DWORD, D3DX_VIDMODEDESC*))0x48BCDE;
HRESULT (__stdcall *D3DXGetDeviceCaps)(DWORD deviceIndex, D3DX_VIDMODEDESC* pVidMode, D3DDEVICEDESC7* pD3DCaps, DDCAPS* pDDHALCaps, DDCAPS* pDDHELCaps) = (HRESULT (__stdcall*)(DWORD, D3DX_VIDMODEDESC*, D3DDEVICEDESC7*, DDCAPS*, DDCAPS*))0x48C28D; 

HRESULT (__stdcall* D3DXCreateContextEx)(unsigned int devIndex, unsigned int flags, HWND hwnd, HWND focusWnd, int nNumColorBits, int nNumAlphaBits, int nNumDepthBits, int nNumStencilBits, int nNumBackBuffers, int nWidth, int nHeight, int nRefreshRate, LPD3DXCONTEXT* ppCtx)
= reinterpret_cast<HRESULT  (__stdcall*)(unsigned int devIndex, unsigned int flags, HWND hwnd, HWND focusWnd, int nNumColorBits, int nNumAlphaBits, int nNumDepthBits, int nNumStencilBits, int nNumBackBuffers, int nWidth, int nHeight, int nRefreshRate, LPD3DXCONTEXT *ppCtx)>(0x48C5ED);

HRESULT(__stdcall* D3DXCreateContext)(unsigned int devIndex, unsigned int flags, HWND hWnd, int width, int height, LPD3DXCONTEXT* ppCtx)
= reinterpret_cast<HRESULT(__stdcall*)(unsigned int devIndex, unsigned int flags, HWND hWnd, int width, int height, LPD3DXCONTEXT * ppCtx)>(0x48C3D6);

HRESULT(__stdcall* D3DXGetSurfaceFormat)(int devIndex, D3DX_VIDMODEDESC* pVidMode, unsigned int surfClassFlags, unsigned int which, D3DX_SURFACEFORMAT* pFormat)
= reinterpret_cast<HRESULT (__stdcall*)(int devIndex, D3DX_VIDMODEDESC *pVidMode, unsigned int surfClassFlags, unsigned int which, D3DX_SURFACEFORMAT *pFormat)>(0x48C09A);

DWORD(__stdcall* D3DXGetMaxSurfaceFormats)(int devIndex, _DWORD* pVidMode, int surfClassFlags)
= reinterpret_cast<DWORD (__stdcall*)(int devIndex, _DWORD *pVidMode, int surfClassFlags)>(0x48BF99);

D3DXVECTOR3 * (__stdcall * D3DXVec3Normalize)(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV) = reinterpret_cast<D3DXVECTOR3 *(__stdcall*)(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV)>(0x48A1EC);
D3DXMATRIX * (__stdcall * D3DXMatrixRotationAxis)(D3DXMATRIX *pOut, const D3DXVECTOR3 *pV, float angle) = reinterpret_cast<D3DXMATRIX *(__stdcall*)(D3DXMATRIX *pOut, const D3DXVECTOR3 *pV, float angle)>(0x48ABB3);
double (__cdecl * Vec3LengthSq)(const D3DXVECTOR3 *pV) = reinterpret_cast<double(__cdecl*)(const D3DXVECTOR3 *pV)>(0x40E3C0);
void (__thiscall * Vec3Set)(D3DXVECTOR3 *This, float x, float y, float z) = reinterpret_cast<void(__thiscall*)(D3DXVECTOR3 *This, float x, float y, float z)>(0x40E420);
double (__cdecl * Vec3Dot)(const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2) = reinterpret_cast<double(__cdecl*)(const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)>(0x40E3F0);
double (__cdecl * DegreesToRadians)(float deg) = reinterpret_cast<double(__cdecl*)(float deg)>(0x40DDF3);
HRESULT (__cdecl * MatrixPerspectiveFovLH)(D3DXMATRIX *pMat, float fovy, float aspect, float zn, float zf) = reinterpret_cast<HRESULT(__cdecl*)(D3DXMATRIX *pMat, float fovy, float aspect, float zn, float zf)>(0x40CB94);
D3DXMATRIX * (__cdecl * D3DXMatrixIdentity)(D3DXMATRIX *pOut) = reinterpret_cast<D3DXMATRIX *(__cdecl*)(D3DXMATRIX *pOut)>(0x40E450);
D3DXMATRIX * (__stdcall * D3DXMatrixMultiply)(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2) = reinterpret_cast<D3DXMATRIX *(__stdcall*)(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2)>(0x48A4BC);
HRESULT (__cdecl * MatrixLookAtLH)(D3DXMATRIX *pMat, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp) = reinterpret_cast<HRESULT(__cdecl*)(D3DXMATRIX *pMat, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp)>(0x40C64A);
D3DXMATRIX * (__stdcall * D3DXMatrixInverse)(D3DXMATRIX *pOut, float *pfDeterminant, const D3DXMATRIX *pM) = reinterpret_cast<D3DXMATRIX *(__stdcall*)(D3DXMATRIX *pOut, float *pfDeterminant, const D3DXMATRIX *pM)>(0x48A5BD);

// === FUNCTION IMPLEMENATIONS ===

D3DXVECTOR3 *__stdcall hook_D3DXVec3Normalize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV)
{
  // Guard clause: Validate input pointers
  if (!pOut || !pV)
    return NULL;

  // Logging
  static int log_D3DXVec3Normalize_48A1EC = 1;
  log_D3DXVec3Normalize_48A1EC--;
  if (!log_D3DXVec3Normalize_48A1EC) {
    DebugLog("[HNH:SUB] %s\n", "D3DXVec3Normalize_48A1EC");
    log_D3DXVec3Normalize_48A1EC = 10000;
  }

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
  DebugLog("[HNH:SUB] %s\n", "D3DXMatrixRotationAxis_48ABB3");

  if (!pOut || !pV)
    return NULL;

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

double __cdecl hook_Vec3LengthSq(const D3DXVECTOR3 *pV)
{
  DebugLog("[HNH:SUB] Vec3LengthSq_40E3C0\n");
  return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;
}

void __fastcall hook_Vec3Set(D3DXVECTOR3 *This, int unused_edx, float x, float y, float z)
{
  DebugLog("[HNH:SUB] hook_Vec3Set_40E420\n");
  This->Vec3Set(x, y, z);
}

double __cdecl hook_Vec3Dot(const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)
{
  DebugLog("[HNH:SUB] Vec3Dot_40E3F0\n");
  return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

double __cdecl hook_DegreesToRadians(float deg)
{
  float sign = 1.0f;
  float accumulatedPi = 0.0f;

  DebugLog("[HNH:SUB] %s\n", "DegreesToRadians_40DDF3");

  if (deg < 0.0f)
  {
    sign = -1.0f;
    deg = -deg;
  }

  while (deg > 180.0f)
  {
    deg -= 180.0f;
    accumulatedPi += (float)M_PI;
  }

  return (deg * (float)(M_PI / 180.0) + accumulatedPi) * sign;
}

HRESULT __cdecl hook_MatrixPerspectiveFovLH(D3DXMATRIX *pMat, float fovy, float aspect, float zn, float zf)
{
  double X; // st7
  float v7; // [esp+18h] [ebp-24h]
  float v8; // [esp+20h] [ebp-1Ch]
  float v9; // [esp+30h] [ebp-Ch]
  float v10; // [esp+34h] [ebp-8h]
  float v11; // [esp+38h] [ebp-4h]

  DebugLog("[HNH:SUB] %s\n", "MatrixPerspectiveFovLH_40CB94");

  if ( fabs(zf - zn) < 0.009999999776482582 )
    return E_INVALIDARG;
  X = sin(fovy / 2.0);
  if ( fabs(X) < 0.009999999776482582 )
    return E_INVALIDARG;
  v8 = sin(fovy / 2.0);
  v10 = cos(fovy / 2.0) / v8 * aspect;
  v7 = sin(fovy / 2.0);
  v9 = cos(fovy / 2.0) / v7 * 1.0;
  v11 = zf / (zf - zn);
  memset(pMat, 0, sizeof(D3DXMATRIX));
  pMat->_11 = v10;
  pMat->_22 = v9;
  pMat->_33 = v11;
  pMat->_34 = 1.0;
  pMat->_43 = -v11 * zn;
  return S_OK;
}

D3DXMATRIX *__cdecl hook_D3DXMatrixIdentity(D3DXMATRIX *pOut)
{
  D3DXMATRIX *result; // eax

  static int log_D3DXMatrixIdentity = 1;
  log_D3DXMatrixIdentity--;
  if (!log_D3DXMatrixIdentity) {
    DebugLog("[HNH:SUB] %s\n", "D3DXMatrixIdentity_40E450");
    log_D3DXMatrixIdentity = 1000;
  }

  result = pOut;
  pOut->m[3][2] = 0.0;
  pOut->m[3][1] = 0.0;
  pOut->m[3][0] = 0.0;
  pOut->m[2][3] = 0.0;
  pOut->m[2][1] = 0.0;
  pOut->m[2][0] = 0.0;
  pOut->m[1][3] = 0.0;
  pOut->m[1][2] = 0.0;
  pOut->m[1][0] = 0.0;
  pOut->m[0][3] = 0.0;
  pOut->m[0][2] = 0.0;
  pOut->m[0][1] = 0.0;
  pOut->m[3][3] = 1.0;
  pOut->m[2][2] = 1.0;
  pOut->m[1][1] = 1.0;
  pOut->m[0][0] = 1.0;
  return result;
}

LPVOID orig_D3DXMatrixMultiply = NULL;

D3DXMATRIX *__stdcall hook_D3DXMatrixMultiply(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2)
{
  if (!pOut || !pM1 || !pM2)
    return nullptr;

  // logging
  static int log_D3DXMatrixMultiply_48A4BC = 1;
  log_D3DXMatrixMultiply_48A4BC--;
  if (!log_D3DXMatrixMultiply_48A4BC) {
    DebugLog("[HNH:SUB] %s\n", "D3DXMatrixMultiply_48A4BC");
    log_D3DXMatrixMultiply_48A4BC = 10000;
  }

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

HRESULT __cdecl hook_MatrixLookAtLH(D3DXMATRIX *pMat, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp)
{
  float lenSq;
  float invLen;
  float dotProduct;
  D3DXVECTOR3 forward; // Formerly v60
  D3DXVECTOR3 up;      // Formerly v64
  D3DXVECTOR3 right;   // Formerly v61
  D3DXVECTOR3 tempVec;

  DebugLog("[HNH:SUB] %s\n", "MatrixLookAtLH_40C64A");

  // --- 1. Compute and normalize the forward vector (Z-axis) ---
  float dx = pAt->x - pEye->x;
  float dy = pAt->y - pEye->y;
  float dz = pAt->z - pEye->z;

  Vec3Set(&tempVec, dx, dy, dz);
  forward = tempVec;

  lenSq = Vec3LengthSq(&forward);
  invLen = sqrt(lenSq);
  if (invLen < 0.000001f)
    return E_INVALIDARG;

  float forwardLenInv = 1.0f / invLen;
  forward.x *= forwardLenInv;
  forward.y *= forwardLenInv;
  forward.z *= forwardLenInv;

  // --- 2. Orthogonalize the up vector (Y-axis) with fallbacks ---
  dotProduct = forward.z * pUp->z + forward.y * pUp->y + forward.x * pUp->x;
  Vec3Set(&tempVec, dotProduct * forward.x, dotProduct * forward.y, dotProduct * forward.z);

  Vec3Set(&up, pUp->x - tempVec.x, pUp->y - tempVec.y, pUp->z - tempVec.z);
  lenSq = Vec3LengthSq(&up);
  invLen = sqrt(lenSq);

  if (invLen < 0.000001f)
  {
    // Fallback 1: Try alignment using Y world axis
    float cy = forward.y;
    Vec3Set(&tempVec, forward.x * cy, forward.y * cy, forward.z * cy);
    Vec3Set(&up, 0.0f - tempVec.x, 1.0f - tempVec.y, 0.0f - tempVec.z);
    lenSq = Vec3LengthSq(&up);
    invLen = sqrt(lenSq);

    if (invLen < 0.000001f)
    {
      // Fallback 2: Try alignment using Z world axis
      float cz = forward.z;
      Vec3Set(&tempVec, forward.x * cz, forward.y * cz, forward.z * cz);
      Vec3Set(&up, 0.0f - tempVec.x, 0.0f - tempVec.y, 1.0f - tempVec.z);
      lenSq = Vec3LengthSq(&up);
      invLen = sqrt(lenSq);

      if (invLen < 0.000001f)
        return E_INVALIDARG;
    }
  }

  float upLenInv = 1.0f / invLen;
  up.x *= upLenInv;
  up.y *= upLenInv;
  up.z *= upLenInv;

  // --- 3. Compute the right vector (X-axis) using cross product (up x forward) ---
  right.x = up.y * forward.z - up.z * forward.y;
  right.y = up.z * forward.x - up.x * forward.z;
  right.z = up.x * forward.y - up.y * forward.x;

  // --- 4. Assemble the final view matrix ---
  D3DXMatrixIdentity(pMat);

  // Rotation components
  pMat->_11 = right.x;   pMat->_12 = up.x;   pMat->_13 = forward.x;
  pMat->_21 = right.y;   pMat->_22 = up.y;   pMat->_23 = forward.y;
  pMat->_31 = right.z;   pMat->_32 = up.z;   pMat->_33 = forward.z;

  // Translation components (dot product with eye position)
  pMat->_41 = Vec3Dot(pEye, &right);
  pMat->_42 = Vec3Dot(pEye, &up);
  pMat->_43 = Vec3Dot(pEye, &forward);

  return S_OK;
}

LPVOID orig_D3DXMatrixInverse = NULL;

D3DXMATRIX* WINAPI hook_D3DXMatrixInverse(D3DXMATRIX *pOut, float *pfDeterminant, const D3DXMATRIX *pM)
{
  if (!pOut || !pM)
    return 0;

  static int log_D3DXMatrixInverse_48A5BD = 1;
  log_D3DXMatrixInverse_48A5BD--;
  if (!log_D3DXMatrixInverse_48A5BD) {
    DebugLog("[HNH:SUB] %s\n", "D3DXMatrixInverse_48A5BD");
    log_D3DXMatrixInverse_48A5BD = 10000;
  }

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

LPVOID hnh_Hook(DWORD dwAddr, LPVOID pDetour, const char* pszName) {
  LPVOID addr = (LPVOID)dwAddr;
  LPVOID orig = NULL;
  MH_STATUS res = MH_CreateHook(addr, pDetour, reinterpret_cast<LPVOID*>(&orig));
  if (res == MH_OK) res = MH_EnableHook(addr);
  DebugLog("[HNH] Hooked %s; %s\n", pszName, MH_StatusToString(res));
  return orig;
}

void register_MatrixLookAtLH_40C64A() {
  hnh_Hook(0x40C64A, hook_MatrixLookAtLH, "MatrixLookAtLH_40C64A");
  hnh_Hook(0x48A1EC, hook_D3DXVec3Normalize, "D3DXVec3Normalize_48A1EC");
  hnh_Hook(0x48ABB3, hook_D3DXMatrixRotationAxis, "D3DXMatrixRotationAxis_48ABB3");
  hnh_Hook(0x40E3C0, hook_Vec3LengthSq, "Vec3LengthSq_40E3C0");
  hnh_Hook(0x40E420, hook_Vec3Set, "hook_Vec3Set_40E420");
  hnh_Hook(0x40E3F0, hook_Vec3Dot, "Vec3Dot_40E3F0");
  hnh_Hook(0x40DDF3, hook_DegreesToRadians, "DegreesToRadians_40DDF3");
  hnh_Hook(0x40CB94, hook_MatrixPerspectiveFovLH, "MatrixPerspectiveFovLH_40CB94");
  hnh_Hook(0x40E450, hook_D3DXMatrixIdentity, "D3DXMatrixIdentity_40E450");

  orig_D3DXMatrixMultiply = hnh_Hook(0x48A4BC, hook_D3DXMatrixMultiply, "D3DXMatrixMultiply_48A4BC");
  orig_D3DXMatrixInverse = hnh_Hook(0x48A5BD, hook_D3DXMatrixInverse, "D3DXMatrixInverse_48A5BD");

}
