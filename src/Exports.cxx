/**  © 2014, Brandon T. All Rights Reserved.
  *
  *  This file is part of the LibTesseract Library.
  *  LibTesseract is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  LibTesseract is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with LibTesseract.  If not, see <http://www.gnu.org/licenses/>.
  */


#include "Exports.hxx"

EXPORT tesseract::TessBaseAPI* Tesseract_Create()
{
	return new tesseract::TessBaseAPI();
}

void Tesseract_Delete(tesseract::TessBaseAPI* &tesseract_ptr)
{
    delete tesseract_ptr;
    tesseract_ptr = NULL;
}

int Tesseract_Init(tesseract::TessBaseAPI* tesseract_ptr, const char* datapath, const char* language)
{
    return tesseract_ptr->Init(datapath, language);
}

void Tesseract_End(tesseract::TessBaseAPI* &tesseract_ptr)
{
	tesseract_ptr->End();
}

void Tesseract_SetImage(tesseract::TessBaseAPI* tesseract_ptr, const unsigned char* imagedata, int width, int height, int bytes_per_pixel, int bytes_per_line)
{
	tesseract_ptr->SetImage(imagedata, width, height, bytes_per_pixel, bytes_per_line);
}

const char* Tesseract_GetUTF8Text(tesseract::TessBaseAPI* tesseract_ptr)
{
    char* utf8_text_ptr = tesseract_ptr->GetUTF8Text();
    int len = strlen(utf8_text_ptr);

    char* pascal_string = new char[len + (sizeof(int) * 2) + 1]; //allocate a buffer representing pascal strings..
    strcpy(&pascal_string[sizeof(int) * 2], utf8_text_ptr); //copy the string into the pascal buffer..
    pascal_string[len + (sizeof(int) * 2) + 1] = 0;  //null terminate it..
    delete [] utf8_text_ptr;

    *((unsigned int*)&pascal_string[0]) = -1;  //set pascal reference count to constant..
    *((unsigned int*)&pascal_string[sizeof(int)]) = len; //set the length of the pascal string..

    return &pascal_string[sizeof(int) * 2];
}

void Tesseract_FreeUTF8Text(char* &utf8_text_ptr)
{
    delete[] &utf8_text_ptr[-(sizeof(int) * 2)]; //delete the pascal string..
    utf8_text_ptr = NULL;
}

bool Tesseract_SetVariable(tesseract::TessBaseAPI* tesseract_ptr, const char* name, const char* value)
{
	return tesseract_ptr->SetVariable(name, value);
}

void Tesseract_Clear(tesseract::TessBaseAPI* tesseract_ptr)
{
    tesseract_ptr->Clear();
}

int GetPluginABIVersion()
{
    return 2;
}

int GetFunctionCount()
{
    return PascalExportCount;
}

int GetTypeCount()
{
	return PascalTypeCount;
}

int GetFunctionInfo(int Index, void** Address, char** Definition)
{
    if (Index < PascalExportCount)
    {
        #if defined _WIN32 || defined _WIN64
        *Address = (void*)GetProcAddress(hInstance, PascalExports[Index * 2]);
        #else
        *Address = (void*)dlsym(RTLD_DEFAULT, PascalExports[Index * 2]);
        #endif
        strcpy(*Definition, PascalExports[Index * 2 + 1]);
        return Index;
    }
    return -1;
}

int GetTypeInfo(int Index, char** Type, char** Definition)
{
	if (Index < PascalTypeCount)
	{
		strcpy(*Type, PascalTypes[Index * 2 + 0]);
		strcpy(*Definition, PascalTypes[Index * 2 + 1]);
		return Index;
	}
	return -1;
}
