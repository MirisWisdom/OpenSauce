/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
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