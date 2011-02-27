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

XLIVE_API BOOL WINAPI XNotifyGetNext(__in        HANDLE                  hNotification,
									 __in        DWORD                   dwMsgFilter,
									 __out       PDWORD                  pdwId,
									 __out_opt   PULONG_PTR              pParam) {
#ifndef XLIVE_NO_XAM
	return FALSE;
#else
	return FALSE;
#endif
}

XLIVE_API VOID WINAPI XNotifyPositionUI(__in    DWORD dwPosition) {
#ifndef XLIVE_NO_XAM

#else

#endif
}

XLIVE_API DWORD WINAPI XNotifyDelayUI(__in    ULONG ulMilliSeconds) {
#ifndef XLIVE_NO_XAM
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}