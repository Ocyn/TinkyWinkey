#include "svc.hpp"

int webhookCall(char *log) noexcept
{
    (void)log;
    // TODO : Implement the webhook call using libcurl now it compile

    // HINTERNET hInternet = InternetOpenW(L"MyAgent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    // if (hInternet == NULL)
    // {
    //     DWORD dwError = GetLastError();
    //     _tprintf(_T("InternetOpen failed (%lu)\n"), dwError);
    //     Sleep(1000);
    //     return 1;
    // }
    // HINTERNET hConnect = InternetConnectW(hInternet, L"discord.com",
    //     INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    // if (hConnect == NULL)
    // {
    //     DWORD dwError = GetLastError();
    //     _tprintf(_T("InternetConnect failed (%lu)\n"), dwError);
    //     InternetCloseHandle(hInternet);
    //     Sleep(1000);
    //     return 1;
    // }

    // HINTERNET hRequest = HttpOpenRequestW(hConnect, L"POST",
    //     L"/api/webhooks/1387428910676967575/3aqlKFk5S51ddCHSFPAXX6nahNv5Gxn_G1lTh0wZO_bSMfRPxrIJp-79McJf6H8hFlBn",
    //     NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
    // if (hRequest == NULL)
    // {
    //     DWORD dwError = GetLastError();
    //     _tprintf(_T("HttpOpenRequest failed (%lu)\n"), dwError);
    //     InternetCloseHandle(hConnect);
    //     InternetCloseHandle(hInternet);
    //     Sleep(1000);
    //     return 1;
    // }
    // if (hRequest == NULL)
    // {
    //     DWORD dwError = GetLastError();
    //     _tprintf(_T("HttpOpenRequest failed (%lu)\n"), dwError);
    //     InternetCloseHandle(hConnect);
    //     InternetCloseHandle(hInternet);
    //     Sleep(1000);
    //     return 1;
    // }
    // HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
    // if (GetLastError() != ERROR_SUCCESS)
    // {
    //     DWORD dwError = GetLastError();
    //     _tprintf(_T("HttpSendRequest failed (%lu)\n"), dwError);
    //     InternetCloseHandle(hRequest);
    //     InternetCloseHandle(hConnect);
    //     InternetCloseHandle(hInternet);
    //     Sleep(1000);
    //     return 1;
    // }
    // _tprintf(_T("Calling webhook...\n"));
    // Sleep(1000);

    // InternetCloseHandle(hRequest);
    // InternetCloseHandle(hConnect);
    // InternetCloseHandle(hInternet);
    return 0;
}
