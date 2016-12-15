/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

//_KeGetCurrentIrql@0
//_KeGetCurrentThread@0
//_XapiIsXapiThread@0

XBOX_PROC(DWORD, GetLastError, (
		  ));
#define ApiGetLastError XboxApi.GetLastError


//_SetLastError@4
//_XapiSetLastNTError@4
//_CreateIoCompletionPort@16
//_PostQueuedCompletionStatus@16
//_GetQueuedCompletionStatus@20
//_GetOverlappedResult@16