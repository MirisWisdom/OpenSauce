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

XLIVE_API INT WINAPI XShowGuideUI(DWORD) {
#ifndef XLIVE_NO_XLIVE
	return 1;
#else
	return 1;
#endif
}