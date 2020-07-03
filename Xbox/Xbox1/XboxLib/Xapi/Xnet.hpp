/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(DWORD, XNetGetTitleXnAddr, (
		  XNADDR *pxna
		  ));
#define ApiNetGetTitleXnAddr XboxApi.XNetGetTitleXnAddr