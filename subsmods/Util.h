// Miscellaneous functions used in a number of files.

#pragma once

// Takes format because it assumes input is encrypted.
// Makes it easier to use from overlay code, where I don't
// want to makea complete unencrypted version, since I need
// to pass it on to the exe.
int GetPlayLen(char *lpBuffer, int size, const FormatData *);

int ExtensionIs(const char *name, const char *ext);
int ExtensionIsW(const wchar_t *name, const wchar_t *ext);

// Means I don't have to link to winsock.
unsigned long myhtonl(unsigned long x);

wchar_t* JISToRomanji(wchar_t *string);
wchar_t* JISNameToRomanji(wchar_t *string);

int StripFunkyASCII(wchar_t *string);
int AddFunkyASCII(wchar_t *string);

