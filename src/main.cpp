#ifdef _MSC_VER
#pragma comment(lib, "libtesseract302-static.lib")
#pragma comment(lib, "liblept168.lib")
#endif

#include "stdafx.h"

void FreeUTF8Text()
{
    for (auto it = UTF8TextGC.begin(); it != UTF8TextGC.end();)
    {
        if (std::get<0>(*it) == GetCurrentThreadId()) //std::this_thread::get_id()
        {
            std::cout<<"Freeing Tesseract UTF8-Text Pointer: "<<static_cast<void*>(&(std::get<1>(*it))[0])<<"\n";
            it = UTF8TextGC.erase(it);
        }
        else
            ++it;
    }
}

void FreeTesseractInstance()
{
    for (auto it = TesseractInstanceGC.begin(); it != TesseractInstanceGC.end();)
    {
        if (std::get<0>(*it) == GetCurrentThreadId())
        {
            std::cout<<"Freeing Tesseract Instance Pointer: "<<static_cast<void*>(std::get<1>(*it).get())<<"\n";
            std::get<1>(*it)->End();
            it = TesseractInstanceGC.erase(it);
        }
        else
            ++it;
    }
}

extern "C" BOOL __stdcall DllMain(HMODULE module, DWORD fdwReason, void* lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(module);
            hModule = module;
            return true;

        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            FreeUTF8Text();
            FreeTesseractInstance();
            break;

        default:
            break;
    }
    return true;
}

