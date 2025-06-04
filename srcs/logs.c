#include "logs.h"

char	*GetDateFormated(void)
{
	LPSYSTEMTIME lpSystemTime = (LPSYSTEMTIME)malloc(sizeof(SYSTEMTIME));
	if (lpSystemTime == NULL)
	{
		printf("Memory allocation failed\n");
		return NULL;
	}
	GetLocalTime(lpSystemTime);
	if (lpSystemTime == NULL)
	{
		printf("Failed to get local time: %lu\n", GetLastError());
		return NULL;
	}
	char *dateStr = (char *)malloc(22);
	if (dateStr == NULL)
	{
		printf("Memory allocation failed\n");
		free(lpSystemTime);
		return NULL;
	}
	sprintf_s(dateStr, 22, "%d.%d.%d %d:%d:%d",
		lpSystemTime->wDay, lpSystemTime->wMonth, lpSystemTime->wYear,
		lpSystemTime->wHour, lpSystemTime->wMinute, lpSystemTime->wSecond);
	free(lpSystemTime);
	return dateStr;
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
	{
		char windowTitle[256];
		if (GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle)) == 0)
			return;

		char *dateStr = GetDateFormated();
		if (dateStr == NULL)
		{
			printf("Failed to get date string\n");
			return;
		}

		int i = 0;
		while (windowTitle[i] != '\0' && i < 256 - 1)
		{
			if (windowTitle[i] == '\r' || windowTitle[i] == '\n')
				windowTitle[i] = ' ';
			i++;
		}
		windowTitle[i] = '\0';
		printf("[%s] - Foreground window title: %s\n", dateStr, windowTitle);
		free(dateStr);
	}
}

int	main(void)
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
