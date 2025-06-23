#include "svc.hpp"

int InstallService()
{
	return 0;
}

int StartService()
{
	return 0;
}

int StopService()
{
	return 0;
}

int DeleteService()
{
	return 0;
}

int _tmain(int arc, TCHAR *argv[])
{
	if (arc < 2)
	{
		_tprintf(_T("Usage: %s <options>"), argv[0]);
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
