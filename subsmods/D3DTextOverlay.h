// Note:  File name is because D3D10 support may eventually be needed.
#pragma once

#include "Shrink.h"

#include <Windows.h>
#include <d3dx9.h>

int DisplayTextAtPos(wchar_t *text, int x, int y, D3DCOLOR color, D3DCOLOR outline, D3DCOLOR shadow, ID3DXSprite * d3dSprite=0, unsigned long flags = DT_LEFT | DT_TOP | DT_NOCLIP);
void DisplayMiniTextAtPos(const char* text, RECT *rect, unsigned long color, unsigned long outline, ID3DXSprite * d3dSprite=0, unsigned long flags = DT_LEFT | DT_TOP | DT_NOCLIP);

void AddText(int duration, wchar_t *text, D3DCOLOR color);
void AddText(int start, int end, wchar_t *text, D3DCOLOR color);
void AddText(int start, int end, wchar_t *text, D3DCOLOR color, D3DCOLOR outline, D3DCOLOR shadow);

void AddText(int duration, char *text, D3DCOLOR color);
void AddText(int start, int end, char *text, D3DCOLOR color);
void AddText(int start, int end, char *text, D3DCOLOR color, D3DCOLOR outline, D3DCOLOR shadow);

void CleanupAllLines();
void NukeFont();