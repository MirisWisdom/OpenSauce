/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

//_UTF8ToUnicode@16
//_UnicodeToUTF8@16



XBOX_PROC(int, MultiByteToWideChar, (
		  UINT CodePage,         // code page
		  DWORD dwFlags,         // character-type options
		  LPCSTR lpMultiByteStr, // string to map
		  int cbMultiByte,       // number of bytes in string
		  LPWSTR lpWideCharStr,  // wide-character buffer
		  int cchWideChar        // size of buffer
		  ));
#define ApiMultiByteToWideChar XboxApi.MultiByteToWideChar



XBOX_PROC(int, WideCharToMultiByte, (
		  UINT CodePage,            // code page
		  DWORD dwFlags,            // performance and mapping flags
		  LPCWSTR lpWideCharStr,    // wide-character string
		  int cchWideChar,          // number of chars in string.
		  LPSTR lpMultiByteStr,     // buffer for new string
		  int cbMultiByte,          // size of buffer
		  LPCSTR lpDefaultChar,     // default for unmappable chars
		  LPBOOL lpUsedDefaultChar  // set when default char used
		  ));
#define ApiWideCharToMultiByte XboxApi.WideCharToMultiByte