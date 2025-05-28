#define _WIN32_WINNT 0x0A00
#define WINVER 0x0A00
#define PSAPI_VERSION 1

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "psapi.lib")

#include <windows.h>
#include <psapi.h>
#include <stdio.h>

void CALLBACK WinForeground(HWINEVENTHOOK hWinEventHook,
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
		if (dwEventThread == 3900 || dwEventThread == 3756)
			return;
		char WindowName[256];
		// GetClassNameA(hwnd, WindowName, sizeof(WindowName)); // Get the class name of the foreground window
		if (WindowName[0] == '\0' || WindowName[1] == '\0') // If the class name is empty, get the window title
			return;
		GetWindowTextA(hwnd, WindowName, sizeof(WindowName)); // Get the title of the foreground window, not the process name
		int i = 0;
		while (WindowName[i] != '\0' && i < 256 - 1)
		{
			if (WindowName[i] == '\n' || WindowName[i] == '\r')
				WindowName[i] = ' ';
			i++;
		}
		printf("Window : %s at %lu since the start of the computer\n", WindowName, dwmsEventTime);
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
