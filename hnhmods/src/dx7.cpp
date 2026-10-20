#include "dx7.hpp"
#include "console.hpp"

#include <cmath>
#include <corecrt_math_defines.h>

// === GLOBAL FUNCTIONS ===
HRESULT (*D3DXUninitialize)(void) = reinterpret_cast<HRESULT(*)(void)>(0x48B8F0);
HRESULT (*D3DXInitialize)(void) = (HRESULT(*)(void))0x48B80B; 
DWORD (*D3DXGetDeviceCount)(void) = (DWORD(*)(void))0x48B9DD;
DWORD (*D3DXGetMaxNumVideoModes)(DWORD deviceIndex, DWORD flags) = (DWORD (*)(DWORD, DWORD))0x48BBF0; 
HRESULT (*D3DXGetDeviceDescription)(DWORD deviceIndex, D3DX_DEVICEDESC* pd3dxDeviceDesc) = (HRESULT(*)(DWORD, D3DX_DEVICEDESC*))0x48BA9E; 
HRESULT (*D3DXGetVideoMode)(DWORD deviceIndex, DWORD flags, DWORD modeIndex, D3DX_VIDMODEDESC* pModeList) = (HRESULT (*)(DWORD, DWORD, DWORD, D3DX_VIDMODEDESC*))0x48BCDE;
HRESULT (*D3DXGetDeviceCaps)(DWORD deviceIndex, D3DX_VIDMODEDESC* pVidMode, D3DDEVICEDESC7* pD3DCaps, DDCAPS* pDDHALCaps, DDCAPS* pDDHELCaps) = (HRESULT (*)(DWORD, D3DX_VIDMODEDESC*, D3DDEVICEDESC7*, DDCAPS*, DDCAPS*))0x48C28D; 

// === FUNCTION IMPLEMENATIONS ===

D3DXVECTOR3 *__stdcall D3DXVec3Normalize(D3DXVECTOR3 *pOut, const D3DXVECTOR3 *pV)
{
  DebugLog("[HNH:SUB] %s\n", "D3DXVec3Normalize_48A1EC");

  // Guard clause: Validate input pointers
  if (!pOut || !pV)
    return NULL;

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

D3DXMATRIX *__stdcall D3DXMatrixRotationAxis(D3DXMATRIX *pOut, const D3DXVECTOR3 *pV, float angle)
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

double __cdecl Vec3LengthSq(const D3DXVECTOR3 *pV)
{
  DebugLog("[HNH:SUB] Vec3LengthSq_40E3C0\n");
  return pV->x * pV->x + pV->y * pV->y + pV->z * pV->z;
}

void __cdecl Vec3Set(D3DXVECTOR3 *This, float x, float y, float z)
{
  This->x = x;
  This->y = y;
  This->z = z;
}

void __fastcall hook_Vec3Set(D3DXVECTOR3 *This, int unused_edx, float x, float y, float z)
{
  DebugLog("[HNH:SUB] hook_Vec3Set_40E420\n");
  Vec3Set(This, x, y, z);
}

double __cdecl Vec3Dot(const D3DXVECTOR3 *pV1, const D3DXVECTOR3 *pV2)
{
  DebugLog("[HNH:SUB] Vec3Dot_40E3F0\n");
  return pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;
}

double __cdecl DegreesToRadians(float deg)
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

HRESULT __cdecl MatrixPerspectiveFovLH(D3DXMATRIX *pMat, float fovy, float aspect, float zn, float zf)
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

D3DXMATRIX *__cdecl D3DXMatrixIdentity(D3DXMATRIX *pOut)
{
  D3DXMATRIX *result; // eax

  DebugLog("[HNH:SUB] %s\n", "D3DXMatrixIdentity_40E450");

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

D3DXMATRIX *__stdcall D3DXMatrixMultiply(D3DXMATRIX *pOut, const D3DXMATRIX *pM1, const D3DXMATRIX *pM2)
{
  DebugLog("[HNH:SUB] %s\n", "D3DXMatrixMultiply_48A4BC");

  // Guard clause: Validate input pointers
  if (!pOut || !pM1 || !pM2)
    return NULL;

  D3DXMATRIX tempResult;
  const D3DXMATRIX *sourceM2 = pM2;

  // Handle matrix aliasing (if output overlaps with input M2)
  if (pM2 == pOut)
  {
    if (pM1 == pOut)
    {
      // If output overlaps with both, copy M1 first or handle safely
      // Standard DX implementation handles this via a temporary buffer
    }
    qmemcpy(&tempResult, pM2, sizeof(tempResult));
    sourceM2 = &tempResult;
  }

  // Perform standard 4x4 matrix multiplication (Row-major / DirectX convention)
  // pOut = pM1 * sourceM2
  for (int row = 0; row < 4; ++row)
  {
    for (int col = 0; col < 4; ++col)
    {
      tempResult.m[row][col] = 
        pM1->m[row][0] * sourceM2->m[0][col] +
        pM1->m[row][1] * sourceM2->m[1][col] +
        pM1->m[row][2] * sourceM2->m[2][col] +
        pM1->m[row][3] * sourceM2->m[3][col];
    }
  }

  // Copy the computed result back to pOut (safe if pOut == pM1)
  qmemcpy(pOut, &tempResult, sizeof(*pOut));

  return pOut;
}

HRESULT __cdecl MatrixLookAtLH(D3DXMATRIX *pMat, const D3DXVECTOR3 *pEye, const D3DXVECTOR3 *pAt, const D3DXVECTOR3 *pUp)
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

D3DXMATRIX *__stdcall D3DXMatrixInverse(D3DXMATRIX *pOut, float *pfDeterminant, const D3DXMATRIX *pM)
{
  DebugLog("[HNH:SUB] %s\n", "D3DXMatrixInverse_48A5BD");

  if (!pOut || !pM)
    return NULL;

  // Extract matrix elements for readability
  float a00 = pM->_11, a01 = pM->_12, a02 = pM->_13, a03 = pM->_14;
  float a10 = pM->_21, a11 = pM->_22, a12 = pM->_23, a13 = pM->_24;
  float a20 = pM->_31, a21 = pM->_32, a22 = pM->_33, a23 = pM->_34;
  float a30 = pM->_41, a31 = pM->_42, a32 = pM->_43, a33 = pM->_44;

  // Compute 2x2 sub-determinant cross-products
  float b00 = a00 * a11 - a01 * a10;
  float b01 = a00 * a12 - a02 * a10;
  float b02 = a00 * a13 - a03 * a10;
  float b03 = a01 * a12 - a02 * a11;
  float b04 = a01 * a13 - a03 * a11;
  float b05 = a02 * a13 - a03 * a12;

  float b06 = a20 * a31 - a21 * a30;
  float b07 = a20 * a32 - a22 * a30;
  float b08 = a20 * a33 - a23 * a30;
  float b09 = a21 * a32 - a22 * a31;
  float b10 = a21 * a33 - a23 * a31;
  float b11 = a22 * a33 - a23 * a32;

  // Calculate the 4x4 determinant
  float det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

  if (pfDeterminant)
    *pfDeterminant = det;

  // Check if the matrix is invertible (non-zero determinant & finite)
  float invDet = 1.0f / det;
  if (!_finite(invDet))
    return NULL;

  // Compute inverse matrix elements via adjugate matrix scaled by 1 / determinant
  D3DXMATRIX temp;

  // Row 1
  temp._11 = (a11 * b11 - a12 * b10 + a13 * b09) * invDet;
  temp._12 = (a03 * b10 - a01 * b11 - a02 * b09) * invDet;
  temp._13 = (a31 * b05 - a32 * b04 + a33 * b03) * invDet;
  temp._14 = (a23 * b04 - a21 * b05 - a22 * b03) * invDet;

  // Row 2
  temp._21 = (a12 * b08 - a10 * b11 - a13 * b07) * invDet;
  temp._22 = (a00 * b11 - a02 * b08 + a03 * b07) * invDet;
  temp._23 = (a32 * b02 - a30 * b05 - a33 * b01) * invDet;
  temp._24 = (a20 * b05 - a22 * b02 + a23 * b01) * invDet;

  // Row 3
  temp._31 = (a10 * b10 - a11 * b08 + a13 * b06) * invDet;
  temp._32 = (a01 * b08 - a00 * b10 - a03 * b06) * invDet;
  temp._33 = (a30 * b04 - a31 * b02 + a33 * b00) * invDet;
  temp._34 = (a21 * b02 - a20 * b04 - a23 * b00) * invDet;

  // Row 4
  temp._41 = (a11 * b07 - a10 * b09 - a12 * b06) * invDet;
  temp._42 = (a00 * b09 - a01 * b07 + a02 * b06) * invDet;
  temp._43 = (a31 * b01 - a30 * b03 - a32 * b00) * invDet;
  temp._44 = (a20 * b03 - a21 * b01 + a22 * b00) * invDet;

  *pOut = temp;
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
  hnh_Hook(0x40C64A, MatrixLookAtLH, "MatrixLookAtLH_40C64A");
  hnh_Hook(0x48A1EC, D3DXVec3Normalize, "D3DXVec3Normalize_48A1EC");
  hnh_Hook(0x48ABB3, D3DXMatrixRotationAxis, "D3DXMatrixRotationAxis_48ABB3");
  hnh_Hook(0x40E3C0, Vec3LengthSq, "Vec3LengthSq_40E3C0");
  hnh_Hook(0x40E420, hook_Vec3Set, "hook_Vec3Set_40E420");
  hnh_Hook(0x40E3F0,  Vec3Dot, "Vec3Dot_40E3F0");
  hnh_Hook(0x40DDF3, DegreesToRadians, "DegreesToRadians_40DDF3");
  hnh_Hook(0x40CB94, MatrixPerspectiveFovLH, "MatrixPerspectiveFovLH_40CB94");
  hnh_Hook(0x40E450, D3DXMatrixIdentity, "D3DXMatrixIdentity_40E450");
  hnh_Hook(0x48A4BC, D3DXMatrixMultiply, "D3DXMatrixMultiply_48A4BC");
  hnh_Hook(0x48A5BD, D3DXMatrixInverse, "D3DXMatrixInverse_48A5BD");
}
