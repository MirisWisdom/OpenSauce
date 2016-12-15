/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/

XLIVE_API INT WINAPI XLiveInitialize(__in const XLiveInitializeParams * pxlip) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLiveInput(DWORD* p) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	p[5] = 0;
	return 1;	// -1 ?
#endif
}

XLIVE_API INT WINAPI XLiveRender() {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLiveUninitialize() {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLiveOnCreateDevice(void*, void*) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLive_5006() {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLiveOnResetDevice(void*) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API HRESULT WINAPI XHV2CreateEngine(void* /*PXHV_INIT_PARAMS*/ pParams,
										  HANDLE *phWorkerThread,
										  void* /*PIXHV2ENGINE*/ *ppEngine) {
#ifndef XLIVE_NO_XLIVE
	return 0xFFFFFFFF;
#else
	if(ppEngine != NULL)
		*ppEngine = NULL;

	return 0xFFFFFFFF;
#endif
}

//XLive_5010
//XLive_5011
//XLive_5012

XLIVE_API INT WINAPI XLivePBufferAllocate(DWORD size, void** pBuffer) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	XLiveFeignProtectedBuffer* buf = CAST_PTR(XLiveFeignProtectedBuffer*, malloc(size+sizeof(XLiveFeignProtectedBuffer)));
	if (buf == NULL)
		return E_OUTOFMEMORY;

	buf->dwSignature = XLIVE_FEIGN_PROTECTED_BUFFER_SIG;
	buf->dwSize = size;
	*pBuffer = buf;

	return NO_ERROR;
#endif
}

//XLive_5017

XLIVE_API INT WINAPI XLivePBufferGetByte(void* pBuffer, DWORD dwOffset, BYTE* value) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	XLiveFeignProtectedBuffer* buf = CAST_PTR(XLiveFeignProtectedBuffer*, pBuffer);

	if(buf != NULL && buf->dwSignature == XLIVE_FEIGN_PROTECTED_BUFFER_SIG && dwOffset >= 0 && dwOffset < buf->dwSize)
	{
		BYTE* data = CAST_PTR(BYTE*, buf+1);
		*value = data[dwOffset];
	}

	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLivePBufferSetByte(void* pBuffer, DWORD dwOffset, BYTE value) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	XLiveFeignProtectedBuffer* buf = CAST_PTR(XLiveFeignProtectedBuffer*, pBuffer);

	if(buf != NULL && buf->dwSignature == XLIVE_FEIGN_PROTECTED_BUFFER_SIG && dwOffset >= 0 && dwOffset < buf->dwSize)
	{
		BYTE* data = CAST_PTR(BYTE*, buf+1);
		data[dwOffset] = value;
	}

	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLivePBufferGetDword(void* pBuffer, DWORD dwOffset, DWORD* value) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	XLiveFeignProtectedBuffer* buf = CAST_PTR(XLiveFeignProtectedBuffer*, pBuffer);

	if(buf != NULL && buf->dwSignature == XLIVE_FEIGN_PROTECTED_BUFFER_SIG && dwOffset >= 0 && dwOffset < buf->dwSize)
	{
		BYTE* data = CAST_PTR(BYTE*, buf+1);
		*value = *CAST_PTR(DWORD*, &data[dwOffset]);
	}

	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLivePBufferSetDword(void* pBuffer, DWORD dwOffset, DWORD value) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	XLiveFeignProtectedBuffer* buf = CAST_PTR(XLiveFeignProtectedBuffer*, pBuffer);

	if(buf != NULL && buf->dwSignature == XLIVE_FEIGN_PROTECTED_BUFFER_SIG && dwOffset >= 0 && dwOffset < buf->dwSize)
	{
		BYTE* data = CAST_PTR(BYTE*, buf+1);
		*CAST_PTR(DWORD*, &data[dwOffset]) = value;
	}

	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLiveGetUpdateInformation(DWORD) {
#ifndef XLIVE_NO_XLIVE
	return -1;
#else
	return -1;
#endif
}

//XLive_5023

XLIVE_API INT WINAPI XLiveUpdateSystem(DWORD) {
#ifndef XLIVE_NO_XLIVE
	return -1;
#else
	return -1;
#endif
}

//XLive_5025
//XLive_5026
//XLive_5027

XLIVE_API INT WINAPI XLiveSecureLoadLibraryW(LPCWSTR lpLibFileName, OUT HMODULE* hLibHandle, DWORD dwFlags) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	HMODULE handle = LoadLibraryExW(lpLibFileName, NULL, dwFlags);
	if(hLibHandle != NULL)
		*hLibHandle = handle;

	return NO_ERROR;
#endif
}

//XLive_5029

XLIVE_API INT WINAPI XLivePreTranslateMessage(DWORD) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

//XLive_5206

//XLive_5208
//XLive_5209

XLIVE_API INT WINAPI XLive_5210(DWORD) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

XLIVE_API INT WINAPI XLive_5212(DWORD, DWORD, LPWSTR, LPWSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD) {
#ifndef XLIVE_NO_XLIVE
	return NO_ERROR;
#else
	return NO_ERROR;
#endif
}

//XLive_5213
//XLive_5214

XLIVE_API INT WINAPI XLiveShowGuideUI(DWORD) {
#ifndef XLIVE_NO_XLIVE
	return 1;
#else
	return 1;
#endif
}

//XLive_5230
//XLive_5231

//XLive_5233
//XLive_5234
//XLive_5235
//XLive_5236
//XLive_5237
//XLive_5238

//XLive_5250
//XLive_5251
//XLive_5252
//XLive_5253

XLIVE_API DWORD WINAPI XLiveCancelOverlapped(PXOVERLAPPED pOverlapped) {
#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

//XLive_5255

XLIVE_API DWORD WINAPI XLiveEnumerate(HANDLE hEnum, PVOID pvBuffer, DWORD cbBuffer, PDWORD pcItemsReturned, PXOVERLAPPED pOverlapped) {
	if(pcItemsReturned != NULL)
		*pcItemsReturned = 0;

#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

//XLive_5257
//XLive_5258
//XLive_5259

XLIVE_API DWORD WINAPI XLiveShowSigninUI(DWORD cPanes, DWORD dwFlags) {
#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

XLIVE_API DWORD WINAPI XLiveUserGetXUID(DWORD dwUserIndex, PXUID *pxuid) {
	static XUID k_stub_xuid = 0x1000100010001000UL; // note really constant...
	*pxuid = &k_stub_xuid;

#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

XLIVE_API DWORD WINAPI XLiveUserGetSigninState(DWORD dwUserIndex) {
#ifndef XLIVE_NO_XLIVE
	return eXUserSigninState_SignedInLocally;
#else
	return eXUserSigninState_SignedInLocally;
#endif
}

XLIVE_API DWORD WINAPI XLiveUserGetName(DWORD dwUserIndex, LPSTR szUserName, DWORD cchUserName) {
	static const char k_stub_player_name[] = "Player1";
	static const size_t k_stub_player_name_length = NUMBEROF(k_stub_player_name)+1; // include null terminator
	if (cchUserName < k_stub_player_name_length)
		return ERROR_INVALID_PARAMETER;

	strcpy_s(szUserName, cchUserName*sizeof(szUserName[0]), k_stub_player_name);
#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

XLIVE_API DWORD WINAPI XUserAreUsersFriends(DWORD dwUserIndex, PXUID pXuids, DWORD dwXuidCount, PBOOL pfResult, PXOVERLAPPED pOverlapped) {
	if(pfResult != NULL)
		*pfResult = FALSE;

#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

XLIVE_API DWORD WINAPI XLiveUserCheckPrivilege(DWORD dwUserIndex, DWORD/*XPRIVILEGE_TYPE*/ PrivilegeType, PBOOL pfResult) {
	*pfResult = FALSE;

#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}


XLIVE_API HANDLE WINAPI XLiveNotifyCreateListener(ULONGLONG qwAreas) {
	static const HANDLE k_feign_handle = CAST_PTR(HANDLE, 1); // This function returns zero when there's a failure (though I wonder if INVALID_HANDLE_VALUE would work too?)

#ifndef XLIVE_NO_XLIVE
	return k_feign_handle;
#else
	return k_feign_handle;
#endif
}

XLIVE_API DWORD WINAPI XLiveShowPlayersUI(DWORD dwUserIndex) {
#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

//XLive_5272

XLIVE_API DWORD WINAPI XLiveUserReadGamerPictureByKey(CONST PVOID/*PXUSER_DATA*/ pGamercardPictureKey,
													  BOOL fSmall, PBYTE pbTextureBuffer, DWORD dwPitch, DWORD dwHeight,
													  PXOVERLAPPED pOverlapped
) {
#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

//XLive_5274

XLIVE_API DWORD WINAPI XLiveShowFriendsUI(DWORD dwUserIndex) {
#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

// The API function is VOID...
XLIVE_API DWORD WINAPI XLiveUserSetProperty(DWORD dwUserIndex, DWORD dwPropertyId, DWORD cbValue, CONST VOID *pvValue) {
#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

// The API function is VOID...
XLIVE_API DWORD WINAPI XLiveUserSetContext(DWORD dwUserIndex, DWORD dwContextId, DWORD dwContextValue) {
#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

XLIVE_API DWORD WINAPI XLiveUserWriteAchievements(DWORD dwTitleId, DWORD dwUserIndex, XUID xuid,
												  DWORD dwDetailFlags, DWORD dwStartingIndex, DWORD cItem,
												  PDWORD pcbBuffer, PHANDLE ph) {
	*pcbBuffer = 0;
	*ph = INVALID_HANDLE_VALUE;

#ifndef XLIVE_NO_XLIVE
	return ERROR_INVALID_FUNCTION;
#else
	return ERROR_INVALID_FUNCTION;
#endif
}

//XLive_5279

XLIVE_API DWORD WINAPI XLiveUserCreateAchievementEnumerator(DWORD dwNumAchievements, CONST VOID/*XUSER_ACHIEVEMENT*/ *pAchievements, PXOVERLAPPED *pOverlapped) {
#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

XLIVE_API DWORD WINAPI XLiveUserReadStats(DWORD dwTitleId, DWORD dwNumXuids, CONST XUID *pXuids,
										  DWORD dwNumStatsSpecs, CONST VOID /*XUSER_STATS_SPEC*/ *pSpecs,
										  PDWORD *pcbResults, DWORD* /*PXUSER_STATS_READ_RESULTS*/ pResults,
										  PXOVERLAPPED *pOverlapped) {
	*(*pcbResults) = 4; // The XLive kernel implementation may defined this just as a DWORD*, not a PDWORD* (so verify, as always)
	if(pResults != NULL) *pResults = 0;

#ifndef XLIVE_NO_XLIVE
	return ERROR_SUCCESS;
#else
	return ERROR_SUCCESS;
#endif
}

//XLive_5282
//XLive_5283
//XLive_5284
//XLive_5285
//XLive_5286
//XLive_5287
//XLive_5288
//XLive_5289
//XLive_5290
//XLive_5291
//XLive_5292
//XLive_5293

XLIVE_API DWORD WINAPI XLiveSessionCreate(DWORD dwFlags, DWORD dwUserIndex, DWORD dwMaxPublicSlots, DWORD dwMaxPrivateSlots,
										  ULONGLONG *pqwSessionNonce, PXSESSION_INFO pSessionInfo, PXOVERLAPPED pXOverlapped,
										  HANDLE *ph) {
  *ph = INVALID_HANDLE_VALUE;

#ifndef XLIVE_NO_XLIVE
	return ERROR_INVALID_FUNCTION;
#else
	return ERROR_INVALID_FUNCTION;
#endif
}

//XLive_5303