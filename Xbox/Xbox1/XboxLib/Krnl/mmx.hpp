/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(LONG, FscSetCacheSize, (
		  ULONG uCachePages
		  ));
#define KrnlFscSetCacheSize K_KERNEL_EXPORTS.FscSetCacheSize



XBOX_PROC(PVOID, MmAllocateContiguousMemory, (
		  IN ULONG NumberOfBytes
		  ));
#define KrnlMmAllocateContiguousMemory K_KERNEL_EXPORTS.MmAllocateContiguousMemory

XBOX_PROC(PVOID, MmAllocateContiguousMemoryEx, (
		  IN ULONG			NumberOfBytes,
		  IN PHYSICAL_ADDRESS LowestAcceptableAddress,
		  IN PHYSICAL_ADDRESS HighestAcceptableAddress,
		  IN ULONG			Alignment OPTIONAL,
		  IN ULONG			ProtectionType
		  ));
#define KrnlMmAllocateContiguousMemoryEx K_KERNEL_EXPORTS.MmAllocateContiguousMemoryEx

XBOX_PROC(PVOID, MmAllocateSystemMemory, (
		  ULONG NumberOfBytes,
		  ULONG Protect
		  ));
#define KrnlMmAllocateSystemMemory K_KERNEL_EXPORTS.MmAllocateSystemMemory

XBOX_PROC(VOID, MmFreeContiguousMemory, (
		  IN PVOID BaseAddress
		  ));
#define KrnlMmFreeContiguousMemory K_KERNEL_EXPORTS.MmFreeContiguousMemory

XBOX_PROC(NTSTATUS, MmFreeSystemMemory, (
		  PVOID BaseAddress,
		  ULONG NumberOfBytes
		  ));
#define KrnlMmFreeSystemMemory K_KERNEL_EXPORTS.MmFreeSystemMemory

XBOX_PROC(PHYSICAL_ADDRESS, MmGetPhysicalAddress, (
		  IN PVOID   BaseAddress
		  ));
#define KrnlMmGetPhysicalAddress K_KERNEL_EXPORTS.MmGetPhysicalAddress

XBOX_PROC(PVOID, MmMapIoSpace, (
		  IN PHYSICAL_ADDRESS PhysicalAddress,
		  IN ULONG            NumberOfBytes,
		  IN ULONG            Protect
		  ));
#define KrnlMmMapIoSpace K_KERNEL_EXPORTS.MmMapIoSpace

XBOX_PROC(VOID, MmPersistContiguousMemory, (
		  IN PVOID   BaseAddress,
		  IN ULONG   NumberOfBytes,
		  IN BOOLEAN Persist
		  ));
#define KrnlMmPersistContiguousMemory K_KERNEL_EXPORTS.MmPersistContiguousMemory

XBOX_PROC(ULONG, MmQueryAllocationSize, (
		  IN PVOID   BaseAddress
		  ));
#define KrnlMmQueryAllocationSize K_KERNEL_EXPORTS.MmQueryAllocationSize

XBOX_PROC(VOID, MmSetAddressProtect, (
		  IN PVOID BaseAddress,
		  IN ULONG NumberOfBytes,
		  IN ULONG NewProtect
		  ));
#define KrnlMmSetAddressProtect K_KERNEL_EXPORTS.MmSetAddressProtect