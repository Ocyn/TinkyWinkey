#ifndef WEBHOOK_HPP
#define WEBHOOK_HPP
#define _WIN32_WINNT 0x0A00
#define _WIN32_WINNT_WIN10_RS1 0x0A00
#define _WIN32_WINNT_WIN10_RS3 0x0A03
#define _WIN32_WINNT_WIN10_RS4 0x0A04
#define _WIN32_WINNT_WIN10_TH2 0x0A02

#pragma warning(push)
#pragma warning(disable: 4820) // Disable  C4820: 'bytes' bytes padding added after data member 'member'
#include <Windows.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#pragma warning(pop)
int webhookCall(char *log);

#endif // WEBHOOK_HPP