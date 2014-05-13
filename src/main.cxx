#if defined _WIN32 || defined _WIN64
#include <windows.h>

HINSTANCE hInstance;

extern "C" __declspec(dllexport) BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, void* lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            hInstance = hinstDLL;
            break;

        case DLL_PROCESS_DETACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return true;
}
#else
extern "C" void load() __attribute__((constructor))
{
}

extern "C" void unload() __attribute__((constructor))
{
}
#endif
