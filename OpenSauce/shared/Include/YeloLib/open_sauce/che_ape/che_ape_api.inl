/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CheApe for specific license information
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