#include <stdlib.h>
#include "pp.h"

struct Sub {
	char *fileName;
	char *text;
	int start;
	int end;
};

struct FileInfo {
	__int64 offset;
	char *name;
	wchar_t *ppName;
	Sub *subs;
	Sub *extraSub;
	int numSubs;
	int size;
	const FormatData *format;
};

void LoadConfigSubs(int display=0, int *subbed=0, int *total=0);
void LoadSubs(wchar_t *file, int display = 0, int *subbed=0, int *total=0, int *skippedLines=0, int *loadedLines=0);
void LoadSubsFromPPs(wchar_t *path, wchar_t **files, int numFiles);

void ClearSubs();
FileInfo *GetSubsAndHeaderInfo(wchar_t *ppFile, __int64 startOffset);
Sub *GetSub(char *name);
void SaveSubs(FILE *out);

