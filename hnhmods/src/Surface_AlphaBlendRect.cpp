#include "dx7.hpp"

#include "console.hpp"
#include "shared.hpp"

// Surface_AlphaBlendRect_41A039 -- software alpha-blend of a rect between
// locked DirectDraw surfaces (used for the scene-background crossfade).
// out = ((100-nBlendWeight)*dst + nBlendWeight*src) / 100 per channel,
// channel masks = g_dwGBitMask / g_dwRBitMask / g_dwRGBBitMask (G/R/B) so
// the same code serves 565/555/888. Bit depth from g_dwPixelFormat:
// 16 (WORD stores), 24 (WORD + byte stores), else 32 (DWORD stores).
// bSkipColorKey & 1: source pixels equal to the surface's DD color key
// (GetColorKey type 8) are left untouched. Same-surface blends lock once
// and unlock once; two-surface blends lock src (Pointer) + dst (pSrcRow)
// and unlock both. Pitches come from each GetSurfaceDesc's lPitch /
// g_dwBytesPerPixel, then scaled to bytes per depth. The 16/24-bit paths
// READ the source as a DWORD (one pixel beyond the rect on the last
// column -- harmless, all channel masks fit the low bits; kept faithful).

// === PINNED GAME FUNCTIONS ===
void (__cdecl*Surface_AlphaBlendRect)(LPDIRECTDRAWSURFACE7 pDestSurface, int nDestX, int nDestY,
    LPDIRECTDRAWSURFACE7 pSrcSurface, LPRECT pSrcRect, int nBlendWeight, char bSkipColorKey) =
    reinterpret_cast<void(__cdecl*)(LPDIRECTDRAWSURFACE7, int, int, LPDIRECTDRAWSURFACE7,
      LPRECT, int, char)>(0x41A039);
int (__cdecl*Surface_LockAndGetPointer)(LPDIRECTDRAWSURFACE7 pSurface) =
    reinterpret_cast<int(__cdecl*)(LPDIRECTDRAWSURFACE7)>(0x4196FD);



// === FUNCTIONS ===
// per-channel blend: ((100-w)*dst + w*src)/100, channels isolated by the
// G/R/B bit masks (same expression the original inlines six times)
static DWORD BlendPixel(DWORD dwDst, DWORD dwSrc, int nW, int nInvW)
{
    return (g_dwGBitMask & ((nInvW * (g_dwGBitMask & dwDst) + nW * (g_dwGBitMask & dwSrc)) / 100))
         | (g_dwRBitMask & ((nInvW * (g_dwRBitMask & dwDst) + nW * (g_dwRBitMask & dwSrc)) / 100))
         | (g_dwRGBBitMask & ((nInvW * (g_dwRGBBitMask & dwDst) + nW * (g_dwRGBBitMask & dwSrc)) / 100));
}

void __cdecl hook_Surface_AlphaBlendRect(LPDIRECTDRAWSURFACE7 pDestSurface, int nDestX, int nDestY,
                                         LPDIRECTDRAWSURFACE7 pSrcSurface, LPRECT pSrcRect,
                                         int nBlendWeight, char bSkipColorKey)
{
    DDSURFACEDESC2 ddsd;                   // [ebp-84h]
    DDCOLORKEY colorKey;                   // [ebp-A0h]
    BYTE *pSrcBase, *pDstBase;             // locked pointers
    DWORD dwSrcPitch, dwDstPitch;          // byte pitches
    DWORD dwKey;                           // key color (low bits per depth)
    DWORD dwSrc, dwDst, dwBlend;
    UINT nBpp;                             // bytes per pixel: 2/3/4
    int nInvBlendWeight, nRectHeight, nRectWidthBytes;
    int nSrcLeftBytes, nDestLeftBytes;
    int i, y;
    BOOL bSameSurface;

    DebugLog("[HNH:SUB] Surface_AlphaBlendRect_41A039\n");

    bSameSurface = 0;
    nInvBlendWeight = 100 - nBlendWeight;

    if (pSrcSurface == pDestSurface)
    {
        ddsd.dwSize = sizeof(DDSURFACEDESC2);              // 124
        pSrcSurface->GetSurfaceDesc(&ddsd);
        dwDstPitch = dwSrcPitch = (DWORD)ddsd.lPitch / g_dwBytesPerPixel;
        pSrcSurface->GetColorKey(DDCKEY_SRCBLT /*8*/, &colorKey);
        pSrcBase = (BYTE *)Surface_LockAndGetPointer(pSrcSurface);
        pDstBase = pSrcBase;                               // one lock
        bSameSurface = 1;
    }
    else
    {
        ddsd.dwSize = sizeof(DDSURFACEDESC2);
        pDestSurface->GetSurfaceDesc(&ddsd);
        dwDstPitch = (DWORD)ddsd.lPitch / g_dwBytesPerPixel;
        ddsd.dwSize = sizeof(DDSURFACEDESC2);
        pSrcSurface->GetSurfaceDesc(&ddsd);
        dwSrcPitch = (DWORD)ddsd.lPitch / g_dwBytesPerPixel;
        pSrcSurface->GetColorKey(DDCKEY_SRCBLT, &colorKey);
        pSrcBase = (BYTE *)Surface_LockAndGetPointer(pSrcSurface);
        pDstBase = (BYTE *)Surface_LockAndGetPointer(pDestSurface);
    }

    nRectHeight = pSrcRect->bottom - pSrcRect->top;
    dwKey = colorKey.dwColorSpaceLowValue;

    if (g_dwPixelFormat == 16)
    {
        nBpp = 2;
        nRectWidthBytes = 2 * (pSrcRect->right - pSrcRect->left);
        nSrcLeftBytes   = 2 * pSrcRect->left;
        nDestLeftBytes  = 2 * nDestX;
        dwSrcPitch *= 2;
        dwDstPitch *= 2;
        // (the original's dwColorKeyFlags = 6/8 dance is dead)

        for (y = 0; y < nRectHeight; ++y)
        {
            BYTE *pSrc = pSrcBase + (pSrcRect->top + y) * dwSrcPitch + nSrcLeftBytes;
            BYTE *pDst = pDstBase + (nDestY + y) * dwDstPitch + nDestLeftBytes;
            for (i = 0; i < nRectWidthBytes; i += 2)
            {
                // source is READ as a DWORD in the original (straddles into
                // the next pixel; the masks keep only the current one)
                dwSrc = *(DWORD *)(pSrc + i);
                if ((bSkipColorKey & 1) && *(WORD *)(pSrc + i) == (WORD)dwKey)
                    continue;                              // key pixel: skip
                dwDst = *(DWORD *)(pDst + i);
                *(WORD *)(pDst + i) = (WORD)BlendPixel(dwDst, dwSrc, nBlendWeight, nInvBlendWeight);
            }
        }
    }
    else if (g_dwPixelFormat == 24)
    {
        nRectWidthBytes = 3 * (pSrcRect->right - pSrcRect->left);
        nSrcLeftBytes   = 3 * pSrcRect->left;
        nDestLeftBytes  = 3 * nDestX;
        dwSrcPitch *= 3;
        dwDstPitch *= 3;
        dwKey &= 0xFFFFFF;                                 // 24-bit key

        for (y = 0; y < nRectHeight; ++y)
        {
            BYTE *pSrc = pSrcBase + (pSrcRect->top + y) * dwSrcPitch + nSrcLeftBytes;
            BYTE *pDst = pDstBase + (nDestY + y) * dwDstPitch + nDestLeftBytes;
            for (i = 0; i < nRectWidthBytes; i += 3)
            {
                dwSrc = *(DWORD *)(pSrc + i) & 0xFFFFFF;    // dword read (see above)
                if ((bSkipColorKey & 1) && dwSrc == dwKey)
                    continue;
                dwDst = *(DWORD *)(pDst + i);
                // 24-bit store: low WORD + third byte
                dwBlend = BlendPixel(dwDst, dwSrc, nBlendWeight, nInvBlendWeight);
                *(WORD *)(pDst + i) = (WORD)dwBlend;
                *(BYTE *)(pDst + i + 2) = (BYTE)(dwBlend >> 16);
            }
        }
    }
    else                                                   // 32-bit
    {
        nRectWidthBytes = 4 * (pSrcRect->right - pSrcRect->left);
        nSrcLeftBytes   = 4 * pSrcRect->left;
        nDestLeftBytes  = 4 * nDestX;
        dwSrcPitch *= 4;
        dwDstPitch *= 4;

        for (y = 0; y < nRectHeight; ++y)
        {
            BYTE *pSrc = pSrcBase + (pSrcRect->top + y) * dwSrcPitch + nSrcLeftBytes;
            BYTE *pDst = pDstBase + (nDestY + y) * dwDstPitch + nDestLeftBytes;
            for (i = 0; i < nRectWidthBytes; i += 4)
            {
                if ((bSkipColorKey & 1) && *(DWORD *)(pSrc + i) == dwKey)
                    continue;
                dwSrc = *(DWORD *)(pSrc + i);
                dwDst = *(DWORD *)(pDst + i);
                *(DWORD *)(pDst + i) = BlendPixel(dwDst, dwSrc, nBlendWeight, nInvBlendWeight);
            }
        }
    }

    pSrcSurface->Unlock(NULL);
    if (!bSameSurface)
        pDestSurface->Unlock(NULL);
}

void register_Surface_AlphaBlendRect() {
    hnh_Hook(0x41A039, hook_Surface_AlphaBlendRect, "Surface_AlphaBlendRect_41A039");
}
