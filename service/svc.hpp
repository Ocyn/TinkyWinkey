#ifndef SVC_HPP
#define SVC_HPP

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdio.h>

#pragma comment(lib, "Advapi32.lib")

// Main service function
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);


// Global variables for service control
SERVICE_STATUS g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;
PROCESS_INFORMATION g_ProcessInfo = {0};

// Service name definition
#define SERVICE_NAME _T("svc_fdp")

#endif
