#include <stdio.h>
#define _WIN32_WINNT 0x0A00
#include <windows.h>

int main(void)
{
	while (1)
	{
		for (int key = 8; key <= 255; key++)
		{
			SHORT state = GetAsyncKeyState(key);
			if (state & 0x8000)
			{
				if (key == VK_ESCAPE)
					return 0;
				printf("Key %d pressed\n", key);
				Sleep(1000);
			}
		}
		Sleep(10);
	}
}
