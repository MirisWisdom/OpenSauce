/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(NTSTATUS, KeDelayExecutionThread, (
		  IN CCHAR			  WaitMode,
		  IN BOOLEAN          Alertable,
		  IN PLARGE_INTEGER   Interval
		  ));
#define KrnlDelayExecutionThread K_KERNEL_EXPORTS.DelayExecutionThread

XBOX_PROC(VOID, KeInitializeDpc, (
		  KDPC*				   Dpc,
		  PKDEFERRED_ROUTINE   DeferredRoutine,
		  PVOID                DeferredContext
		  ));
#define KrnlInitializeDpc K_KERNEL_EXPORTS.InitializeDpc

XBOX_PROC(VOID, KeInitializeTimerEx, (
		  IN PKTIMER      Timer,
		  IN TIMER_TYPE   Type
		  ));
#define KrnlInitializeTimerEx K_KERNEL_EXPORTS.InitializeTimerEx

XBOX_PROC(VOID, KeQuerySystemTime, (
		  PLARGE_INTEGER CurrentTime
		  ));
#define KrnlQuerySystemTime K_KERNEL_EXPORTS.QuerySystemTime

XBOX_PROC(BOOLEAN, KeSetTimer, (
		  IN PKTIMER        Timer,
		  IN LARGE_INTEGER  DueTime,
		  IN PKDPC          Dpc OPTIONAL
		  ));
#define KrnlSetTimer K_KERNEL_EXPORTS.SetTimer