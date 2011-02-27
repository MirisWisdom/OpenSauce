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
#pragma once

typedef ULONGLONG XUID;
typedef XUID *PXUID;

typedef struct {
	WORD wActionId;
	WCHAR wszActionText[23];
	DWORD dwFlags;
}XCUSTOMACTION;

typedef
VOID
(WINAPI *PXOVERLAPPED_COMPLETION_ROUTINE)(
	__in    DWORD                       dwErrorCode,
	__in    DWORD                       dwNumberOfBytesTransfered,
	__inout struct _XOVERLAPPED*        pOverlapped);


typedef struct _XOVERLAPPED {
	ULONG_PTR                           InternalLow;
	ULONG_PTR                           InternalHigh;
	ULONG_PTR                           InternalContext;
	HANDLE                              hEvent;
	PXOVERLAPPED_COMPLETION_ROUTINE     pCompletionRoutine;
	DWORD_PTR                           dwCompletionContext;
	DWORD                               dwExtendedError;
}XOVERLAPPED, *PXOVERLAPPED;