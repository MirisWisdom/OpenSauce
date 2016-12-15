/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(NTSTATUS, IoCreateFile, (
		  OUT PHANDLE             FileHandle,
		  IN  ACCESS_MASK         DesiredAccess,
		  IN  POBJECT_ATTRIBUTES  ObjectAttributes,
		  OUT PIO_STATUS_BLOCK    IoStatusBlock,
		  IN  PLARGE_INTEGER      AllocationSize,
		  IN  ULONG               FileAttributes,
		  IN  ULONG               ShareAccess,
		  IN  ULONG               Disposition,
		  IN  ULONG               CreateOptions,
		  IN  ULONG               Options
		  ));
#define KrnlIoCreateFile K_KERNEL_EXPORTS.IoCreateFile

XBOX_PROC(NTSTATUS, IoCreateSymbolicLink, (
		  IN PSTRING SymbolicLinkName,
		  IN PSTRING DeviceName
		  ));
#define KrnlIoCreateSymbolicLink K_KERNEL_EXPORTS.IoCreateSymbolicLink

XBOX_PROC(NTSTATUS, IoDeleteSymbolicLink, (
		  IN PSTRING SymbolicLinkName
		  ));
#define KrnlIoDeleteSymbolicLink K_KERNEL_EXPORTS.IoDeleteSymbolicLink