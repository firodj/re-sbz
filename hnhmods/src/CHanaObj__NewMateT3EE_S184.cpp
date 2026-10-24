#include "dx7.hpp"
#include "console.hpp"
#include "shared.hpp"

void(__cdecl* CHanaObj__NewMateT3EE_S184)(CHanaMateT3EE_S184** ppOutMate) = reinterpret_cast<void(__cdecl*)(CHanaMateT3EE_S184**)>(0x40F720);

// CHanaObj__NewMateT3EE_S184 at 0x40F720
// Creates and initializes a new CHanaMateT3EE_S184 object (size 0x184 bytes)

void __cdecl hook_CHanaObj__NewMateT3EE_S184_40F720(CHanaMateT3EE_S184 **ppOutMate)
{
    CHanaMateT3EE_S184 *pMate;

    DebugLog("[HNH:SUB] %s\n", "CHanaObj__NewMateT3EE_S184_40F720");

    // Allocate memory (0x184 bytes = 388 decimal)
    pMate = (CHanaMateT3EE_S184 *)MemAlloc_40DEF7(0x184u);
    memset(pMate, 0, sizeof(CHanaMateT3EE_S184));

    // Set type identifier
    pMate->dwType = 0x3EE;  // 1006 decimal

    // Initialize D3D material to white diffuse/ambient, black specular/emissive
    pMate->d3dMaterial_130.diffuse.r = 1.0f;
    pMate->d3dMaterial_130.diffuse.g = 1.0f;
    pMate->d3dMaterial_130.diffuse.b = 1.0f;
    pMate->d3dMaterial_130.diffuse.a = 1.0f;

    pMate->d3dMaterial_130.ambient.r = 1.0f;
    pMate->d3dMaterial_130.ambient.g = 1.0f;
    pMate->d3dMaterial_130.ambient.b = 1.0f;
    pMate->d3dMaterial_130.ambient.a = 1.0f;

    // Specular and emissive set to black (0)
    *(DWORD *)&pMate->d3dMaterial_130.specular.r = 0;  // r,g = 0
    *(DWORD *)&pMate->d3dMaterial_130.specular.b = 0;  // b,a = 0
    *(DWORD *)&pMate->d3dMaterial_130.emissive.r = 0;  // r,g = 0
    *(DWORD *)&pMate->d3dMaterial_130.emissive.b = 0;  // b,a = 0

    pMate->d3dMaterial_130.power = 0.0f;

    // Cache diffuse alpha and mark material as dirty
    pMate->fDiffuseA = pMate->d3dMaterial_130.diffuse.a;
    pMate->fMatDirty = 1;

    // Increment reference count
    CHanaObj__AddRef_40BFC3(pMate);

    // Return the new object
    *ppOutMate = pMate;
}

// Register hook if needed
void register_CHanaObj__NewMateT3EE_S184_40F720() {
    hnh_Hook(0x40F720, hook_CHanaObj__NewMateT3EE_S184_40F720, "CHanaObj__NewMateT3EE_S184_40F720");
}