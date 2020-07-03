/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(VOID, HalReadSMBusValue, (
		  ULONG   BusNumber,
		  ULONG   SlotNumber,
		  ULONG   RegisterNumber,
		  PVOID   Buffer,
		  ULONG   Length,
		  BOOLEAN WritePCISpace
		  ));
#define KrnlHalReadSMBusValue K_KERNEL_EXPORTS.HalReadSMBusValue

XBOX_PROC(VOID, HalReadWritePCISpace, (
		  IN ULONG   BusNumber,
		  IN ULONG   SlotNumber,
		  IN ULONG   RegisterNumber,
		  IN PVOID   Buffer,
		  IN ULONG   Length,
		  IN BOOLEAN WritePCISpace
		  ));
#define KrnlHalReadWritePCISpace K_KERNEL_EXPORTS.HalReadWritePCISpace

// DECLSPEC_NORETURN
XBOX_PROC(VOID, HalReturnToFirmware, (
		  RETURN_FIRMWARE Routine
		  ));
#define KrnlHalReturnToFirmware K_KERNEL_EXPORTS.HalReturnToFirmware

XBOX_PROC(ULONG, HalWriteSMBusValue, (
		  UCHAR   Address,
		  UCHAR   Command,
		  BOOLEAN WordFlag,
		  ULONG   Value
		  ));
#define KrnlHalWriteSMBusValue K_KERNEL_EXPORTS.HalWriteSMBusValue

XBOX_PROC(VOID, READ_PORT_BUFFER_UCHAR, (
		  IN PUCHAR Port,
		  IN PUCHAR Buffer,
		  IN ULONG  Count
		  ));
#define KrnlREAD_PORT_BUFFER_UCHAR K_KERNEL_EXPORTS.READ_PORT_BUFFER_UCHAR

XBOX_PROC(VOID, READ_PORT_BUFFER_USHORT, (
		  IN PUSHORT Port,
		  IN PUSHORT Buffer,
		  IN ULONG   Count
		  ));
#define KrnlREAD_PORT_BUFFER_USHORT K_KERNEL_EXPORTS.READ_PORT_BUFFER_USHORT

XBOX_PROC(VOID, READ_PORT_BUFFER_ULONG, (
		  IN PULONG Port,
		  IN PULONG Buffer,
		  IN ULONG  Count
		  ));
#define KrnlREAD_PORT_BUFFER_ULONG K_KERNEL_EXPORTS.READ_PORT_BUFFER_ULONG

XBOX_PROC(VOID, WRITE_PORT_BUFFER_UCHAR, (
		  IN PUCHAR Port,
		  IN PUCHAR Buffer,
		  IN ULONG  Count
		  ));
#define KrnlWRITE_PORT_BUFFER_UCHAR K_KERNEL_EXPORTS.WRITE_PORT_BUFFER_UCHAR

XBOX_PROC(VOID, WRITE_PORT_BUFFER_USHORT, (
		  IN PUSHORT Port,
		  IN PUSHORT Buffer,
		  IN ULONG   Count
		  ));
#define KrnlWRITE_PORT_BUFFER_USHORT K_KERNEL_EXPORTS.WRITE_PORT_BUFFER_USHORT

XBOX_PROC(VOID, WRITE_PORT_BUFFER_ULONG, (
		  IN PULONG Port,
		  IN PULONG Buffer,
		  IN ULONG  Count
		  ));
#define KrnlWRITE_PORT_BUFFER_ULONG K_KERNEL_EXPORTS.WRITE_PORT_BUFFER_ULONG