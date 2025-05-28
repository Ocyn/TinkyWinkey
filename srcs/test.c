#define _WIN32_WINNT 0x0A00
#define WINVER 0x0A00
#define PSAPI_VERSION 1

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "psapi.lib")

#include <windows.h>
#include <psapi.h>
#include <stdio.h>

void CALLBACK WinForeground(HWINEVENTHOOK hWinEventHook, // Handle to the event hook
	DWORD event, // Event type
	HWND hwnd, // Handle to the window that generated the event
	LONG idObject,	 // Object identifier
	LONG idChild, // Child identifier
	DWORD dwEventThread, // Thread identifier of the thread that generated the event
	DWORD dwmsEventTime) // Time of the event in milliseconds since the system started
{
	(void)hWinEventHook;
	(void)idObject;
	(void)idChild;
	(void)dwEventThread;
	if (event == EVENT_SYSTEM_FOREGROUND)
	{
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid); // Get the process ID of the foreground window

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid); // Open the process to query its information
		if (hProcess)
		{
			char processName[MAX_PATH] = "<unknown>";
			if (GetModuleBaseNameA(hProcess, NULL, processName, sizeof(processName) / sizeof(char))) // Get the name of the process
			{
				if (strcmp(processName, "Explorer.EXE") == 0) // Skip if the process is Explorer.EXE, need to work on it
					return;
				printf("%s at %lu since the start of the computer\n", processName, dwmsEventTime);
			}
			CloseHandle(hProcess);
		}
	}
}

int main(void)
{
	HWINEVENTHOOK hook = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
		NULL,
		WinForeground,
		0, 0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	if (hook == NULL)
	{
		printf("Failed to set hook: %lu\n", GetLastError());
		return 1;
	}
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
		;
	UnhookWinEvent(hook);
	return 0;
}
