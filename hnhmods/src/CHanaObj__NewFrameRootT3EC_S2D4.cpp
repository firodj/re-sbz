#include "dx7.hpp"
#include "console.hpp"
#include "shared.hpp"

void(__cdecl* CHanaObj__NewFrameRootT3EC_S2D4)(CHanaFrameRootT3EC_S2D4 **ppOutFrameRoot) = reinterpret_cast<void(__cdecl*)(CHanaFrameRootT3EC_S2D4**)>(0x404908);

// CHanaObj__NewFrameRootT3EC_S2D4 at 0x404908
// Creates and initializes a new CHanaFrameRootT3EC_S2D4 object (size 0x2D4 bytes)

void __cdecl hook_CHanaObj__NewFrameRootT3EC_S2D4_404908(CHanaFrameRootT3EC_S2D4 **ppOutFrameRoot)
{
    CHanaFrameRootT3EC_S2D4 *pFrameRoot;

    DebugLog("[HNH:SUB] %s\n", "CHanaObj__NewFrameRootT3EC_S2D4_404908");

    // Allocate memory (0x2D4 bytes = 724 decimal)
    pFrameRoot = (CHanaFrameRootT3EC_S2D4*) MemAlloc_40DEF7(0x2D4u);
    memset(pFrameRoot, 0, sizeof(CHanaFrameRootT3EC_S2D4));

    // Set type identifier
    pFrameRoot->dwType = 0x3EC;  // 1004 decimal

    // Initialize three 4x4 matrices to identity
    // mat4x4_fmrx (frame matrix?)
    pFrameRoot->mat4x4_fmrx._11 = 1.0f;
    pFrameRoot->mat4x4_fmrx._22 = 1.0f;
    pFrameRoot->mat4x4_fmrx._33 = 1.0f;
    pFrameRoot->mat4x4_fmrx._44 = 1.0f;
    // Off-diagonal elements zeroed by memset, explicit zero stores:
    // m[3][1], m[2][3], m[2][0], m[1][2], m[0][3], m[0][1] = 0

    // mat4x4_after
    pFrameRoot->mat4x4_after._11 = 1.0f;
    pFrameRoot->mat4x4_after._22 = 1.0f;
    pFrameRoot->mat4x4_after._33 = 1.0f;
    pFrameRoot->mat4x4_after._44 = 1.0f;

    // mat4x4_before
    pFrameRoot->mat4x4_before._11 = 1.0f;
    pFrameRoot->mat4x4_before._22 = 1.0f;
    pFrameRoot->mat4x4_before._33 = 1.0f;
    pFrameRoot->mat4x4_before._44 = 1.0f;

    // Initialize root frame flags
    pFrameRoot->bIsRoot_1F8 = 1;      // This is a root frame
    pFrameRoot->nCountChild = 0;      // No children initially
    pFrameRoot->nCount_12C++;         // Increment reference/instance count

    // Return the new object
    *ppOutFrameRoot = pFrameRoot;
}

// Register hook if needed
void register_CHanaObj__NewFrameRootT3EC_S2D4_404908() {
    hnh_Hook(0x404908, hook_CHanaObj__NewFrameRootT3EC_S2D4_404908, "CHanaObj__NewFrameRootT3EC_S2D4_404908");
}