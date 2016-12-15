/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	CRC's a string. </summary>
		///
		/// <param name="str">	The string to CRC. </param>
		///
		/// <returns>	The CRC for the string. </returns>
		const uint32 CRCString(const char* str);
		
#ifdef DEBUG
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Verifies that the CRC for the provided string is correct. </summary>
		///
		/// <param name="str">	The string to verify. </param>
		/// <param name="crc">	The CRC to verify. </param>
		///
		/// <returns>	The provided CRC. </returns>
		const uint32 VerifyCRC(const char* str, uint32 crc);

		#define RESOURCE_ID_DEBUG(str) Yelo::Interface::OpenSauceUI::CRCString( (str) )
		#define RESOURCE_ID(str, crc) Yelo::Interface::OpenSauceUI::VerifyCRC( (str), (crc) )
#else
		#define RESOURCE_ID(str, crc) crc
#endif
		#define RESOURCE_ID_NONE 0xFFFFFFFF
	};};
};
#endif