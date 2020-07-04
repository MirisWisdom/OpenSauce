/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(HANDLE, CreateEventA, (
		  __in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
		  __in     BOOL bManualReset,
		  __in     BOOL bInitialState,
		  __in_opt LPCSTR lpName
		  ));
#define ApiCreateEvent XboxApi.CreateEvent



//_OpenEventA@12
//_SetEvent@4
//_ResetEvent@4
//_PulseEvent@4



XBOX_PROC(HANDLE, CreateSemaphoreA, (
		  __in_opt LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
		  __in     LONG lInitialCount,
		  __in     LONG lMaximumCount,
		  __in_opt LPCSTR lpName
		  ));
#define ApiCreateSemaphore XboxApi.CreateSemaphore



//_OpenSemaphoreA@12
//_ReleaseSemaphore@12
//_CreateMutexA@12
//_OpenMutexA@12
//_ReleaseMutex@4
//_WaitForSingleObjectEx@12
//_SignalObjectAndWait@16
//_WaitForMultipleObjectsEx@20
//_SleepEx@8
//_CreateWaitableTimerA@12
//_OpenWaitableTimerA@12
//_SetWaitableTimer@24
//_CancelWaitableTimer@4
//_WaitForSingleObject@8
//_WaitForMultipleObjects@16
//_Sleep@4