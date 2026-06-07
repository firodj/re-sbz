#include "Shrink.h"
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
// Note:  No longer in use, but keeping it around just in case.


// Use different names for release and debug version.
#ifndef _DEBUG
#define DLL_NAME "AG3Overlay.dll"
#else
#define DLL_NAME "AG3Overlay.dll"
#endif

// Not used, but may be handy in the future.
unsigned long GetTargetThreadIdFromProcname(char *procName)
{
   PROCESSENTRY32 pe;
   HANDLE thSnapshot, hProcess;
   BOOL retval, ProcFound = false;
   unsigned long pTID, threadID;

   thSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

   if(thSnapshot == INVALID_HANDLE_VALUE)
   {
      MessageBox(NULL, "Error: unable to create toolhelp snapshot", "Loader", NULL);
      return false;
   }

   pe.dwSize = sizeof(PROCESSENTRY32);

    retval = Process32First(thSnapshot, &pe);

   while(retval)
   {
      if(!stricmp(pe.szExeFile, procName) )
      {
         ProcFound = true;
         break;
      }

      retval    = Process32Next(thSnapshot,&pe);
      pe.dwSize = sizeof(PROCESSENTRY32);
   }

   CloseHandle(thSnapshot);

   _asm {
      mov eax, fs:[0x18]
      add eax, 36
      mov [pTID], eax
   }

   hProcess = OpenProcess(PROCESS_VM_READ, false, pe.th32ProcessID);
   ReadProcessMemory(hProcess, (const void *)pTID, &threadID, 4, NULL);
   CloseHandle(hProcess);

   return threadID;
}

// Not used, but may be handy in the future.
unsigned long GetTargetProcessIdFromProcname(char *procName)
{
   PROCESSENTRY32 pe;
   HANDLE thSnapshot;
   BOOL retval, ProcFound = false;
   thSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if(thSnapshot == INVALID_HANDLE_VALUE)
   {
      MessageBox(NULL, "Error: unable to create toolhelp snapshot", "Loader", NULL);
      return false;
   }
   pe.dwSize = sizeof(PROCESSENTRY32);
   retval = Process32First(thSnapshot, &pe);
   while(retval)
   {
      if(!stricmp(pe.szExeFile, procName) )
      {
         ProcFound = true;
         break;
      }
      retval    = Process32Next(thSnapshot,&pe);
      pe.dwSize = sizeof(PROCESSENTRY32);
   }
   return pe.th32ProcessID;
}

#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | \
                              PROCESS_QUERY_INFORMATION | \
                              PROCESS_VM_OPERATION | \
                              PROCESS_VM_WRITE | \
                              PROCESS_VM_READ \
                             )

int RunAndInject(wchar_t *exe) {
	if (!exe) return 0;
	STARTUPINFOW startInfo;
	PROCESS_INFORMATION procInfo;
	memset(&startInfo, 0, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
	int out = 0;
	if (CreateProcessW(exe, 0, 0, 0, 0, 0, 0, 0, &startInfo, &procInfo)) {
		Sleep(1);
		//SuspendThread(procInfo.hThread);
		HANDLE hThread;

		void *RemoteString = (LPVOID)VirtualAllocEx(procInfo.hProcess,
											 NULL,
											 strlen(DLL_NAME),
											 MEM_RESERVE|MEM_COMMIT,
											 PAGE_READWRITE
											);
		if (RemoteString &&
			WriteProcessMemory(procInfo.hProcess, (LPVOID)RemoteString, DLL_NAME,strlen(DLL_NAME), NULL) &&
			(hThread = CreateRemoteThread(procInfo.hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, (LPVOID)RemoteString, 0,0))) {
			out = 1;
			// Make sure it's injected before resuming thread.
			// Needs to be injected before Direct3d objects are created.
			WaitForSingleObject(hThread, 3000);
			CloseHandle(hThread);
		}
		//ResumeThread(procInfo.hThread);
	}
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);
	return out;
}

wchar_t *knownExes[] = {
	L"AG3_Play.exe",
	L"JS3_Play.exe",

	L"Sexy Beach 3 Plus!.exe"
	L"SexyBeach3Plus.exe",
	L"Sexy Beach 3.exe",
	L"Sexy Beach 3 Upgraded.exe",

	L"RapeLay English.exe",
	L"RapeLay.exe",

	L"HF-Hako(Trial).exe",
	L"HakoTrial.exe",
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//SetEnvironmentVariable("__COMPAT_LAYER", "#ApplicationLocale");
	//SetEnvironmentVariable("AppLocaleID", "0411");
	wchar_t *exe = 0;
	wchar_t *commandLine = GetCommandLineW();
	int argc;
	int i;
	wchar_t **argv = CommandLineToArgvW(commandLine, &argc);
	if (argc >= 2) {
		if (!RunAndInject(argv[1])) {
			// Currently do nothing.
		}
	}
	else {
		for (i=0; i<sizeof(knownExes)/sizeof(knownExes[0]); i++) {
			if (RunAndInject(knownExes[i])) {
				break;
			}
		}
		if (i == sizeof(knownExes)/sizeof(knownExes[0])) {
			// Currently do nothing.
		}
	}
	LocalFree(argv);
	return 0;
}

// Old debugging code.  Injects dll into already running version of Alamar's exe.
/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HMODULE dll = LoadLibrary(DLL_NAME);
	HOOKPROC cbtProc;
	unsigned long loadLibAddy = (unsigned long)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	if (dll && (cbtProc = (HOOKPROC)GetProcAddress(dll, "CBTProc"))) {
		STARTUPINFO startInfo;
		PROCESS_INFORMATION procInfo;
		memset(&startInfo, 0, sizeof(startInfo));
		startInfo.cb = sizeof(startInfo);
		if (CreateProcess("AG3_Play.subtitle_v1.5.3.exe", 0, 0, 0, 0, 0, 0, 0, &startInfo, &procInfo)) {
	   HANDLE hProc = procInfo.hProcess;
	   void *LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

	   void *RemoteString = (LPVOID)VirtualAllocEx(hProc,
											 NULL,
											 strlen(DLL_NAME),
											 MEM_RESERVE|MEM_COMMIT,
											 PAGE_READWRITE
											);
	   WriteProcessMemory(hProc, (LPVOID)RemoteString, DLL_NAME,strlen(DLL_NAME), NULL);
	   CreateRemoteThread(hProc,
						  NULL,
						  NULL,
						  (LPTHREAD_START_ROUTINE)LoadLibAddy,
						  (LPVOID)RemoteString,
						  NULL,
						  NULL
						 );

			CloseHandle(procInfo.hThread);
			CloseHandle(procInfo.hProcess);
		}
	}
	FreeLibrary(dll);
	return 0;
}
//*/