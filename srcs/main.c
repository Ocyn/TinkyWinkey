#include "logs.h"

HANDLE fd = NULL;

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
	(void)dwmsEventTime;

	if (event == EVENT_SYSTEM_FOREGROUND)
		get_foreground_window(hwnd);
}

int already_running(void)
{
	HANDLE mutex = CreateMutex(NULL, FALSE, "Global\\TinkyWinkeyMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(mutex);
		return 1;
	}
	return 0;
}

int WINAPI	WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	(void)hInstance;
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nShowCmd;
	fd = CreateFileW(L"logs.txt", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (fd != INVALID_HANDLE_VALUE)
		SetFilePointer(fd, 0, NULL, FILE_END);
	else
		return 1;

	if (already_running())
		return 1;
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
	return MessageBox(NULL, "hello, world", "caption", 0);
}
