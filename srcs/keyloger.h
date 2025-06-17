#ifndef KEYLOGGER_H
#define KEYLOGGER_H

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

int	    handleKeyPress(t_key *keyData);
void	initKeylogger(t_key *keyData);

#endif