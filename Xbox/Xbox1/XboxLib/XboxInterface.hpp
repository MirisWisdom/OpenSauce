/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

namespace XboxLib
{
	extern void DisplayLastError(); // !!! DEFINE IN YOUR DXT !!!

	BOOL IsEmulatorPresent();

	void XBOXAPI Initialize();
	void XBOXAPI Dispose();

	struct s_led_state
	{
		BYTE r1 : 1;
		BYTE r2 : 1;
		BYTE r3 : 1;
		BYTE r4 : 1;

		BYTE g1 : 1;
		BYTE g2 : 1;
		BYTE g3 : 1;
		BYTE g4 : 1;

		OVERRIDE_OPERATOR_CAST_THIS(BYTE);
	}; BOOST_STATIC_ASSERT( sizeof(s_led_state) == 1);

	void XBOXAPI SetLedStatus(DWORD led_state);

	void XBOXAPI EjectTray();

	void XBOXAPI LoadTry();

	void XBOXAPI RebootToDash();

	void XBOXAPI Reset();

	void XBOXAPI PowerOff();

	void XBOXAPI PowerCycle();

	void XBOXAPI GetCpuTemp(DWORD* out_value);

	void XBOXAPI GetSysTemp(DWORD* out_value);

	void XBOXAPI GetFanSpeed(DWORD* out_value);


	void* __fastcall _memcpy(void* dst, const void* src, size_t count);
	void XBOXAPI _memzero(void* dst, size_t count);

	bool XBOXAPI IsAddressValid(void* address);

	bool XBOXAPI Screenshot();
};