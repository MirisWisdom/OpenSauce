/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

//_DebugBreak@0



XBOX_PROC(void, GetSystemTimeAsFileTime, (
		  LPFILETIME lpSystemTimeAsFileTime
		  ));
#define ApiGetSystemTimeAsFileTime XboxApi.GetSystemTimeAsFileTime



//_wvsprintfW@12
//_wvsprintfA@12
//_DebugPrint
//_wsprintfW
//_wsprintfA