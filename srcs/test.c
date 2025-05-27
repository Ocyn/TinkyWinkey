#define _WIN32_WINNT 0x0A00
#define WINVER 0x0A00
#include <windows.h>
#include <stdio.h>

void printForegroundProcessName(HWND hwnd)
{
	if (!hwnd)
		return;

	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	if (!pid)
		return;

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (!hProcess)
		return;

	char name[MAX_PATH] = "<unknown>";
	if (GetModuleBaseNameA(hProcess, NULL, name, sizeof(name) / sizeof(char)))
		printf("Foreground process: %s\n", name);

	CloseHandle(hProcess);
}

void CALLBACK WinForeground(HWINEVENTHOOK hWinEventHook,
	DWORD event,
	HWND hwnd,
	LONG idObject,
	LONG idChild,
	DWORD dwEventThread,
	DWORD dwmsEventTime)
{
	if (event == EVENT_SYSTEM_FOREGROUND)
	{
		char WindowName[256];
		GetClassNameA(hwnd, WindowName, sizeof(WindowName));
		printf("Foreground window changed to: %s\n at %lu since the start of the computer", WindowName, dwmsEventTime);

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
