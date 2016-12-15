/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

//_XShaHmacInitialize@12
//_XShaHmacUpdate@12
//_XShaHmacComputeFinal@16
//_XapiConvertSignatureToNonRoamable@12
//_XapipCalculateSaveGameSignatureBegin@8
//_XapipCalculateSaveGameSignatureUpdate@12
//_XapipCalculateSaveGameSignatureEnd@8
//_XapipCalculateContentSignatureBegin@0
//_XapipCalculateContentSignatureUpdate@12
//_XapipCalculateContentSignatureEnd@8



XBOX_PROC(DWORD, XCalculateSignatureGetSize, (
		  DWORD dwFlags
		  ));
#define ApiCalcSigGetSize XboxApi.XCalculateSignatureGetSize



XBOX_PROC(HANDLE, XCalculateSignatureBeginEx, (
		  DWORD dwFlags,
		  DWORD dwAltTitleId
		  ));
#define ApiCalcSigBeginEx XboxApi.XCalculateSignatureBeginEx



XBOX_PROC(HANDLE, XCalculateSignatureBegin, (
		  DWORD dwFlags
		  ));
#define ApiCalcSigBegin XboxApi.XCalculateSignatureBegin



XBOX_PROC(DWORD, XCalculateSignatureUpdate, (
		  HANDLE hCalcSig,
		  const BYTE *pbData,
		  ULONG cbData
		  ));
#define ApiCalcSigUpdate XboxApi.XCalculateSignatureUpdate



XBOX_PROC(DWORD, XCalculateSignatureEnd, (
		  HANDLE hCalcSig,
		  PVOID pSignature
		  ));
#define ApiCalcSigEnd XboxApi.XCalculateSignatureEnd