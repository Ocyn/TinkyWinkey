#include "svc.hpp"

int _tmain(int arc, TCHAR *argv[])
{
	if (arc < 2)
	{
		_tprintf(_T("Usage: %s <options>"), argv[0]);
		return 1;
	}
	if (_tcscmp(argv[1], _T("install")) == 0)
	{
		_tprintf(_T("Installing service..."));
	}
	else if (_tcscmp(argv[1], _T("start")) == 0)
	{
		_tprintf(_T("Starting service..."));
	}
	else if (_tcscmp(argv[1], _T("stop")) == 0)
	{
		_tprintf(_T("Stopping service..."));
	}
	else if (_tcscmp(argv[1], _T("delete")) == 0)
	{
		_tprintf(_T("Deleting service..."));
	}
	else
	{
		_tprintf(_T("Unknown option: %s"), argv[1]);
		return 1;
	}
}
