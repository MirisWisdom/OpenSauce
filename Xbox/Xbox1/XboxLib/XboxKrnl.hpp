/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once
#include "XboxLib.hpp"

#pragma region DirectX
XBOX_PROC(VOID, D3DDevice_Present, (
		  CONST RECT* pSourceRect,
		  CONST RECT* pDestRect,
		  void*,
		  void*
));


//D3DDevice_CreateTexture 80062BB0
//D3DDevice_CreateCubeTexture 80062BF0
//D3DDevice_Release 80062C30
//D3DDevice_SetRenderTarget 80062E60
//D3DDevice_GetBackBuffer 80063330
//D3DDevice_CreateRenderTarget 800633B0
//D3DDevice_CreateDepthStencilSurface 80063400

//D3DDevice_CreateIndexBuffer 800648D0


#pragma region SetRenderState
// ******************************************************************
// * SetRenderState
// ******************************************************************
XBOX_PROC(VOID, D3DDevice_SetRenderState_MultiSampleType, (
		  DWORD D3DMULTISAMPLE_TYPE /* D3DMULTISAMPLE_* */
));

XBOX_PROC(VOID, D3DDevice_SetRenderState_PSTextureModes, (
		  DWORD PS_TEXTUREMODE /* PS_TEXTUREMODES_* */
));

XBOX_PROC(VOID, D3DDevice_SetRenderState_EdgeAntiAlias, (
		  BOOL VALUE
));

XBOX_PROC(VOID, D3DDevice_SetRenderState_ShadowFunc, (
		  DWORD D3DCMPFUNC /* D3DCMP_* */
));

XBOX_PROC(VOID, D3DDevice_SetRenderState_FogColor, (
		  DWORD Color
));

XBOX_PROC(VOID, D3DDevice_SetRenderState_CullMode, (
		  DWORD D3DCULL /* D3DCULL_* */
));

XBOX_PROC(VOID, D3DDevice_SetRenderState_FrontFace, (
		  DWORD D3DFRONT /* D3DFRONT_* */
));

//D3DDevice_SetRenderState_NormalizeNormals
//D3DDevice_SetRenderState_TextureFactor
//D3DDevice_SetRenderState_LineWidth
//D3DDevice_SetRenderState_Dxt1NoiseEnable
//D3DDevice_SetRenderState_ZBias
//D3DDevice_SetRenderState_LogicOp
#pragma endregion
#pragma endregion


typedef struct _XBOX_KERNEL_VTABLE {
#pragma region DirectX
	// ******************************************************************
	// * Device
	// ******************************************************************
	struct {
		_D3DDevice_Present Present;

		// ******************************************************************
		// * SetRenderState
		// ******************************************************************
		struct {
			_D3DDevice_SetRenderState_MultiSampleType MultiSampleType;
			_D3DDevice_SetRenderState_PSTextureModes PSTextureModes;
			_D3DDevice_SetRenderState_EdgeAntiAlias EdgeAntiAlias;
			_D3DDevice_SetRenderState_ShadowFunc ShadowFunc;
			_D3DDevice_SetRenderState_FogColor FogColor;
			_D3DDevice_SetRenderState_CullMode CullMode;
			_D3DDevice_SetRenderState_FrontFace FrontFace;
		}SetRenderState;

	}DXDevice;
#pragma endregion

}XBOX_KERNEL_VTABLE;

typedef struct _XBOX_KERNEL_DATA {
// ******************************************************************
// * Keys
// ******************************************************************
	unsigned char* KeyCert;
	unsigned char* KeyHD;
	unsigned char* KeyAltSignatures;
	unsigned char* KeySignature;
	unsigned char* KeyLAN;
	unsigned char* KeyEEPROM;
}XBOX_KERNEL_DATA;



#include "Krnl/av.hpp"
#include "Krnl/ex.hpp"
#include "Krnl/fatx.hpp"
#include "Krnl/gdfx.hpp"
#include "Krnl/halx.hpp"
#include "Krnl/idex.hpp"
#include "Krnl/io.hpp"
#include "Krnl/ke.hpp"
#include "Krnl/ldrx.hpp"
#include "Krnl/mmx.hpp"
#include "Krnl/nt.hpp"
#include "Krnl/obx.hpp"
#include "Krnl/phy.hpp"
#include "Krnl/ps.hpp"
#include "Krnl/rawx.hpp"
#include "Krnl/rtl.hpp"
#include "Krnl/udfx.hpp"

#pragma region Kernel Exports
typedef struct _SystemPteRange
{
	LONG Unknown0;
	DWORD Unknown1; // from?
	DWORD Unknown2; // from?
	DWORD Unknown3; // to?
	DWORD* AvailablePages;
	void* RemoveZeroPage; // proc
}_SystemPteRange;

typedef struct _GlobalData
{
	void* RetailPfnRegion;
	_SystemPteRange* SystemPteRange;
	DWORD* AvailablePages;
	DWORD* AllocatedPagesByUsage;
	RTL_CRITICAL_SECTION* AddressSpaceLock;
	DWORD* VadRoot;
	DWORD* VadHint;
	DWORD* VadFreeHint;
}_GlobalData;

typedef struct _HardwareInfo
{
	DWORD Flags;
	bool Unknown0;
	bool Unknown1;
	bool Unknown2;
	bool Unknown3;
}_HardwareInfo;


XBOX_PROC(VOID, XcSHAInit, (
		  UCHAR *pbSHAContext
		  ));
#define KrnlXcSHAInit K_KERNEL_EXPORTS.XcSHAInit

XBOX_PROC(VOID, XcSHAUpdate, (
		  UCHAR *pbSHAContext,
		  UCHAR *pbInput,
		  ULONG dwInputLength
		  ));
#define KrnlXcSHAUpdate K_KERNEL_EXPORTS.XcSHAUpdate

XBOX_PROC(VOID, XcSHAFinal, (
		  UCHAR *pbSHAContext,
		  UCHAR *pbDigest
		  ));
#define KrnlXcSHAFinal K_KERNEL_EXPORTS.XcSHAFinal

// XBOX_PROC(, , (
// 		  ));

typedef struct _XBOX_EXPORT_TABLE {
	_AvGetSavedDataAddress AvGetSavedDataAddress;
	_AvSendTVEncoderOption AvSendTVEncoderOption;
	_AvSetDisplayMode AvSetDisplayMode;
	_AvSetSavedDataAddress AvSetSavedDataAddress;
	void* DbgBreakPoint;
	void* DbgBreakPointWithStatus;
	void* DbgLoadImageSymbols;
	void* DbgPrint;
	void* HalReadSMCTrayState;
	void* DbgPrompt;
	void* DbgUnLoadImageSymbols;
	void* ExAcquireReadWriteLockExclusive;
	void* ExAcquireReadWriteLockShared;
	_ExAllocatePool ExAllocatePool;
	void* ExAllocatePoolWithTag;
	void* ExEventObjectType;
	_ExFreePool ExFreePool;
	void* ExInitializeReadWriteLock;
	void* ExInterlockedAddLargeInteger;
	void* ExInterlockedAddLargeStatistic;
	void* ExInterlockedCompareExchange64;
	void* ExMutantObjectType;
	void* ExQueryPoolBlockSize;
	_ExQueryNonVolatileSetting ExQueryNonVolatileSetting;
	void* ExReadWriteRefurbInfo;
	void* ExRaiseException;
	void* ExRaiseStatus;
	void* ExReleaseReadWriteLock;
	_ExSaveNonVolatileSetting ExSaveNonVolatileSetting;
	void* ExSemaphoreObjectType;
	void* ExTimerObjectType;
	void* ExfInterlockedInsertHeadList;
	void* ExfInterlockedInsertTailList;
	void* ExfInterlockedRemoveHeadList;
	void* FscGetCacheSize;
	void* FscInvalidateIdleBlocks;
	_FscSetCacheSize FscSetCacheSize;
	void* HalClearSoftwareInterrupt;
	void* HalDisableSystemInterrupt;
	void* IdexDiskPartitionPrefixBuffer;
	PSTRING HalDiskModelNumber;
	PSTRING HalDiskSerialNumber;
	void* HalEnableSystemInterrupt;
	void* HalGetInterruptVector;
	_HalReadSMBusValue HalReadSMBusValue;
	_HalReadWritePCISpace HalReadWritePCISpace;
	void* HalRegisterShutdownNotification;
	void* HalRequestSoftwareInterrupt;
	_HalReturnToFirmware HalReturnToFirmware;
	_HalWriteSMBusValue HalWriteSMBusValue;
	void* InterlockedCompareExchange;
	void* InterlockedDecrement;
	void* InterlockedIncrement;
	void* InterlockedExchange;
	void* InterlockedExchangeAdd;
	void* InterlockedFlushSList;
	void* InterlockedPopEntrySList;
	void* InterlockedPushEntrySList;
	void* IoAllocateIrp;
	void* IoBuildAsynchronousFsdRequest;
	void* IoBuildDeviceIoControlRequest;
	void* IoBuildSynchronousFsdRequest;
	void* IoCheckShareAccess;
	void* IoCompletionObjectType;
	void* IoCreateDevice;
	_IoCreateFile IoCreateFile;
	_IoCreateSymbolicLink IoCreateSymbolicLink;
	void* IoDeleteDevice;
	_IoDeleteSymbolicLink IoDeleteSymbolicLink;
	unsigned long : 32; // unused
	void* IoFileObjectType; // structure
	void* IoFreeIrp;
	void* IoInitializeIrp;
	void* IoInvalidDeviceRequest;
	void* IoQueryFileInformation;
	void* IoQueryVolumeInformation;
	void* IoQueueThreadIrp;
	void* IoRemoveShareAccess;
	void* IoSetIoCompletion;
	void* IoSetShareAccess;
	void* IoStartNextPacket;
	void* IoStartNextPacketByKey;
	void* IoStartPacket;
	void* IoSynchronousDeviceIoControlRequest;
	void* IoSynchronousFsdRequest;
	void* IofCallDriver;
	void* IofCompleteRequest;
	bool* KdDebuggerEnabled;
	bool* KdDebuggerNotPresent;
	void* IoDismountVolume;
	void* IoDismountVolumeByName;
	void* KeAlertResumeThread;
	void* KeAlertThread;
	void* KeBoostPriorityThread;
	void* KeBugCheck;
	void* KeBugCheckEx;
	void* KeCancelTimer;
	void* KeConnectInterrupt;
	_KeDelayExecutionThread KeDelayExecutionThread;
	void* KeDisconnectInterrupt;
	void* KeEnterCriticalRegion;
	_GlobalData MmGlobalData;
	void* KeGetCurrentIrql;
	void* KeGetCurrentThread;
	void* KeInitializeApc;
	void* KeInitializeDeviceQueue;
	_KeInitializeDpc KeInitializeDpc;
	void* KeInitializeEvent;
	void* KeInitializeInterrupt;
	void* KeInitializeMutant;
	void* KeInitializeQueue;
	void* KeInitializeSemaphore;
	_KeInitializeTimerEx KeInitializeTimerEx;
	void* KeInsertByKeyDeviceQueue;
	void* KeInsertDeviceQueue;
	void* KeInsertHeadQueue;
	void* KeInsertQueue;
	void* KeInsertQueueApc;
	void* KeInsertQueueDpc;
	PDWORD KeInterruptTime;
	void* KeIsExecutingDpc;
	void* KeLeaveCriticalRegion;
	void* KePulseEvent;
	void* KeQueryBasePriorityThread;
	void* KeQueryInterruptTime;
	void* KeQueryPerformanceCounter;
	void* KeQueryPerformanceFrequency;
	_KeQuerySystemTime KeQuerySystemTime;
	void* KeRaiseIrqlToDpcLevel;
	void* KeRaiseIrqlToSynchLevel;
	void* KeReleaseMutant;
	void* KeReleaseSemaphore;
	void* KeRemoveByKeyDeviceQueue;
	void* KeRemoveDeviceQueue;
	void* KeRemoveEntryDeviceQueue;
	void* KeRemoveQueue;
	void* KeRemoveQueueDpc;
	void* KeResetEvent;
	void* KeRestoreFloatingPointState;
	void* KeResumeThread;
	void* KeRundownQueue;
	void* KeSaveFloatingPointState;
	void* KeSetBasePriorityThread;
	void* KeSetDisableBoostThread;
	void* KeSetEvent;
	void* KeSetEventBoostPriority;
	void* KeSetPriorityProcess;
	void* KeSetPriorityThread;
	_KeSetTimer KeSetTimer;
	void* KeSetTimerEx;
	void* KeStallExecutionProcessor;
	void* KeSuspendThread;
	void* KeSynchronizeExecution;
	PDWORD KeSystemTime;
	void* KeTestAlertThread;
	PDWORD KeTickCount;
	PDWORD KeTimeIncrement;
	void* KeWaitForMultipleObjects;
	void* KeWaitForSingleObject;
	void* KfRaiseIrql;
	void* KfLowerIrql;
	void* KiBugCheckData; // structure, 5 dwords
	void* KiUnlockDispatcherDatabase;
	PDWORD LaunchDataPage;
	_MmAllocateContiguousMemory MmAllocateContiguousMemory;
	_MmAllocateContiguousMemoryEx MmAllocateContiguousMemoryEx;
	_MmAllocateSystemMemory MmAllocateSystemMemory;
	void* MmClaimGpuInstanceMemory;
	void* MmCreateKernelStack;
	void* MmDeleteKernelStack;
	_MmFreeContiguousMemory MmFreeContiguousMemory;
	_MmFreeSystemMemory MmFreeSystemMemory;
	_MmGetPhysicalAddress MmGetPhysicalAddress;
	void* MmIsAddressValid;
	void* MmLockUnlockBufferPages;
	void* MmLockUnlockPhysicalPage;
	_MmMapIoSpace MmMapIoSpace;
	_MmPersistContiguousMemory MmPersistContiguousMemory;
	void* MmQueryAddressProtect;
	_MmQueryAllocationSize MmQueryAllocationSize;
	void* MmQueryStatistics;
	_MmSetAddressProtect MmSetAddressProtect;
	void* MmUnmapIoSpace;
	_NtAllocateVirtualMemory NtAllocateVirtualMemory;
	void* NtCancelTimer;
	_NtClearEvent NtClearEvent;
	_NtClose NtClose;
	void* NtCreateDirectoryObject;
	_NtCreateEvent NtCreateEvent;
	_NtCreateFile NtCreateFile;
	void* NtCreateIoCompletion;
	_NtCreateFile NtCreateMutant;
	void* NtCreateSemaphore;
	void* NtCreateTimer;
	void* NtDeleteFile;
	void* NtDeviceIoControlFile;
	_NtDuplicateObject NtDuplicateObject;
	void* NtFlushBuffersFile;
	_NtFreeVirtualMemory NtFreeVirtualMemory;
	void* NtFsControlFile;
	void* NtOpenDirectoryObject;
	_NtOpenFile NtOpenFile;
	void* NtOpenSymbolicLinkObject;
	void* NtProtectVirtualMemory;
	void* NtPulseEvent;
	void* NtQueueApcThread;
	_NtQueryDirectoryFile NtQueryDirectoryFile;
	void* NtQueryDirectoryObject;
	void* NtQueryEvent;
	_NtQueryFullAttributesFile NtQueryFullAttributesFile;
	_NtQueryInformationFile NtQueryInformationFile;
	void* NtQueryIoCompletion;
	void* NtQueryMutant;
	void* NtQuerySemaphore;
	void* NtQuerySymbolicLinkObject;
	void* NtQueryTimer;
	void* NtQueryVirtualMemory;
	_NtQueryVolumeInformationFile NtQueryVolumeInformationFile;
	_NtReadFile NtReadFile;
	void* NtReadFileScatter;
	void* NtReleaseMutant;
	void* NtReleaseSemaphore;
	void* NtRemoveIoCompletion;
	_NtResumeThread NtResumeThread;
	_NtSetEvent NtSetEvent;
	_NtSetInformationFile NtSetInformationFile;
	void* NtSetIoCompletion;
	void* NtSetSystemTime;
	void* NtSetTimerEx;
	void* NtSignalAndWaitForSingleObjectEx;
	void* NtSuspendThread;
	_NtUserIoApcDispatcher NtUserIoApcDispatcher;
	_NtWaitForSingleObject NtWaitForSingleObject;
	_NtWaitForSingleObjectEx NtWaitForSingleObjectEx;
	void* NtWaitForMultipleObjectsEx;
	_NtWriteFile NtWriteFile;
	void* NtWriteFileGather;
	_NtYieldExecution NtYieldExecution;
	void* ObCreateObject;
	void* ObDirectoryObjectType;
	void* ObInsertObject;
	void* ObMakeTemporaryObject;
	void* ObOpenObjectByName;
	void* ObOpenObjectByPointer;
	void* ObpObjectHandleTable; // structure
	void* ObReferenceObjectByHandle;
	void* ObReferenceObjectByName;
	void* ObReferenceObjectByPointer;
	void* ObSymbolicLinkObjectType;
	void* ObfDereferenceObject;
	void* ObfReferenceObject;
	void* PhyGetLinkState;
	void* PhyInitialize;
	void* PsCreateSystemThread;
	_PsCreateSystemThreadEx PsCreateSystemThreadEx;
	void* PsQueryStatistics;
	void* PsSetCreateThreadNotifyRoutine;
	_PsTerminateSystemThread PsTerminateSystemThread;
	void* PsThreadObjectType;
	_RtlAnsiStringToUnicodeString RtlAnsiStringToUnicodeString;
	_RtlAppendStringToString RtlAppendStringToString;
	void* RtlAppendUnicodeStringToString;
	void* RtlAppendUnicodeToString;
	void* RtlAssert;
	void* RtlCaptureContext;
	void* RtlCaptureStackBackTrace;
	void* RtlCharToInteger;
	_RtlCompareMemory RtlCompareMemory;
	void* RtlCompareMemoryUlong;
	void* RtlCompareString;
	void* RtlCompareUnicodeString;
	_RtlCopyString RtlCopyString;
	void* RtlCopyUnicodeString;
	void* RtlCreateUnicodeString;
	void* RtlDowncaseUnicodeChar;
	void* RtlDowncaseUnicodeString;
	_RtlEnterCriticalSection RtlEnterCriticalSection;
	void* RtlEnterCriticalSectionAndRegion;
	void* RtlEqualString;
	void* RtlEqualUnicodeString;
	void* RtlExtendedIntegerMultiply;
	void* RtlExtendedLargeIntegerDivide;
	void* RtlExtendedMagicDivide;
	void* RtlFillMemory;
	void* RtlFillMemoryUlong;
	void* RtlFreeAnsiString;
	unsigned long : 32; // unused
	void* RtlGetCallersAddress;
	_RtlInitAnsiString RtlInitAnsiString;
	void* RtlInitUnicodeString;
	_RtlInitializeCriticalSection RtlInitializeCriticalSection;
	void* RtlIntegerToChar;
	void* RtlIntegerToUnicodeString;
	_RtlLeaveCriticalSection RtlLeaveCriticalSection;
	void* RtlLeaveCriticalSectionAndRegion;
	void* RtlLowerChar;
	void* RtlMapGenericMask;
	_RtlMoveMemory RtlMoveMemory;
	void* RtlMultiByteToUnicodeN;
	void* RtlMultiByteToUnicodeSize;
	_RtlNtStatusToDosError RtlNtStatusToDosError;
	void* RtlRaiseException;
	void* RtlRaiseStatus;
	void* RtlTimeFieldsToTime;
	void* RtlTimeToTimeFields;
	void* RtlTryEnterCriticalSection;
	void* RtlUlongByteSwap;
	_RtlUnicodeStringToAnsiString RtlUnicodeStringToAnsiString;
	void* RtlUnicodeStringToInteger;
	void* RtlUnicodeToMultiByteN;
	void* RtlUnicodeToMultiByteSize;
	void* RtlUnwind;
	void* RtlUpcaseUnicodeChar;
	void* RtlUpcaseUnicodeString;
	void* RtlUpcaseUnicodeToMultiByteN;
	void* RtlUpperChar;
	void* RtlUpperString;
	void* RtlUshortByteSwap;
	void* RtlWalkFrameChain;
	_RtlZeroMemory RtlZeroMemory;
	PUCHAR XboxEEPROMKey; // 16
	_HardwareInfo XboxHardwareInfo;
	PUCHAR XboxHDKey; // 16
	PDWORD XboxKrnlVersion;
	PUCHAR XboxSignatureKey; // 16
	PSTRING XeImageFileName;
	void* XeLoadSection;
	void* XeUnloadSection;
	_READ_PORT_BUFFER_UCHAR READ_PORT_BUFFER_UCHAR;
	_READ_PORT_BUFFER_USHORT READ_PORT_BUFFER_USHORT;
	_READ_PORT_BUFFER_ULONG READ_PORT_BUFFER_ULONG;
	_WRITE_PORT_BUFFER_UCHAR WRITE_PORT_BUFFER_UCHAR;
	_WRITE_PORT_BUFFER_USHORT WRITE_PORT_BUFFER_USHORT;
	_WRITE_PORT_BUFFER_ULONG WRITE_PORT_BUFFER_ULONG;
	_XcSHAInit XcSHAInit;
	_XcSHAUpdate XcSHAUpdate;
	_XcSHAFinal XcSHAFinal;
	void* XcRC4Key;
	void* XcRC4Crypt;
	void* XcHMAC;
	void* XcPKEncPublic;
	void* XcPKDecPrivate;
	void* XcPKGetKeyLen;
	void* XcVerifyPKCS1Signature;
	void* XcModExp;
	void* XcDESKeyParity;
	void* XcKeyTable;
	void* XcBlockCrypt;
	void* XcBlockCryptCBC;
	void* XcCryptService;
	void* XcUpdateCrypto;
	void* RtlRip;
	PUCHAR XboxLANKey; // 16
	PUCHAR* XboxAlternateSignatureKeys; // 16x16
	PDWORD XePublicKeyData; // 70
	PDWORD HalBootSMCVideoMode;
	void* IdexChannelObject; // structure
	void* HalIsResetOrShutdownPending;
	void* IoMarkIrpMustComplete;
	void* HalInitiateShutdown;
	void* snprintf;
	void* sprintf;
	void* vsnprintf;
	void* vsprintf;
	void* HalEnableSecureTrayEject;
	void* HalWriteSMCScratchRegister;
	unsigned long : 32; // unused
	unsigned long : 32; // unused
	unsigned long : 32; // unused
	unsigned long : 32; // unused
	unsigned long : 32; // unused
	unsigned long : 32; // unused
	unsigned long : 32; // unused
	void* MmDbgAllocateMemory;
	void* MmDbgFreeMemory;
	void* MmDbgQueryAvailablePages;
	void* MmDbgReleaseAddress;
	void* MmDbgWriteCheck;
}XBOX_EXPORT_TABLE;
#pragma endregion

enum
{
	//////////////////////////////////////////////////////////////////////////
	// Devices on SMBus
	//////////////////////////////////////////////////////////////////////////

	SMBDEV_PIC16L = 0x20,
	SMBDEV_VIDEO_ENCODER_CONNEXANT = 0x8A,
	SMBDEV_VIDEO_ENCODER_FOCUS = 0xD4,
	SMBDEV_VIDEO_ENCODER_XCALIBUR = 0xE0,
	SMBDEV_TEMP_MONITOR = 0x98,
	SMBDEV_EEPROM = 0xA8,

	//////////////////////////////////////////////////////////////////////////
	// Commands that can be sent to the PIC
	//////////////////////////////////////////////////////////////////////////

	PIC16L_CMD_POWER = 2,
	PIC16L_CMD_DVDTRAPSTATE = 3,
	PIC16L_CMD_LED_MODE = 7,
	PIC16L_CMD_LED_REGISTER = 8,
	PIC16L_CMD_EJECT = 12,
	PIC16L_CMD_INTERRUPT_REASON = 17,
	PIC16L_CMD_RESET_ON_EJECT = 25,
	PIC16L_CMD_SCRATCH_REGISTER = 27,

	//////////////////////////////////////////////////////////////////////////
	// Commands that can be sent to the Video Encoder
	//////////////////////////////////////////////////////////////////////////

	VIDEO_ENCODER_CMD_DETECT = 0,

	//////////////////////////////////////////////////////////////////////////
	// Values For Video Encoder
	//////////////////////////////////////////////////////////////////////////

	VIDEO_ENCODER_UNKNOWN = 0,
	VIDEO_ENCODER_CONNEXANT,
	VIDEO_ENCODER_FOCUS,
	VIDEO_ENCODER_XCALIBUR,

	//////////////////////////////////////////////////////////////////////////
	// Sub commands for the PIC Power command
	//////////////////////////////////////////////////////////////////////////

	POWER_SUBCMD_RESET = 1 << 0,
	POWER_SUBCMD_CYCLE = 1 << 6,
	POWER_SUBCMD_POWER_OFF = 1 << 7,

	//////////////////////////////////////////////////////////////////////////
	// Sub Command for the PIC LED command
	//////////////////////////////////////////////////////////////////////////

	LED_MODE_SUBCMD_DEFAULT_MODE = 0,
	LED_MODE_SUBCMD_CUSTOM_MODE,

	//////////////////////////////////////////////////////////////////////////
	// Sub Command for the PIC LED command
	//////////////////////////////////////////////////////////////////////////

	LED_REGISTER_CYCLE3_GREEN = 1 << 0,
	LED_REGISTER_CYCLE2_GREEN = 1 << 1,
	LED_REGISTER_CYCLE1_GREEN = 1 << 2,
	LED_REGISTER_CYCLE0_GREEN = 1 << 3,
	LED_REGISTER_CYCLE3_RED = 1 << 4,
	LED_REGISTER_CYCLE2_RED = 1 << 5,
	LED_REGISTER_CYCLE1_RED = 1 << 6,
	LED_REGISTER_CYCLE0_RED = 1 << 7,

	//////////////////////////////////////////////////////////////////////////
	// Sub commands for the PIC EJECT command
	//////////////////////////////////////////////////////////////////////////

	EJECT_SUBCMD_EJECT = 0,
	EJECT_SUBCMD_LOAD,

	//////////////////////////////////////////////////////////////////////////
	// Reason for interrupt
	//////////////////////////////////////////////////////////////////////////

	INTERRUPT_REASON_BITVALUE_POWER_BUTTON = 1 << 0,
	INTERRUPT_REASON_BITVALUE_AV_REMOVED = 1 << 4,
	INTERRUPT_REASON_BITVALUE_EJECT_BUTTON = 1 << 5,

	//////////////////////////////////////////////////////////////////////////
	// Sub commands for Reset on Eject PIC command
	//////////////////////////////////////////////////////////////////////////

	RESET_ON_EJECT_SUBCMD_ENABLE = 0,
	RESET_ON_EJECT_SUBCMD_DISABLE,

	//////////////////////////////////////////////////////////////////////////
	// Scratch register values
	//////////////////////////////////////////////////////////////////////////

	SCRATCH_REGISTER_BITVALUE_EJECT_AFTER_BOOT = 1 << 0,
	SCRATCH_REGISTER_BITVALUE_DISPLAY_ERROR = 1 << 1,
	SCRATCH_REGISTER_BITVALUE_NO_ANIMATION = 1 << 2,
	SCRATCH_REGISTER_BITVALUE_RUN_DASHBOARD = 1 << 3,
};


extern XBOX_KERNEL_VTABLE K_KERNEL_5933;

//extern XBOX_KERNEL_DATA K_KERNELDATA_5933;

extern XBOX_EXPORT_TABLE K_KERNEL_EXPORTS;

extern const void* K_NV_FRAME_BUFFER_ADDRESS;
extern const DWORD K_NV_FRAME_BUFFER_SIZE;

void BuildKernelExportTable();