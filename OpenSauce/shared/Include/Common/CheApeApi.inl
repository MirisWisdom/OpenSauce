/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
extern "C" {

	bool __declspec( dllexport ) CheApeApi_GetPchBuildDateA(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPSTR lpBuffer)
	{
		if(lpBuffer != NULL)
			return strcpy_s(lpBuffer, nBufferLength, __TIMESTAMP__) == k_errnone;

		return false;
	}

	bool __declspec( dllexport ) CheApeApi_GetPchBuildDateW(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPWSTR lpBuffer)
	{
		if(lpBuffer != NULL)
			return wcscpy_s(lpBuffer, nBufferLength, BOOST_PP_CAT(L, __TIMESTAMP__)) == k_errnone;
		
		return false;
	}

	bool __declspec( dllexport ) CheApeApi_GetPchPathA(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPSTR lpBuffer)
	{
		if(lpBuffer != NULL)
			return strcpy_s(lpBuffer, nBufferLength, k_cheape_api_pch_path_ascii) == k_errnone;

		return false;
	}

	bool __declspec( dllexport ) CheApeApi_GetPchPathW(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPWSTR lpBuffer)
	{
		if(lpBuffer != NULL)
			return wcscpy_s(lpBuffer, nBufferLength, k_cheape_api_pch_path_wide) == k_errnone;

		return false;
	}

	bool __declspec( dllexport ) CheApeApi_GetTargetToolNameW(__in DWORD nBufferLength,
		__out_ecount_part_opt(nBufferLength, return + 1) LPWSTR lpBuffer)
	{
		if(lpBuffer != NULL)
			return wcscpy_s(lpBuffer, nBufferLength, 
				#if defined(PLATFORM_TYPE_GUERILLA)
					L"Guerilla"
				#elif defined(PLATFORM_TYPE_TOOL)
					L"Tool"
				#elif defined(PLATFORM_TYPE_SAPIEN)
					L"Sapien"
				#endif
			) == k_errnone;

		return false;
	}

	bool __declspec( dllexport ) CheApeApi_IsDebug()
	{
		return
			#ifdef _DEBUG
				true
			#else
				false
			#endif
		;
	}

};