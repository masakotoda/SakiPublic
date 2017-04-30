// SakiInjector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>

#ifndef MAKEULONGLONG
#define MAKEULONGLONG(ldw, hdw) ((ULONGLONG(hdw) << 32) | ((ldw) & 0xFFFFFFFF))
#endif

#ifndef MAXULONGLONG
#define MAXULONGLONG ((ULONGLONG)~((ULONGLONG)0))
#endif

DWORD GetMainThreadId(DWORD dwProcID)
{
	DWORD dwMainThreadID = 0;
	ULONGLONG ullMinCreateTime = MAXULONGLONG;

	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap != INVALID_HANDLE_VALUE) {
		THREADENTRY32 th32;
		th32.dwSize = sizeof(THREADENTRY32);
		BOOL bOK = TRUE;
		for (bOK = Thread32First(hThreadSnap, &th32); bOK;
		bOK = Thread32Next(hThreadSnap, &th32)) {
			if (th32.th32OwnerProcessID == dwProcID) {
				HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION,
					TRUE, th32.th32ThreadID);
				if (hThread) {
					FILETIME afTimes[4] = { 0 };
					if (GetThreadTimes(hThread,
						&afTimes[0], &afTimes[1], &afTimes[2], &afTimes[3])) {
						ULONGLONG ullTest = MAKEULONGLONG(afTimes[0].dwLowDateTime,
							afTimes[0].dwHighDateTime);
						if (ullTest && ullTest < ullMinCreateTime) {
							ullMinCreateTime = ullTest;
							dwMainThreadID = th32.th32ThreadID; // let it be main... :)
						}
					}
					CloseHandle(hThread);
				}
			}
		}
#ifndef UNDER_CE
		CloseHandle(hThreadSnap);
#else
		CloseToolhelp32Snapshot(hThreadSnap);
#endif
	}
	return dwMainThreadID;
}

int main()
{
	while (true)
	{
		int pid;
		std::cout << "Enter pid to crash (enter 0 to exit): ";
		std::cin >> pid;

		TCHAR buffer[_MAX_PATH]{};
		::GetModuleFileName(0, buffer, _MAX_PATH);
		std::wstring path = buffer;
		auto pos = path.rfind('\\');
		path = path.substr(0, pos) + L"\\SakiParasite.dll";

		HMODULE dll = LoadLibrary(path.c_str());
		if (!dll)
		{
			std::cout << "The DLL could not be found.\n";
			continue;
		}

		HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "CrashHook");
		if (!addr)
		{
			std::cout << "Function was not found.\n";
			continue;
		}

		DWORD threadId = GetMainThreadId(pid);
		if (threadId == 0)
		{
			std::cout << "Unabled to get main thread of the specified pid.\n";
			continue;
		}

		HHOOK handle = SetWindowsHookEx(WH_KEYBOARD, addr, dll, threadId);
		if (!handle)
		{
			std::cout << "We can't hook the keyboard event to the specified pid. (Note we can't hook to console app.)\n";
			continue;
		}

		std::cout << "Now hit a key on the victim application...\n";

		std::cout << "Enter any key to unhook and exit.\n";
		char unhook = 0;
		std::cin >> unhook;
		UnhookWindowsHookEx(handle);
		break;
	}

	return 0;
}
