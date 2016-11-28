/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

// ******************************************************************
// * File I/O
// ******************************************************************
	XBOX_STDPROC(FILE*, _fsopen, (
		__in_z const char * _Filename,
		__in_z const char * _Mode,
		__in int _ShFlag
	));
	#define Std_fsopen XboxStd.fsopen

	XBOX_STDPROC(int, fclose, (
		__inout FILE * _File
	));
	#define Std_fclose XboxStd.fclose

	XBOX_STDPROC(int, fflush, (
		__inout_opt FILE * _File
	));
	#define Std_fflush XboxStd.fflush

	XBOX_STDPROC(FILE*, fopen, (
		__in_z const char * _Filename,
		__in_z const char * _Mode
	));
	#define Std_fopen XboxStd.fopen

	XBOX_STDPROC(char*, fgets, (
		__out_ecount_z(_MaxCount) char * _Buf,
		__in int _MaxCount,
		__inout FILE * _File
	));
	#define Std_fgets XboxStd.fgets

	XBOX_STDPROC(int, fseek, (
		__inout FILE * _File,
		__in long _Offset,
		__in int _Origin
	));
	#define Std_fseek XboxStd.fseek

	XBOX_STDPROC(long, ftell, (
		__inout FILE * _File
	));
	#define Std_ftell XboxStd.ftell

	//_fopen