// Has all the drawing and text maintenance code.

#include "Shrink.h"
#include "D3DTextOverlay.h"
#include "myIDirect3DDevice9.h"
#include "Config.h"
#include <stdio.h>
#include "GameHandler.h"

ID3DXFont * d3dFont = 0;
IDirect3DDevice9 * oldD3d9dev = 0;
ID3DXSprite * d3dSprite = 0;
ID3DXFont * d3dMiniFont = 0;

// Also takes care of sprite.
void NukeFont() {
	if (d3dSprite) {
		ID3DXSprite * temp = d3dSprite;
		d3dSprite = 0;
		temp->Release();
	}
	if (d3dFont) {
		ID3DXFont * temp = d3dFont;
		d3dFont = 0;
		temp->Release();
	}
	if (d3dMiniFont) {
		ID3DXFont * temp = d3dMiniFont;
		d3dMiniFont = 0;
		temp->Release();
	}
}

struct Line {
	wchar_t *text;
	int start;
	int end;
	D3DCOLOR color;
	D3DCOLOR shadow;
	D3DCOLOR outline;
};

struct Lines {
	Line *lines;
	int numLines;
};

Lines queuedLines = {0,0};
Lines displayedLines = {0,0};

void CleanupLine(Lines &lines, int index) {
	free(lines.lines[index].text);
	if (--lines.numLines) {
		for (int i=index; i<lines.numLines; i++) {
			lines.lines[i] = lines.lines[i+1];
		}
	}
	else {
		free(lines.lines);
		lines.lines = 0;
	}
}

void CleanupAllLines() {
	while (queuedLines.numLines) {
		CleanupLine(queuedLines, 0);
	}
	while (displayedLines.numLines) {
		CleanupLine(displayedLines, 0);
	}
}

void CreateFont(IDirect3DDevice9 *d3d9dev, D3DVIEWPORT9 &d3dvp) {
	if (d3d9dev != oldD3d9dev ||
		(DWORD)state.d3dRect.left   != d3dvp.X ||
		(DWORD)state.d3dRect.right  != d3dvp.X + d3dvp.Width ||
		(DWORD)state.d3dRect.top    != d3dvp.Y ||
		(DWORD)state.d3dRect.bottom != d3dvp.Y + d3dvp.Height ||
		!d3dFont ||
		!d3dMiniFont ||
		!d3dSprite) {
			NukeFont();
			oldD3d9dev = d3d9dev;
			if (D3D_OK != D3DXCreateSprite(d3d9dev, &d3dSprite))
				d3dSprite = 0;
			state.d3dRect.left   = d3dvp.X;
			state.d3dRect.right  = d3dvp.X + d3dvp.Width;
			state.d3dRect.top    = d3dvp.Y;
			state.d3dRect.bottom = d3dvp.Y + d3dvp.Height;

			D3DXFONT_DESCW desc;
			memset(&desc, 0, sizeof(desc));
			state.fontHeight = desc.Height = d3dvp.Height/24;
			if (config.forceFontHeight) {
				state.fontHeight = desc.Height = config.forceFontHeight;
			}
			desc.Width = 0;
			desc.Weight = 400;
			desc.MipLevels = 1;
			desc.Quality = ANTIALIASED_QUALITY;
			desc.PitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
			desc.CharSet = DEFAULT_CHARSET;
			// Should always be true, but just in case....
			if (config.forceFontFace)
				wcsncpy(desc.FaceName, config.forceFontFace, sizeof(desc.FaceName)/sizeof(wchar_t)-1);
			HRESULT res = D3DXCreateFontIndirectW(d3d9dev, &desc, &d3dFont);

			if (res != D3D_OK) d3dFont = 0;
			else {
				// Rather nasty way of calculating width of a space.  Just using " " instead of " ?" returns 0,
				// as spaces at the end of a line are ignored.
				RECT rect2;
				RECT rect3;
				rect2.left = 0;
				rect2.right = 1000;
				rect2.top = 0;
				rect2.bottom = 1000;
				rect3 = rect2;
				d3dFont->DrawTextW(0, L" Test:", -1, &rect2, DT_CALCRECT | DT_LEFT | DT_TOP | DT_WORDBREAK, -1);
				d3dFont->DrawTextW(0, L"Test:", -1, &rect3, DT_CALCRECT | DT_LEFT | DT_TOP | DT_WORDBREAK, -1);
				state.spaceWidth = (rect2.right - rect3.right);
				if (rect2.bottom > state.fontHeight)
					state.fontHeight = rect2.bottom;
				d3dFont->PreloadCharacters(0x20, 0x7F);
			}
			desc.Weight = 800;
			wcscpy(desc.FaceName, L"Arial");
			desc.Height = 16;
			res = D3DXCreateFontIndirectW(d3d9dev, &desc, &d3dMiniFont);
			if (res != D3D_OK) d3dMiniFont = 0;
	}
}

void UpdateText() {
	int t = GetTickCount();
	for (int i=0; i<displayedLines.numLines; ) {
		if (displayedLines.lines[i].end - t < 0) {
			CleanupLine(displayedLines, i);
			continue;
		}
		i++;
	}
	while (queuedLines.numLines && queuedLines.lines[0].start-t <= 0) {
		displayedLines.lines = (Line*) realloc(displayedLines.lines, (displayedLines.numLines+1) * sizeof(Line));
		displayedLines.lines[displayedLines.numLines] = queuedLines.lines[0];
		wchar_t *unformattedText = queuedLines.lines[0].text;
		// For every 2 characters, add at most 5 spaces and a linebreak;
		wchar_t *textStart = (wchar_t*)malloc(2*(1+3*wcslen(unformattedText)));
		wchar_t *text = textStart;
		text[0] = 0;
		int indent = 0;
		while (1) {
			if (indent) while (*unformattedText == ' ' || *unformattedText == '\t') unformattedText++;
			if (!*unformattedText) break;
			int pos = 0;
			int newPos = 0;
			while (1) {
				if (!unformattedText[pos]) break;
				newPos = pos+1+wcscspn(unformattedText + pos+1, L"\r\n \t");
				RECT rect2;
				rect2.left = indent;
				rect2.right = state.d3dRect.right-8;
				if (state.game == SM2) {
					rect2.right -= state.d3dRect.bottom/6;
				}
				rect2.top = 0;
				rect2.bottom = 10000;
				d3dFont->DrawTextW(0, unformattedText, newPos, &rect2, DT_CALCRECT | DT_LEFT | DT_TOP | DT_WORDBREAK, -1);
				if (rect2.bottom > state.fontHeight) {
					// Currently don't wrap long words.  Need to fix later.
					if (!pos) pos = newPos;
					break;
				}
				pos = newPos;
			}
			if (indent) {
				text[-1] = '\n';
				for (int i=0; i<5; i++) {
					text++[0] = ' ';
				}
			}
			else {
				indent = 3*state.spaceWidth;
			}
			memcpy(text, unformattedText, 2*pos);
			text[pos] = 0;
			if (d3dFont) {
				d3dFont->PreloadTextW(text, pos);
			}
			text+=pos+1;
			unformattedText += pos;
		}
		displayedLines.lines[displayedLines.numLines].text = (wchar_t*)realloc(textStart, 2*(1+wcslen(textStart)));
		CleanupLine(queuedLines, 0);
		displayedLines.numLines++;
	}
}

void DisplayMiniTextAtPos(const char* text, RECT *rect, unsigned long color, unsigned long outline, ID3DXSprite * d3dSprite, unsigned long flags) {
	if (d3dMiniFont) {
		for (int w=0; w<4; w++) {
			RECT rect2 = *rect;
			if (w == 0) {
				rect2.right = rect->right+1;
				rect2.left = rect->left+1;
			}
			else if (w==1) {
				rect2.right = rect->right-1;
				rect2.left = rect->left-1;
			}
			else if (w==2) {
				rect2.top = rect->top+1;
				rect2.bottom = rect->bottom+1;
			}
			else if (w==3) {
				rect2.top = rect->top-1;
				rect2.bottom = rect->bottom-1;
			}
			d3dMiniFont->DrawTextA(d3dSprite, text, -1, &rect2, flags, outline);
		}
		d3dMiniFont->DrawTextA(d3dSprite, text, -1, rect, flags, color);
	}
}

int DisplayTextAtPos(wchar_t *text, int x, int y, D3DCOLOR color, D3DCOLOR outline, D3DCOLOR shadow, ID3DXSprite * d3dSprite, unsigned long flags) {
	if (!d3dFont) return 0;
	// Old shadow distance.  Decided to just use 2 in both x and y directions instead.
	//int d = (state.d3dRect.bottom-state.d3dRect.top)/480;
	int i;
	RECT rect2;
	//return 0;
	rect2.left = x;
	rect2.right = x;
	rect2.top = y;
	rect2.bottom = y;
	// Check alpha value, for minor speed optimization.  Raw byte order BGRA, so A is the high order byte.
	if (shadow>>24) {
		for (i = 3; i>0; i--) {
			RECT rect3;
			rect3.right = rect3.left = rect2.left + 2 + i/2;
			rect3.bottom = rect3.top = rect2.top + 2 + i%2;
			d3dFont->DrawTextW(d3dSprite, text, -1, &rect3, flags, shadow);
		}
	}

	if (outline>>24) {
		for (i = 8; i>=0; i--) {
			//if (i==4) continue;
			RECT rect3;
			if (1 == i/3 || 1 == i%3) continue;
			rect3.right = rect3.left = rect2.left + i/3 - 1;
			rect3.bottom = rect3.top = rect2.top + i%3 - 1;
			d3dFont->DrawTextW(d3dSprite, text, -1, &rect3, flags, outline);
		}
	}
	d3dFont->DrawTextW(d3dSprite, text, -1, &rect2, flags, color);
	d3dFont->DrawTextW(d3dSprite, text, -1, &rect2, DT_CALCRECT | flags, color);
	return rect2.bottom;
}

void DisplayText(IDirect3DDevice9 *d3d9dev, D3DVIEWPORT9 &d3dvp) {
	CreateFont(d3d9dev, d3dvp);
	UpdateText();
	if (d3dFont && displayedLines.numLines) {
		if (d3dSprite) {
			d3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
		}
		int height = state.d3dRect.top+2;
		int j;
		for (j=0; j<displayedLines.numLines; j++) {
			height = DisplayTextAtPos(displayedLines.lines[j].text, state.d3dRect.left+4, height, displayedLines.lines[j].color, displayedLines.lines[j].outline, displayedLines.lines[j].shadow, d3dSprite);
		}
		if (d3dSprite) {
			d3dSprite->End();
		}
	}
}

void AddTextEat(int start, int end, wchar_t *text, D3DCOLOR color, D3DCOLOR outline, D3DCOLOR shadow) {
	if (!text[0]) return;
	if (end - start < (int)config.minSubDuration) {
		end = start + config.minSubDuration;
	}
	if (state.displayingHelp) {
		// Clear help text when girl says something.
		CleanupAllLines();
		state.displayingHelp = 0;
	}
	DWORD time = GetTickCount();
	start += time;
	end += time;
	queuedLines.lines = (Line*)realloc(queuedLines.lines, sizeof(Line)*(1+queuedLines.numLines));
	int p = queuedLines.numLines++;
	while (p && queuedLines.lines[p-1].start > start) {
		queuedLines.lines[p] = queuedLines.lines[p-1];
		p--;
	}
	Line *line = queuedLines.lines + p;
	line->start = start;
	line->end = end;
	line->color = color;
	line->outline = outline;
	line->shadow = shadow;
	line->text = text;
}

void AddText(int start, int end, wchar_t *text, D3DCOLOR color, D3DCOLOR outline, D3DCOLOR shadow) {
	AddTextEat(start, end, wcsdup(text), color, outline, shadow);
}

void AddText(int start, int end, char *text, D3DCOLOR color, D3DCOLOR outline, D3DCOLOR shadow) {
	int len = MultiByteToWideChar(CP_UTF8, 0, text, -1, 0, 0);
	if (len > 0) {
		wchar_t *textW = (wchar_t*) malloc(2*len);
		MultiByteToWideChar(CP_UTF8, 0, text, -1, textW, len);
		AddTextEat(start, end, textW, color, outline, shadow);
	}
}
void AddText(int duration, char *text, D3DCOLOR color) {
	AddText(0, duration, text, color, config.defaultOutline[0], config.defaultShadow[0]);
}
void AddText(int duration, wchar_t *text, D3DCOLOR color) {
	AddText(0, duration, text, color, config.defaultOutline[0], config.defaultShadow[0]);
}
void AddText(int start, int end, char *text, D3DCOLOR color) {
	AddText(start, end, text, color, config.defaultOutline[0], config.defaultShadow[0]);
}
void AddText(int start, int end, wchar_t *text, D3DCOLOR color) {
	AddText(start, end, text, color, config.defaultOutline[0], config.defaultShadow[0]);
}

int HandleKeyPress(int c, int control, int shift);

void CheckKeys() {
	static int KeyTimers[256];
	int t = GetTickCount();
	int control = ((unsigned short)GetAsyncKeyState(VK_CONTROL))>>15;
	int shift = ((unsigned short)GetAsyncKeyState(VK_SHIFT))>>15;

	for (int i=' '; i<=VK_OEM_PERIOD; i++) {
		int w = ((unsigned short)GetAsyncKeyState(i))>>15;
		if (!w) KeyTimers[i] = t;
		else {
			if (KeyTimers[i] <= t) {
				HWND h = GetForegroundWindow();
				if (h) {
					DWORD pid;
					DWORD tid = GetWindowThreadProcessId (h, &pid);
					if (pid == GetCurrentProcessId()) {
						KeyTimers[i] = t+200;
						// Use shorter repeat delay for non-control commands.
						if (HandleKeyPress(i, control, shift) && control)
							KeyTimers[i] += 300;
					}
				}
			}
		}
	}
}

HRESULT myIDirect3DDevice9::BeginScene(void) {
	//CheckKeys();
	return(m_pIDirect3DDevice9->BeginScene());
}

HRESULT myIDirect3DDevice9::EndScene(void) {
	D3DVIEWPORT9 d3dvp;
	if (GetViewport(&d3dvp) == D3D_OK && (int)d3dvp.Height == height && (int)d3dvp.Width == width) {
		DisplayText(this, d3dvp);
		if (d3dMiniFont && state.gameHandler) {
			state.gameHandler->DisplayOverlay(d3dFont, d3dMiniFont, d3dSprite);
		}
		CheckKeys();
	}
	return(m_pIDirect3DDevice9->EndScene());
}

ULONG myIDirect3DDevice9::Release(void)
{
	// Not really sure if this is perfect.  Could theoretically be a source of
	// leaks, but not like a whole lot of d3d devices are created/cleaned up.

	if (3 >= --refCount) {
		NukeFont();
		if (refCount<0) refCount = 0;
		refCount = 0;
	}

	ULONG count = m_pIDirect3DDevice9->Release();
	if (!count) {
		NukeFont();
		delete this;
	}
	return count;
}

