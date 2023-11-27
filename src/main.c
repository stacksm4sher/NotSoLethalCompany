#include <windows.h>

#include "util.h"

HINSTANCE pHinstance;

BOOL WINAPI
DllMain(HINSTANCE hinstDLL,  // handle to DLL module
        DWORD fdwReason,     // reason for calling function
        LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);

        pHinstance = hinstDLL;

        HANDLE thread = CreateThread(NULL, 0, Hook, 0, 0, NULL);

        if (thread)
        {
            CloseHandle(thread);
        }
    }

    return TRUE;
}
