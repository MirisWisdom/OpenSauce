/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/guerilla/mfc_interfaces.hpp>

#if PLATFORM_TYPE == PLATFORM_GUERILLA
namespace Yelo
{
	namespace Guerilla
	{
		//////////////////////////////////////////////////////////////////////////
		// CBrush
		static API_FUNC_NAKED BOOL /*__thiscall*/ CBrush__Dtor(MFC_CBrush* brush)
		{
			static const uintptr_t FUNCTION = 0x404C10;

			API_FUNC_NAKED_START()
				mov		ecx, brush
				call	FUNCTION
			API_FUNC_NAKED_END(1)
		}
		static API_FUNC_NAKED BOOL /*__thiscall*/ CBrush__CreateSolidBrush(MFC_CBrush* brush, COLORREF color)
		{
			static const uintptr_t FUNCTION = 0x414060;

			API_FUNC_NAKED_START()
				mov		ecx, brush
				push	color
				call	FUNCTION
			API_FUNC_NAKED_END(2)
		}
		MFC_CBrush::MFC_CBrush(COLORREF color)	{ CBrush__CreateSolidBrush(this, color); }
		MFC_CBrush::~MFC_CBrush()				{ CBrush__Dtor(this); }

		//////////////////////////////////////////////////////////////////////////
		// CDC
		API_FUNC_NAKED COLORREF /*__thiscall*/ CDC__SetBkMode(void* CDC_, int mode)
		{
			static const uintptr_t FUNCTION = 0x856281;

			API_FUNC_NAKED_START()
				mov		ecx, CDC_
				push	mode
				call	FUNCTION
			API_FUNC_NAKED_END(2)
		}
		API_FUNC_NAKED COLORREF /*__thiscall*/ CDC__SetBkColor(void* CDC_, COLORREF color)
		{
			API_FUNC_NAKED_START()
				mov		ecx, CDC_
				mov		eax, [ecx]
				push	color
				call	[eax+0x30]
			API_FUNC_NAKED_END(2)
		}
		API_FUNC_NAKED COLORREF /*__thiscall*/ CDC__SetTextColor(void* CDC_, COLORREF color)
		{
			API_FUNC_NAKED_START()
				mov		ecx, CDC_
				mov		eax, [ecx]
				push	color
				call	[eax+0x34]
			API_FUNC_NAKED_END(2)
		}
	};
};
#endif