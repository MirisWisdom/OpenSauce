/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#include "XboxLib.hpp"

#if defined(__cplusplus)
extern "C" {
#endif
	BOOL k_xbox_lib_emulation_mode = FALSE;

	#pragma region CRT declares
	// will generate a warning of type '4744' because the linker is a dumb dipshit (it is a float fucktard! not an int)
	float _fltused; // When I tried to compile a code statement that set a floating point value, this POS cropped up and we don't compile with the CRT libs (where this is defined) so HACKA HACKA HACKA
	#pragma endregion


	#pragma region cr0 hacks
	#if defined(USE_CR0_HACK)
		boost::uint32_t CR0_REGISTER = 0;
	#endif
	#pragma endregion

#if defined(__cplusplus)
};
#endif