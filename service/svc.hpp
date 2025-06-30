#ifndef SVC_HPP
#define SVC_HPP

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0A00
#define _WIN32_WINNT_WIN10_RS1 0x0A00
#define _WIN32_WINNT_WIN10_RS3 0x0A03
#define _WIN32_WINNT_WIN10_RS4 0x0A04
#define _WIN32_WINNT_WIN10_TH2 0x0A02

#define UNICODE
#define _UNICODE

#pragma warning(push)
#pragma warning(disable: 4820) // Padding added after data member, this warning can't be removed
#pragma warning(disable: 5039) // Pointeur ou reference pouvant throw excpetion, cette erreur (inexistante) ne peut pas être supprimée des .h windows
#include <windows.h>
#include <wtsapi32.h>
#include <tchar.h>
#include <strsafe.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>
#include <cstring>
#include <winhttp.h>
#include <stdio.h>
#pragma warning(pop)

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Wtsapi32.lib")


// Main service function
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv) noexcept;
VOID WINAPI ServiceCtrlHandler(DWORD) noexcept;
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam) noexcept;

// Service utility functions
DWORD FindTargetPID(const wchar_t* targetName) noexcept;
int InstallService() noexcept;
int StartService() noexcept;
int StopService() noexcept;
int DeleteService() noexcept;
int _tmain(int argc, TCHAR *argv[]);
int webhookCall(char *log) noexcept;

// Global variables for service control (declarations)
extern SERVICE_STATUS g_ServiceStatus;
extern SERVICE_STATUS_HANDLE g_StatusHandle;
extern HANDLE g_ServiceStopEvent;
extern PROCESS_INFORMATION g_ProcessInfo;

// Service name definition
#define SERVICE_NAME  _T("### Tinky ###") // TODO: Change the service name
#define SERVICE_DISPLAY_NAME  _T("### Tinky ###") // TODO: Change the display name

#endif
