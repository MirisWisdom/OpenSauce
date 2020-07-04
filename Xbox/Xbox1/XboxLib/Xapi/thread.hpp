/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

//_XapiCallThreadNotifyRoutines@4
//_SetThreadPriority@8
//_GetThreadPriority@4
//_SetThreadPriorityBoost@8
//_GetThreadPriorityBoost@8
//_SuspendThread@4
//_ResumeThread@4
//_RaiseException@16
//_XapiDispatchAPC@12
//_QueueUserAPC@12
//_SwitchToThread@0
//_ExitThread@4
//_GetExitCodeThread@8
//_SetUnhandledExceptionFilter@4
//_GetThreadTimes@20
//_XRegisterThreadNotifyRoutine@8
//_XSetProcessQuantumLength@4
//_XGetProcessQuantumLength@0
//_UnhandledExceptionFilter@4
//_XapiThreadStartup@8



XBOX_PROC(HANDLE, CreateThread, (
		  __in_opt  LPSECURITY_ATTRIBUTES lpThreadAttributes,
		  __in      SIZE_T dwStackSize,
		  __in      LPTHREAD_START_ROUTINE lpStartAddress,
		  __in_opt  LPVOID lpParameter,
		  __in      DWORD dwCreationFlags,
		  __out_opt LPDWORD lpThreadId
		  ));
#define ApiCreateThread XboxApi.CreateThread



XBOX_PROC(DWORD, GetCurrentThreadId, (
		  VOID
		  ));
#define ApiGetCurrentThreadId XboxApi.GetCurrentThreadId