/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
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