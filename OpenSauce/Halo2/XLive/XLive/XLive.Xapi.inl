/*
    Yelo: Open Sauce SDK

    Copyright (C) Kornner Studios (http://kornner.com)

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