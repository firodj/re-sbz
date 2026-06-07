#pragma once
// Needed for a thing or two.

#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include <stdio.h>

// Simple way of ensuring a clean build before release.

#define OVERLAY_NAME "Illusion Subtitle Overlay v1.0.0"
#define WAVE_MONITOR_NAME "Illusion Wave Monitor v1.0.0"
#define PP_EXTRACTOR_NAME "PP Extractor v1.0.0"

/*
inline void DebugOut(char *s) {
	FILE *out = fopen("Debug.txt", "ab");
	fprintf(out, "%s\r\n", s);
	fclose(out);
}
//*/
#ifndef _DEBUG
	#if (_MSC_VER<1300)
		#pragma comment(linker,"/RELEASE")
		#pragma comment(linker,"/opt:nowin98")
	#endif
#endif
