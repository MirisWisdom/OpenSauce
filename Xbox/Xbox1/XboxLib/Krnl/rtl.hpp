/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(NTSTATUS, RtlAnsiStringToUnicodeString, (
		  PUNICODE_STRING DestinationString,
		  PSTRING         SourceString,
		  UCHAR           AllocateDestinationString
		  ));
#define KrnlRtlAnsiStringToUnicodeString K_KERNEL_EXPORTS.RtlAnsiStringToUnicodeString

XBOX_PROC(NTSTATUS, RtlAppendStringToString, (
		  IN PSTRING    Destination,
		  IN PSTRING    Source
		  ));
#define KrnlRtlAppendStringToString K_KERNEL_EXPORTS.RtlAppendStringToString

// compare block of memory, return number of equivalent bytes.
XBOX_PROC(SIZE_T, RtlCompareMemory, (
		  IN CONST VOID* Source1,
		  IN CONST VOID* Source2,
		  IN SIZE_T      Length
		  ));
#define KrnlRtlCompareMemory K_KERNEL_EXPORTS.RtlCompareMemory

XBOX_PROC(VOID, RtlCopyString, (
		  IN OUT PSTRING Dest,
		  IN PSTRING Source OPTIONAL
		  ));
#define KrnlRtlCopyString K_KERNEL_EXPORTS.RtlCopyString

XBOX_PROC(VOID, RtlEnterCriticalSection, (
		  IN PRTL_CRITICAL_SECTION CriticalSection
		  ));
#define KrnlRtlEnterCriticalSection K_KERNEL_EXPORTS.RtlEnterCriticalSection

XBOX_PROC(VOID, RtlInitAnsiString, (
		  IN OUT PANSI_STRING DestinationString,
		  IN     PCHAR        SourceString
		  ));
#define KrnlRtlInitAnsiString K_KERNEL_EXPORTS.RtlInitAnsiString

XBOX_PROC(VOID, RtlInitializeCriticalSection, (
		  IN PRTL_CRITICAL_SECTION CriticalSection
		  ));
#define KrnlRtlInitializeCriticalSection K_KERNEL_EXPORTS.RtlInitializeCriticalSection

XBOX_PROC(VOID, RtlLeaveCriticalSection, (
		  IN PRTL_CRITICAL_SECTION CriticalSection
		  ));
#define KrnlRtlLeaveCriticalSection K_KERNEL_EXPORTS.RtlLeaveCriticalSection

// Move memory either forward or backward, aligned or unaligned,
// in 4-byte blocks, followed by any remaining blocks.
XBOX_PROC(VOID, RtlMoveMemory, (
		  IN VOID UNALIGNED*	   Destination,
		  IN CONST VOID UNALIGNED* Source,
		  IN SIZE_T                Length
		  ));
#define KrnlRtlMoveMemory K_KERNEL_EXPORTS.RtlMoveMemory

XBOX_PROC(ULONG, RtlNtStatusToDosError, (
		  IN NTSTATUS	Status
		  ));
#define KrnlRtlNtStatusToDosError K_KERNEL_EXPORTS.RtlNtStatusToDosError

XBOX_PROC(NTSTATUS, RtlUnicodeStringToAnsiString, (
		  IN OUT PSTRING         DestinationString,
		  IN     PUNICODE_STRING SourceString,
		  IN     BOOLEAN         AllocateDestinationString
		  ));
#define KrnlRtlUnicodeStringToAnsiString K_KERNEL_EXPORTS.RtlUnicodeStringToAnsiString

XBOX_PROC(VOID, RtlZeroMemory, (
		  IN VOID UNALIGNED*  Destination,
		  IN SIZE_T           Length
		  ));
#define KrnlRtlZeroMemory K_KERNEL_EXPORTS.RtlZeroMemory