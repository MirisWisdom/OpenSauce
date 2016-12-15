/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(PVOID, AvGetSavedDataAddress, (
		  ));
#define KrnlAvGetSavedDataAddress K_KERNEL_EXPORTS.AvGetSavedDataAddress

XBOX_PROC(VOID, AvSendTVEncoderOption, (
		  IN PVOID RegisterBase,
		  IN ULONG Option,
		  IN ULONG Param,
		  OUT ULONG* Result
		  ));
#define KrnlAvSendTVEncoderOption K_KERNEL_EXPORTS.AvSendTVEncoderOption

XBOX_PROC(ULONG, AvSetDisplayMode, (
		  IN PVOID RegisterBase,
		  IN ULONG Step,
		  IN ULONG Mode,
		  IN ULONG Format,
		  IN ULONG Pitch,
		  IN ULONG FrameBuffer
		  ));
#define KrnlAvSetDisplayMode K_KERNEL_EXPORTS.AvSetDisplayMode

XBOX_PROC(VOID, AvSetSavedDataAddress, (
		  IN PVOID Address
		  ));
#define KrnlAvSetSavedDataAddress K_KERNEL_EXPORTS.AvSetSavedDataAddress