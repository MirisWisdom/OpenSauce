/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(NTSTATUS, PsCreateSystemThreadEx, (
		  OUT PHANDLE         ThreadHandle,
		  IN  ULONG           ThreadExtraSize,
		  IN  ULONG           KernelStackSize,
		  IN  ULONG           TlsDataSize,
		  OUT PULONG          ThreadId OPTIONAL,
		  IN  PVOID           StartContext1,
		  IN  PVOID           StartContext2,
		  IN  BOOLEAN         CreateSuspended,
		  IN  BOOLEAN         DebugStack,
		  IN  PKSTART_ROUTINE StartRoutine
		  ));
#define Krnl K_KERNEL_EXPORTS.

XBOX_PROC(VOID, PsTerminateSystemThread, (
		  IN NTSTATUS ExitStatus
		  ));
#define KrnlPsTerminateSystemThread K_KERNEL_EXPORTS.PsTerminateSystemThread