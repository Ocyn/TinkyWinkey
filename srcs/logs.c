#include "logs.h"

HANDLE fd = NULL;

int	write_to_file(char *str)
{
	if (fd == NULL)
	{
		printf("File handle is NULL\n");
		return -1;
	}
	DWORD bytesWritten;
	if (!WriteFile(fd, str, (DWORD)strlen(str), &bytesWritten, NULL))
	{
		printf("Failed to write to file: %lu\n", GetLastError());
		return -1;
	}
	return 0;
}

char	*GetDateFormated(void)
{
	LPSYSTEMTIME lpSystemTime = (LPSYSTEMTIME)malloc(sizeof(SYSTEMTIME));
	if (lpSystemTime == NULL)
		return NULL;

	GetLocalTime(lpSystemTime);
	if (lpSystemTime == NULL)
		return NULL;

	char *dateStr = (char *)malloc(22);
	if (dateStr == NULL)
	{
		free(lpSystemTime);
		return NULL;
	}

	sprintf_s(dateStr, 22, "%d.%d.%d %d:%d:%d",
		lpSystemTime->wDay, lpSystemTime->wMonth, lpSystemTime->wYear,
		lpSystemTime->wHour, lpSystemTime->wMinute, lpSystemTime->wSecond);
	free(lpSystemTime);
	return dateStr;
}

void CALLBACK WinEvent(HWINEVENTHOOK hWinEventHook, // Handle to the event hook
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
	{
		char windowTitle[256];
		if (GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle)) == 0)
			return;

		char *dateStr = GetDateFormated();
		if (dateStr == NULL)
			return;

		int i = 0;
		while (windowTitle[i] != '\0' && i < 256 - 1)
		{
			if (windowTitle[i] == '\r' || windowTitle[i] == '\n')
				windowTitle[i] = ' ';
			i++;
		}
		windowTitle[i] = '\0';
		char logEntry[512];
		sprintf_s(logEntry, sizeof(logEntry), "[%s] - Foreground window title: %s\n", dateStr, windowTitle);
		write_to_file(logEntry);
		free(dateStr);
	}
	// TODO Event KeyPress
}

int	main(void)
{
	fd = CreateFileA("logs.txt", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (fd != INVALID_HANDLE_VALUE)
		SetFilePointer(fd, 0, NULL, FILE_END);
	else
		return 1;

	HWINEVENTHOOK hook = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
		NULL,
		WinEvent,
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
