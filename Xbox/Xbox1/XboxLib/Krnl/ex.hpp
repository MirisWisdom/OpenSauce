/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(PVOID, ExAllocatePool, (
		  IN ULONG NumberOfBytes
		  ));
#define KrnlExAllocatePool K_KERNEL_EXPORTS.ExAllocatePool

XBOX_PROC(PVOID, ExFreePool, (
		  IN PVOID P
		  ));
#define KrnlExFreePool K_KERNEL_EXPORTS.ExFreePool

XBOX_PROC(NTSTATUS, ExQueryNonVolatileSetting, (
		  IN  DWORD               ValueIndex,
		  OUT DWORD*			  Type,
		  OUT PUCHAR              Value,
		  IN  SIZE_T              ValueLength,
		  OUT PSIZE_T             ResultLength OPTIONAL
		  ));
#define KrnlExQueryNonVolatileSetting K_KERNEL_EXPORTS.ExQueryNonVolatileSetting

XBOX_PROC(NTSTATUS, ExSaveNonVolatileSetting, (
		  IN  DWORD               ValueIndex,
		  OUT DWORD*			  Type,
		  IN  PUCHAR              Value,
		  IN  SIZE_T              ValueLength
		  ));
#define KrnlExSaveNonVolatileSetting K_KERNEL_EXPORTS.ExSaveNonVolatileSetting