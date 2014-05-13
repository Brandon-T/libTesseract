#include "stdafx.h"
#define Export    extern "C" __declspec(dllexport)

static char* Exports[] = {
	(char*)"Tesseract_Create", (char*)"Function Tesseract_Create(): Pointer;",
	(char*)"Tesseract_Delete", (char*)"Procedure Tesseract_Delete(var tesseract_ptr: Pointer);",
    (char*)"Tesseract_Init", (char*)"Function Tesseract_Init(tesseract_ptr: Pointer; datapath, language: String): Integer;",
	(char*)"Tesseract_End", (char*)"Procedure Tesseract_End(var tesseract_ptr: Pointer);",
    (char*)"Tesseract_SetImage", (char*)"Procedure Tesseract_SetImage(tesseract_ptr: Pointer; const imagedata: Pointer; width, height, bytes_per_pixel, bytes_per_line: Integer);",
    (char*)"Tesseract_GetUTF8Text", (char*)"Function Tesseract_GetUTF8Text(tesseract_ptr: Pointer): Pointer;",
    (char*)"Tesseract_FreeUTF8Text", (char*)"Procedure Tesseract_FreeUTF8Text(var utf8_text_ptr: Pointer);",
	(char*)"Tesseract_SetVariable", (char*)"Function Tesseract_SetVariable(tesseract_ptr: Pointer; name, value: String): Boolean;",
	(char*)"Tesseract_Clear", (char*)"Procedure Tesseract_Clear(tesseract_ptr: Pointer);"
};

HMODULE hModule = nullptr;
const int ExportCount = sizeof(Exports)/(sizeof(Exports[0]) * 2);
TesseractGC<std::tuple<DWORD, PascalArray<char>>> UTF8TextGC;
TesseractGC<std::tuple<DWORD, std::unique_ptr<tesseract::TessBaseAPI>>> TesseractInstanceGC;


/**EXPORTS**/

Export tesseract::TessBaseAPI* Tesseract_Create()
{
    auto result = TesseractInstanceGC.push_back(std::make_tuple(GetCurrentThreadId(), std::move(std::unique_ptr<tesseract::TessBaseAPI>(new tesseract::TessBaseAPI()))));
	return std::get<1>(*result).get();
}

Export void Tesseract_Delete(tesseract::TessBaseAPI* &tesseract_ptr)
{
    auto it = std::find_if(TesseractInstanceGC.begin(), TesseractInstanceGC.end(), [&](const decltype(TesseractInstanceGC)::value_type &other) {
        return std::get<1>(other).get() == tesseract_ptr;
    });

    if (it != TesseractInstanceGC.end())
    {
        std::get<1>(*it)->End();
        TesseractInstanceGC.erase(it);
        tesseract_ptr = nullptr;
    }
}

Export int Tesseract_Init(tesseract::TessBaseAPI* tesseract_ptr, const char* datapath, const char* language)
{
    return tesseract_ptr->Init(datapath, language);
}

Export void Tesseract_End(tesseract::TessBaseAPI* &tesseract_ptr)
{
	tesseract_ptr->End();
}

Export void Tesseract_SetImage(tesseract::TessBaseAPI* tesseract_ptr, const unsigned char* imagedata, int width, int height, int bytes_per_pixel, int bytes_per_line)
{
	tesseract_ptr->SetImage(imagedata, width, height, bytes_per_pixel, bytes_per_line);
}

Export const char* Tesseract_GetUTF8Text(tesseract::TessBaseAPI* tesseract_ptr)
{
    std::unique_ptr<char[]> utf8_text_ptr(tesseract_ptr->GetUTF8Text());
    PascalArray<char> pascal_string(strlen(utf8_text_ptr.get()));
    strcpy(&pascal_string[0], utf8_text_ptr.get());
    auto result = UTF8TextGC.push_back(std::move(std::make_pair(GetCurrentThreadId(), std::move(pascal_string))));
 //std::this_thread::get_id()
    return &(std::get<1>(*result))[0];
}

Export void Tesseract_FreeUTF8Text(char* &utf8_text_ptr)
{
    auto it = std::find_if(UTF8TextGC.begin(), UTF8TextGC.end(), [&](const decltype(UTF8TextGC)::value_type &other) {
        return &(std::get<1>(other))[0] == utf8_text_ptr;
    });

    if (it != UTF8TextGC.end())
    {
        UTF8TextGC.erase(it);
        utf8_text_ptr = nullptr;
    }
}

Export bool Tesseract_SetVariable(tesseract::TessBaseAPI* tesseract_ptr, const char* name, const char* value)
{
	return tesseract_ptr->SetVariable(name, value);
}

Export void Tesseract_Clear(tesseract::TessBaseAPI* tesseract_ptr)
{
    tesseract_ptr->Clear();
}

extern "C" int __declspec(dllexport) GetPluginABIVersion()
{
    return 2;
}

extern "C" int __declspec(dllexport) GetFunctionCount()
{
    return ExportCount;
}

extern "C" int __declspec(dllexport) GetFunctionInfo(int Index, void* &Address, char* &Definition)
{
    if (Index < ExportCount)
    {
        Address = reinterpret_cast<void*>(GetProcAddress(hModule, Exports[Index * 2]));
		#ifdef _MSC_VER
		#pragma warning(disable: 4996)
		strcpy(Definition, Exports[Index * 2 + 1]);
		#else
		strcpy(Definition, Exports[Index * 2 + 1]);
		#endif
        return Index;
    }
    return -1;
}
