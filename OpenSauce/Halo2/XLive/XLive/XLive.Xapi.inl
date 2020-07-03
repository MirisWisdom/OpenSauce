/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/

XLIVE_API VOID WINAPI XCustomSetAction(__in        DWORD               dwActionIndex,
									   __in_z_opt  LPCWSTR             szActionText,
									   __in        DWORD               dwFlags) {
#ifndef XLIVE_NO_XAPI

#else

#endif
}

XLIVE_API BOOL WINAPI XCustomGetLastActionPress(__out_opt   DWORD*              pdwUserIndex,
												__out_opt   DWORD*              pdwActionIndex,
												__out_opt   XUID*               pXuid) {
#ifndef XLIVE_NO_XAPI
	return FALSE;
#else
	return FALSE;
#endif
}

XLIVE_API DWORD WINAPI XCustomSetDynamicActions(__in                        DWORD                   dwUserIndex,
												__in                        XUID                    xuid,
												__in_ecount( cCustomActions ) CONST XCUSTOMACTION*  pCustomActions,
												__in                        WORD                    cCustomActions) {
#ifndef XLIVE_NO_XAPI
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

XLIVE_API DWORD WINAPI XCustomGetLastActionPressEx(__out_opt                       DWORD*  pdwUserIndex,
												   __out_opt                       DWORD*  pdwActionId,
												   __out_opt                       XUID*   pXuid,
												   __out_bcount_opt( pcbPayload )  BYTE*   pbPayload,
												   __inout_opt                     WORD*   pcbPayload) {
#ifndef XLIVE_NO_XAPI
	return ERROR_NO_DATA;
#else
	return ERROR_NO_DATA;
#endif
}

XLIVE_API VOID WINAPI XCustomRegisterDynamicActions() {
#ifndef XLIVE_NO_MISC

#else

#endif
}

XLIVE_API VOID WINAPI XCustomUnregisterDynamicActions() {
#ifndef XLIVE_NO_XAPI

#else

#endif
}

XLIVE_API BOOL WINAPI XCustomGetCurrentGamercard(__out   DWORD*                  pdwUserIndex,
												 __out   XUID*                   pXuid) {
#ifndef XLIVE_NO_XAPI
	return FALSE;
#else
	return FALSE;
#endif
}



XLIVE_API DWORD WINAPI XGetOverlappedExtendedError(__in_opt    PXOVERLAPPED pOverlapped) {
#ifndef XLIVE_NO_XAPI
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

XLIVE_API DWORD WINAPI XGetOverlappedResult(__in        PXOVERLAPPED    pOverlapped,
											__out_opt   LPDWORD         pdwResult,
											__in        BOOL            bWait) {
	if (pdwResult != NULL)
		*pdwResult = 0;

#ifndef XLIVE_NO_XAPI
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}