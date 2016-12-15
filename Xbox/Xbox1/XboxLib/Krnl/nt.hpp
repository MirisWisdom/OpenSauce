/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(NTSTATUS, NtAllocateVirtualMemory, (
		  IN OUT PVOID*	   BaseAddress,
		  IN ULONG         ZeroBits,
		  IN OUT PULONG    AllocationSize,
		  IN DWORD         AllocationType,
		  IN DWORD         Protect
		  ));
#define KrnlNtAllocateVirtualMemory K_KERNEL_EXPORTS.NtAllocateVirtualMemory

XBOX_PROC(NTSTATUS, NtClearEvent, (
		  IN HANDLE EventHandle
		  ));
#define KrnlNtClearEvent K_KERNEL_EXPORTS.NtClearEvent

XBOX_PROC(NTSTATUS, NtClose, (
		  IN HANDLE Handle
		  ));
#define KrnlNtClose K_KERNEL_EXPORTS.NtClose

XBOX_PROC(NTSTATUS, NtCreateEvent, (
		  OUT PHANDLE             EventHandle,
		  IN  POBJECT_ATTRIBUTES  ObjectAttributes OPTIONAL,
		  IN  EVENT_TYPE          EventType,
		  IN  BOOLEAN             InitialState
		  ));
#define KrnlNtCreateEvent K_KERNEL_EXPORTS.NtCreateEvent

// Causes a new file or directory to be created, or opens and
// existing file, device, directory, or volume, giving the caller
// a handle for the file object.
XBOX_PROC(NTSTATUS, NtCreateFile, (
		  OUT PHANDLE             FileHandle, 
		  IN  ACCESS_MASK         DesiredAccess,
		  IN  POBJECT_ATTRIBUTES	ObjectAttributes,
		  OUT PIO_STATUS_BLOCK		IoStatusBlock,
		  IN  PLARGE_INTEGER	    AllocationSize OPTIONAL, 
		  IN  ULONG	            FileAttributes, 
		  IN  ULONG	            ShareAccess, 
		  IN  ULONG	            CreateDisposition, 
		  IN  ULONG	            CreateOptions
		  ));
#define KrnlNtCreateFile K_KERNEL_EXPORTS.NtCreateFile

XBOX_PROC(NTSTATUS, NtCreateMutant, (
		  OUT PHANDLE             MutantHandle,
		  IN  POBJECT_ATTRIBUTES  ObjectAttributes,
		  IN  BOOLEAN             InitialOwner
		  ));
#define KrnlNtCreateMutant K_KERNEL_EXPORTS.NtCreateMutant

XBOX_PROC(NTSTATUS, NtDuplicateObject, (
		  PVOID   SourceHandle,
		  PVOID*  TargetHandle,
		  DWORD   Options
		  ));
#define KrnlNtDuplicateObject K_KERNEL_EXPORTS.NtDuplicateObject

XBOX_PROC(NTSTATUS, NtFreeVirtualMemory, (
		  IN OUT PVOID* BaseAddress,
		  IN OUT PULONG FreeSize,
		  IN ULONG      FreeType
		  ));
#define KrnlNtFreeVirtualMemory K_KERNEL_EXPORTS.NtFreeVirtualMemory

XBOX_PROC(NTSTATUS, NtOpenFile, (
		  OUT PHANDLE             FileHandle,
		  IN  ACCESS_MASK         DesiredAccess,
		  IN  POBJECT_ATTRIBUTES  ObjectAttributes,
		  OUT PIO_STATUS_BLOCK    IoStatusBlock,
		  IN  ULONG               ShareAccess,
		  IN  ULONG               OpenOptions
		  ));
#define KrnlNtOpenFile K_KERNEL_EXPORTS.NtOpenFile

XBOX_PROC(NTSTATUS, NtQueryDirectoryFile, (
		  IN  HANDLE                      FileHandle,
		  IN  HANDLE                      Event OPTIONAL,
		  IN  PVOID                       ApcRoutine, // Todo: define this routine's prototype
		  IN  PVOID                       ApcContext,
		  OUT PIO_STATUS_BLOCK            IoStatusBlock,
		  OUT FILE_DIRECTORY_INFORMATION* FileInformation,
		  IN  ULONG                       Length,
		  IN  FILE_INFORMATION_CLASS      FileInformationClass,
		  IN  PSTRING                     FileMask,
		  IN  BOOLEAN                     RestartScan
		  ));
#define KrnlNtQueryDirectoryFile K_KERNEL_EXPORTS.NtQueryDirectoryFile

XBOX_PROC(NTSTATUS, NtQueryFullAttributesFile, (
		  IN  POBJECT_ATTRIBUTES          ObjectAttributes,
		  OUT PVOID                       Attributes
		  ));
#define KrnlNtQueryFullAttributesFile K_KERNEL_EXPORTS.NtQueryFullAttributesFile

XBOX_PROC(NTSTATUS, NtQueryInformationFile, (
		  IN  HANDLE                      FileHandle,
		  OUT PIO_STATUS_BLOCK            IoStatusBlock,
		  OUT PVOID                       FileInformation, 
		  IN  ULONG                       Length, 
		  IN  FILE_INFORMATION_CLASS      FileInfo
		  ));
#define KrnlNtQueryInformationFile K_KERNEL_EXPORTS.NtQueryInformationFile

XBOX_PROC(NTSTATUS, NtQueryVolumeInformationFile, (
		  IN  HANDLE                      FileHandle,
		  OUT PIO_STATUS_BLOCK            IoStatusBlock,
		  OUT PFILE_FS_SIZE_INFORMATION   FileInformation,
		  IN  ULONG                       Length,
		  IN  FS_INFORMATION_CLASS        FileInformationClass
		  ));
#define KrnlNtQueryVolumeInformationFile K_KERNEL_EXPORTS.NtQueryVolumeInformationFile

XBOX_PROC(NTSTATUS, NtReadFile, (
		  IN  HANDLE          FileHandle,            // TODO: correct paramters
		  IN  HANDLE          Event OPTIONAL,
		  IN  PVOID           ApcRoutine OPTIONAL,
		  IN  PVOID           ApcContext,
		  OUT PVOID           IoStatusBlock,
		  OUT PVOID           Buffer,
		  IN  ULONG           Length,
		  IN  PLARGE_INTEGER  ByteOffset OPTIONAL
		  ));
#define KrnlNtReadFile K_KERNEL_EXPORTS.NtReadFile

XBOX_PROC(NTSTATUS, NtResumeThread, (
		  IN  HANDLE ThreadHandle,
		  OUT PULONG PreviousSuspendCount
		  ));
#define KrnlNtResumeThread K_KERNEL_EXPORTS.NtResumeThread

XBOX_PROC(NTSTATUS, NtSetEvent, (
		  IN  HANDLE ThreadHandle,
		  OUT PULONG PreviousSuspendCount
		  ));
#define KrnlNtSetEvent K_KERNEL_EXPORTS.NtSetEvent

XBOX_PROC(NTSTATUS, NtSetInformationFile, (
		  IN  HANDLE  FileHandle,            // TODO: correct parameters
		  OUT	PVOID	IoStatusBlock,
		  IN	PVOID	FileInformation,
		  IN	ULONG	Length,
		  IN	ULONG	FileInformationClass
		  ));
#define KrnlNtSetInformationFile K_KERNEL_EXPORTS.NtSetInformationFile

XBOX_PROC(VOID, NtUserIoApcDispatcher, (
		  PVOID            ApcContext,
		  PIO_STATUS_BLOCK IoStatusBlock,
		  ULONG            Reserved
		  ));
#define KrnlNtUserIoApcDispatcher K_KERNEL_EXPORTS.NtUserIoApcDispatcher

XBOX_PROC(NTSTATUS, NtWaitForSingleObject, (
		  IN  HANDLE  Handle,
		  IN	BOOLEAN	Alertable,
		  IN	PVOID	Timeout
		  ));
#define KrnlNtWaitForSingleObject K_KERNEL_EXPORTS.NtWaitForSingleObject

XBOX_PROC(NTSTATUS, NtWaitForSingleObjectEx, (
		  IN  HANDLE          Handle,
		  IN  DWORD           WaitMode,
		  IN  BOOLEAN         Alertable,
		  IN  PLARGE_INTEGER  Timeout
		  ));
#define KrnlNtWaitForSingleObjectEx K_KERNEL_EXPORTS.NtWaitForSingleObjectEx

XBOX_PROC(NTSTATUS, NtWriteFile, (
		  IN  HANDLE  FileHandle,            // TODO: correct paramters
		  IN	PVOID	Event,
		  IN	PVOID	ApcRoutine,
		  IN	PVOID	ApcContext,
		  OUT	PVOID	IoStatusBlock,
		  IN	PVOID	Buffer,
		  IN	ULONG	Length,
		  IN	PVOID	ByteOffset
		  ));
#define KrnlNtWriteFile K_KERNEL_EXPORTS.NtWriteFile

XBOX_PROC(VOID, NtYieldExecution, (
		  ));
#define KrnlNtYieldExecution K_KERNEL_EXPORTS.NtYieldExecution